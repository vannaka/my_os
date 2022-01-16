/*********************************************************************
*
*   MODULE:
*       boot.S
*
*   DESCRIPTION:
*       Kernel assembly entry point.
*
*********************************************************************/

/*--------------------------------------------------------------------
							   INCLUDES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
						  LITERAL CONSTANTS
--------------------------------------------------------------------*/

# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

/*--------------------------------------------------------------------
								TYPES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
						   MEMORY CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
							  VARIABLES
--------------------------------------------------------------------*/

/*----------------------------------------------------------
Declare a header as in the Multiboot Standard.
----------------------------------------------------------*/
.section .multiboot.data, "aw"
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/*----------------------------------------------------------
Reserve a stack for the initial thread.
----------------------------------------------------------*/
.section .bootstrap_stack, "aw", @nobits
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

/*----------------------------------------------------------
Preallocate pages used for paging. Don't hard-code addresses
and assume they are available, as the bootloader might have
loaded its multiboot structures or modules there. This lets
the bootloader know it must avoid the addresses.
----------------------------------------------------------*/
.section .bss, "aw", @nobits
	.align 4096
boot_page_directory:
	.skip 4096
boot_page_table1:
	.skip 4096
// Further page tables may be required if the kernel grows beyond 3 MiB.

/*--------------------------------------------------------------------
								MACROS
--------------------------------------------------------------------*/

# TODO: Define page offset in a header
#define __PAGE_OFFSET 0xC0000000
#define pa(X) ((X) - __PAGE_OFFSET)

/*--------------------------------------------------------------------
							  PROCEDURES
--------------------------------------------------------------------*/


/*********************************************************************
*
*   PROCEDURE NAME:
*       _start
*
*   DESCRIPTION:
*       The kernel entry point
*
*********************************************************************/
.section .multiboot.text, "ax"
.global _start
.type _start, @function
_start:
	/*------------------------------------------------------
	Map page frames 0 to _kernel_end at __PAGE_OFFSET
	------------------------------------------------------*/
	movl $pa(boot_page_table1), %edi
	# First address to map is address 0.
	movl $0, %esi
	# Map 1023 pages. The 1024th will be the VGA text buffer.
	movl $1023, %ecx

1:
	# Only map the kernel.
	cmpl $0x00100000, %esi
	jl 2f
	cmpl $pa(_kernel_end), %esi
	jge 3f

	# Map physical address as "present, writable". Note that this maps
	# .text and .rodata as writable. Mind security and map them as non-writable.
	movl %esi, %edx
	orl $0x003, %edx
	movl %edx, (%edi)

2:
	# Size of page is 4096 bytes.
	addl $4096, %esi
	# Size of entries in boot_page_table1 is 4 bytes.
	addl $4, %edi
	# Loop to the next entry if we haven't finished.
	loop 1b

3:
	# Map VGA video memory to 0xC03FF000 as "present, writable".
	movl $(0x000B8000 | 0x003), pa(boot_page_table1) + 1023 * 4

	# The page table is used at both page directory entry 0 (virtually from 0x0
	# to 0x3FFFFF) (thus identity mapping the kernel) and page directory entry
	# 768 (virtually from 0xC0000000 to 0xC03FFFFF) (thus mapping it in the
	# higher half). The kernel is identity mapped because enabling paging does
	# not change the next instruction, which continues to be physical. The CPU
	# would instead page fault if there was no identity mapping.

	# Map the page table to both virtual addresses 0x00000000 and 0xC0000000.
	movl $(pa(boot_page_table1) + 0x003), pa(boot_page_directory) + 0
	movl $(pa(boot_page_table1) + 0x003), pa(boot_page_directory) + 768 * 4

	# Set cr3 to the address of the boot_page_directory.
	movl $pa(boot_page_directory), %ecx
	movl %ecx, %cr3

	# Enable paging and the write-protect bit.
	movl %cr0, %ecx
	orl $0x80010000, %ecx
	movl %ecx, %cr0

	# Jump to higher half with an absolute jump. 
	lea 4f, %ecx
	jmp *%ecx

.section .text

4:
	# At this point, paging is fully set up and enabled.

	# Unmap the identity mapping as it is now unnecessary. 
	movl $0, boot_page_directory + 0

	# Reload crc3 to force a TLB flush so the changes to take effect.
	movl %cr3, %ecx
	movl %ecx, %cr3

	# Set up the stack.
	mov $stack_top, %esp

	# Enter the high-level kernel.
	call kernel_main

	# Infinite loop if the system has nothing more to do.
	cli
1:	hlt
	jmp 1b
