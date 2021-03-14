/*********************************************************************
*
*   MODULE:
*       kernel.c
*
*   DESCRIPTION:
*       Kernel c entry point.
*
*********************************************************************/

/*--------------------------------------------------------------------
							   INCLUDES
--------------------------------------------------------------------*/

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include <kernel/boot/multiboot.h>
#include <kernel/interrupts/interrupts.h>
#include <kernel/interrupts/intc.h>
#include <kernel/tty/tty.h>
#include <kernel/memory/memory.h>

/*--------------------------------------------------------------------
						  LITERAL CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
								TYPES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
						   MEMORY CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
							  VARIABLES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
								MACROS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
							  PROCEDURES
--------------------------------------------------------------------*/

/*********************************************************************
*
*   PROCEDURE NAME:
*       kernel_main
*
*   DESCRIPTION:
*       C entry point of kernel
*
*********************************************************************/
void kernel_main
	(
	multiboot_info_t * 	mbd,
	unsigned int 		magic
	)
	{
	/*------------------------------------------------------
	Local Variables
	------------------------------------------------------*/
	// struct multiboot_mmap_entry *
	// 					mmap_entry;
	uint32_t 			ram_sz;

	if( mbd->flags & MULTIBOOT_INFO_MEMORY )
		{
		ram_sz = mbd->mem_lower + mbd->mem_upper;
		ram_sz *= 1024;
		}

	(void)ram_sz;

	// for( int i = 0; i < mbd->mmap_length; i++ )
	// 	{
		
	// 	}
	// mmap_entry = mbd->mmap_addr;

	/*------------------------------------------------------
	Init memory subsystem
	------------------------------------------------------*/
	memory_init();

	/*------------------------------------------------------
	Init Interrupts
	------------------------------------------------------*/
	interrupt_init();
	intc_init();

	/*------------------------------------------------------
	Init screen hw
	------------------------------------------------------*/
	terminal_initialize();
	
	/*------------------------------------------------------
	Say hi
	------------------------------------------------------*/
	printf( "Hello kernel world!\n" );
	printf( "It WORKS!\n\n" );
	printf( "Num: %d\n", 25 );
	printf( "addr== 0x%X\n", &kernel_main );
	
	/*------------------------------------------------------
	Enable interrupts
	------------------------------------------------------*/
	interrupts_enable();

	while( true );

	} /* kernel_main() */
