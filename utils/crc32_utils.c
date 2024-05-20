#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "dune2000.h"
#include "utils.h"

uint32_t GetCRC32(char *fileName)
{
    uint32_t crc32 = 0;
    
    FILE *file = fopen(fileName, "rb");
    if (file)
    {
        fseek(file, 0, SEEK_END);
        long int size = ftell(file);
        if (size > 0)
        {
            fseek(file, 0, SEEK_SET);
            void *buf = calloc(size, sizeof(char));
            if (buf)
            {
                fread(buf, sizeof(char), size, file);
                crc32 = Crc32_ComputeBuf(0, buf, size);
                free(buf);
            }
        }
        fclose(file);
    }
    
    return crc32;
}
