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

#include <kernel/boot/multiboot.h>

/*--------------------------------------------------------------------
						  LITERAL CONSTANTS
--------------------------------------------------------------------*/

#define STACK_SIZE                  0x4000
#define START_MAP_PA				0x0
#define END_MAP_PA					pa(_kernel_end)
#define PAGE_FRAME_SZ				0x1000
#define PG_TBL_ENTRY_SZ				0x4

#define MULTIBOOT_HEADER_FLAGS 		MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO 

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
.long MULTIBOOT_HEADER_MAGIC
.long MULTIBOOT_HEADER_FLAGS
.long -( MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS )

/*----------------------------------------------------------
Reserve a stack for the initial thread.
----------------------------------------------------------*/
.section .bootstrap_stack, "aw", @nobits
.align 16
stack_bottom:
.skip STACK_SIZE
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

// TODO: Define page offset in a header
#define __PAGE_OFFSET 0xC0000000
#define pa(X) ((X) - __PAGE_OFFSET)

// TODO: Move these to a header
#define pg_tbl_idx( x ) ( ( (x) >> 12 ) & 0x3FF )
#define pg_dir_idx( x ) ( ( (x) >> 22 ) & 0x3FF )

#define pg_dir_entry( x ) ( pa( boot_page_directory ) + ( pg_dir_idx( x ) * 4 ) )

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
	movl $START_MAP_PA, %esi
	movl $1023, %ecx

	/*------------------------------------------------------
	If esi is less than start addr go to next page. If
	esi is greater than end addr, we're done mapping.
	------------------------------------------------------*/
1:
	cmpl $START_MAP_PA, %esi
	jl 2f
	cmpl $END_MAP_PA, %esi
	jge 3f

	/*------------------------------------------------------
	Map page frame as "present, writable".

	TODO: Use linker variables to map pages for .text and 
		.rodata as read only.
	------------------------------------------------------*/
	movl %esi, %edx
	orl $0x003, %edx
	movl %edx, (%edi)

	/*------------------------------------------------------
	Go to next page frame
	------------------------------------------------------*/
2:
	addl $PAGE_FRAME_SZ, %esi
	addl $PG_TBL_ENTRY_SZ, %edi
	
	/*------------------------------------------------------
	Loop if there is still room in boot_page_table1[], i.e.
		ecx is not 0.
	------------------------------------------------------*/
	loop 1b

	// TODO: Check that esi >= END_MAP_PA

	/*------------------------------------------------------
	Install page table at both 0x0 and __PAGE_OFFSET
	------------------------------------------------------*/
3:
	movl $(pa(boot_page_table1) + 0x003), pg_dir_entry( 0 )
	movl $(pa(boot_page_table1) + 0x003), pg_dir_entry( __PAGE_OFFSET )

	/*------------------------------------------------------
	Set cr3 to the address of the boot_page_directory.
	------------------------------------------------------*/
	movl $pa(boot_page_directory), %ecx
	movl %ecx, %cr3

	/*------------------------------------------------------
	Enable paging and the write-protect bit.
	------------------------------------------------------*/
	movl %cr0, %ecx
	orl $0x80010000, %ecx
	movl %ecx, %cr0

	/*------------------------------------------------------
	Jump to higher half with an absolute jump.
	------------------------------------------------------*/
	lea 4f, %ecx
	jmp *%ecx

.section .text

4:
	/*------------------------------------------------------
	Unmap identity mapping and flush TLB
	------------------------------------------------------*/
	movl $0, boot_page_directory + 0

	movl %cr3, %ecx
	movl %ecx, %cr3

	/*------------------------------------------------------
	Setup stack
	------------------------------------------------------*/
	mov $stack_top, %esp

	/*------------------------------------------------------
	push multiboot info struct pointer and bootloader magic
	number
	------------------------------------------------------*/
	push %eax
	addl $__PAGE_OFFSET, %ebx
	push %ebx

	/*------------------------------------------------------
	Enter the high-level kernel
	------------------------------------------------------*/
	call kernel_main

	/*------------------------------------------------------
	Infinit loop if kernel accidentally returns
	------------------------------------------------------*/
	cli
1:	hlt
	jmp 1b
