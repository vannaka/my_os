/* The bootloader will look at this image and start execution at the symbol
   designated at the entry point. */
ENTRY(_start)

/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
	/*------------------------------------------------------
	Place the entry stub code/data at 1 MB. It will map the
	rest of the kernel at 3GB + 1MB.
	------------------------------------------------------*/
	. = 0x00100000;

	.multiboot.data : {
		*(.multiboot.data)
	}

	.multiboot.text : {
		*(.multiboot.text)
	}

	/*------------------------------------------------------
	The kernel will live at 3GB + 1MB in the virtual address space,
	which will be mapped to 1MB in the physical address space.
	------------------------------------------------------*/
	. += 0xC0000000;
	
	_kernel_start = .;
	
	.text ALIGN (4K) : AT (ADDR (.text) - 0xC0000000)
	{
		*(.text)
	}
	.rodata ALIGN (4K) : AT (ADDR (.rodata) - 0xC0000000)
	{
		*(.rodata)
	}
	.data ALIGN (4K) : AT (ADDR (.data) - 0xC0000000)
	{
		*(.data)
	}
	.bss ALIGN (4K) : AT (ADDR (.bss) - 0xC0000000)
	{
		*(COMMON)
		*(.bss)
		*(.bootstrap_stack)
	}

	_kernel_end = .;
}
