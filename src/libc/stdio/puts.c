#include <stdio.h>

/*********************************************************************
*
*  	PROCEDURE NAME:
*      puts
*
*  	DESCRIPTION:
*      	Write string to standard output.
*
*********************************************************************/
int puts
	(
	const char * 		string		/* String to write to stdio 	*/
	)
	{
	return printf( "%s\n", string );
	
	} /* puts() */
