/* GNU ld linker script for dune2000.exe */

ENTRY(_start);

SECTIONS
{
    .text      0x401000        : { dune2000.dat(.text) }
    .rdata     0x4BE000        : { dune2000.dat(.rdata) }
    .data      0x4D9000        : { dune2000.dat(.data); . = ALIGN(0x1000); }
    .bss       ALIGN(0x1000)   : { . = 0x3DDAD4; }
    .idata     0x8CF000        : { dune2000.dat(.idata) }

    /DISCARD/                  : { dune2000.dat(.rsrc) }
    /DISCARD/                  : { *(.drectve) }

    .p_text    ALIGN(0x1000)   : { *(.text) }
    .p_rdata   ALIGN(0x1000)   : { *(.rdata) }
    .p_data    ALIGN(0x1000)   : { *(.data) }
    .p_bss     ALIGN(0x1000)   : { *(.bss) *(COMMON) }

    .rsrc      ALIGN(0x1000)   : { *(.rsrc) }

    .patch     ALIGN(0x1000)   : { *(.patch) }
}
