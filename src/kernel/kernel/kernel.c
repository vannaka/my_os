#include <stdio.h>

#include <kernel/tty.h>

__attribute__( ( constructor ) ) void init_term( void )
	{
	terminal_initialize();
	printf( "Term initialized, printf is available at this point.\n" );
	}

void kernel_main(void) {
	// terminal_initialize();
	printf("Hello, kernel World!\n");
}
