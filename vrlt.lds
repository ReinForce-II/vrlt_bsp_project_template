OUTPUT_FORMAT("elf32-littleriscv", "elf32-littleriscv", "elf32-littleriscv")
OUTPUT_ARCH(riscv)
ENTRY(Reset_Handler)

MEMORY {
    OCM (XRW) : ORIGIN = 0x80000000, LENGTH = 32K
    SDRAM (XRW) : ORIGIN = 0x40000000, LENGTH = 8192K
    FLASH (RX) : ORIGIN = 0x00000000, LENGTH = 256K
}

__stack_start = ORIGIN(SDRAM) + LENGTH(SDRAM);
__min_heap_size = 0x0;
__min_stack_size = 0x0;

SECTIONS
{
    .text :
    {
        . = ALIGN(4);
        KEEP(*(.reset_handler))
        KEEP(*(.isr_vector))
        *(.text);
        *(.text.*);
        . = ALIGN(4);
        __text_end = .;        /* define a global symbols at end of code */
    } > SDRAM

    .init :
    {
        . = ALIGN(4);
        KEEP (*(.init))
        . = ALIGN(4);
    } > SDRAM

    .fini : {
        . = ALIGN(4);
        KEEP (*(.fini))
        . = ALIGN(4);
    } > SDRAM

    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)         /* .rodata sections (constants, strings, etc.) */
        *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
        *(.gnu.linkonce.r.*)
        . = ALIGN(4);
    } > SDRAM

    .sdata : {
        . = ALIGN(4);
        __sdata_start = .;
        __global_pointer = . + 0x800;
        *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2) *(.srodata*)
        *(.sdata .sdata.* .gnu.linkonce.s.*)
        . = ALIGN(4);
        __sdata_end = .;
    } > SDRAM

    .data : {
        . = ALIGN(4);
        __data_start = .;
        *(.data)
        *(.data.*)
        *(.gnu.linkonce.d.*)
        . = ALIGN(4);
        __data_end = .;
    } > SDRAM
    
    .sbss : {
        . = ALIGN(4);
        __sbss_start = .;
        *(.sbss)
        *(.sbss.*)
        *(.gnu.linkonce.sb.*)
        . = ALIGN(4);
        __sbss_end = .;
    } > SDRAM

    .bss : {
        . = ALIGN(4);
        __bss_start = .;
        *(.bss)
        *(.bss.*)
        *(.gnu.linkonce.b.*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end = .;
    } > SDRAM

    .heap_stack :
    {
        . = ALIGN(4);
        __heap_start = .;
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + __min_heap_size;
        . = . + __min_stack_size;
        . = ALIGN(4);
    } > SDRAM
}
