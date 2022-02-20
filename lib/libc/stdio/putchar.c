#include <stdio.h>

#if defined(__is_libk)
#include <kernel/tty/tty.h>
#endif

/*********************************************************************
*
*  	PROCEDURE NAME:
*      putchar
*
*  	DESCRIPTION:
*      	Write char to standard output.
*
*********************************************************************/
int putchar
	(
	int 				ic			/* Char to write to stdio		*/
	) 
	{
#if defined(__is_libk)
	char c = (char) ic;
	terminal_write(&c, sizeof(c));
#else
	// TODO: Implement stdio and the write system call.
	#error "USER MODE IS NOT SUPPORTED YET"
#endif
	return ic;
	
	} /* putchar() */
