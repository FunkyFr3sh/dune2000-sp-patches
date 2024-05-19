/*
 * Copyright (c) 2012, 2013, 2014 Toni Spets <toni.spets@iki.fi>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include "macros/patch.h"
#include "patch.h"
#include "utils.h"

CALL(0x004081C4, _NetHack_SendTo);
CALL(0x0040822B, _NetHack_RecvFrom);
CALL(0x00407F4F, _NetHack_Socket);

// globals referenced in spawner
struct ListAddress AddressList[8];
int TunnelId;
int TunnelIp;
int TunnelPort;
int PortHack;
SOCKET SendSocket = INVALID_SOCKET;
uint32_t NetKey;
uint32_t P2Pheader;

void xormemcpy(void *dst, const void *src, size_t len, uint32_t key)
{
    char *s = (char *)src;
    char *d = (char *)dst;
    char *k = (char *)&key;
    for(uint32_t i = 0; i < len; i++)
        d[i]=s[i]^k[i%sizeof(uint32_t)];
}

WINAPI int Tunnel_SendTo(int sockfd, const void *buf, size_t len, int flags, struct sockaddr_in *dest_addr, int addrlen)
{
    char TempBuf[1024+8];

    uint32_t *crc32 = (void *)TempBuf+4;
    *crc32 = Crc32_ComputeBuf(0, buf, len);
    
    // copy packet to our buffer
    xormemcpy(TempBuf+8, buf, len, NetKey);
    
    if (TunnelPort)
    {
        uint16_t *BufFrom = (void *)TempBuf;
        uint16_t *BufTo = (void *)TempBuf + 2;
        
        // pull dest port to header
        *BufFrom = TunnelId;
        *BufTo = dest_addr->sin_port;

        dest_addr->sin_port = TunnelPort;
        dest_addr->sin_addr.s_addr = TunnelIp;
    }
    else
    {
        uint32_t *header = (void *)TempBuf;
        *header = P2Pheader;
    }
        
    return sendto(sockfd, TempBuf, len+8, flags, (struct sockaddr *)dest_addr, addrlen);
}

WINAPI int Tunnel_RecvFrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr_in *src_addr, int *addrlen)
{
    char TempBuf[1024+8];
    
    // call recvfrom first to get the packet
    int ret = recvfrom(sockfd, TempBuf, sizeof TempBuf, flags, (struct sockaddr *)src_addr, addrlen);
    
    if (ret <= 8)
        return -1;
    
    if (TunnelPort)
    {
        uint16_t *BufFrom = (void *)TempBuf;
        uint16_t *BufTo = (void *)TempBuf + 2;
    
        if (*BufTo != TunnelId)
            return -1;

        src_addr->sin_port = *BufFrom;
        src_addr->sin_addr.s_addr = 0;
    }
    else 
    {
        uint32_t *header = (void *)TempBuf;
        
        if (*header != P2Pheader)
            return -1;
    }
    
    xormemcpy(buf, TempBuf+8, ret-8, NetKey);
    
    uint32_t *crc32 = (void *)TempBuf+4;
    
    if (*crc32 != Crc32_ComputeBuf(0, buf, ret-8))
        return -1;
    
    return ret-8;
}

WINAPI int NetHack_SendTo(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr_in *dest_addr, int addrlen)
{
    if (!SpawnerActive)
        return sendto(sockfd, buf, len, flags, (struct sockaddr *)dest_addr, addrlen);
    
    struct sockaddr_in TempDest;

    // pull index
    int i = dest_addr->sin_addr.s_addr - 1;

    // validate index
    if (i >= 8 || i < 0)
        return -1;

    TempDest.sin_family         = AF_INET;
    TempDest.sin_port           = AddressList[i].port;
    TempDest.sin_addr.s_addr    = AddressList[i].ip;

    // do call to sendto
    return Tunnel_SendTo(sockfd, buf, len, flags, &TempDest, addrlen);
}

WINAPI int NetHack_RecvFrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr_in *src_addr, int *addrlen)
{
    if (!SpawnerActive)
        return recvfrom(sockfd, buf, len, flags, (struct sockaddr *)src_addr, addrlen);
    
    // call recvfrom first to get the packet
    int ret = Tunnel_RecvFrom(sockfd, buf, len, flags, src_addr, addrlen);
    
    // bail out if error
    if (ret == -1)
        return ret;

    // now, we need to map src_addr ip/port to index by reversing the search!
    for (int i = 0; i < 8; i++) {
        // compare ip
        if (src_addr->sin_addr.s_addr == AddressList[i].ip) {
            // compare port
            if (!PortHack && src_addr->sin_port != AddressList[i].port)
                continue;

            // found it, set this index to source addr
            src_addr->sin_addr.s_addr = i + 1;
            src_addr->sin_port = 0;
            break;
        }
    }

    return ret;
}

WINAPI SOCKET WSAAPI NetHack_Socket(int af, int type, int protocol)
{
    SendSocket = socket(af, type, protocol);
    return SendSocket;
}
