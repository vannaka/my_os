/*********************************************************************
*
*   MODULE:
*       printf.c
*
*   DESCRIPTION:
*       printf definition
*
*********************************************************************/

/*--------------------------------------------------------------------
							   INCLUDES
--------------------------------------------------------------------*/

#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

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

static bool print
	(
	const char * 		data,
	size_t 				length
	);


/*********************************************************************
*
*  	PROCEDURE NAME:
*      printf
*
*  	DESCRIPTION:
*      	Processed format string and write it to stdio
*
*********************************************************************/
int printf
	( 
	const char * restrict 
						format, 	/* Formate string				*/
	...								/* Args to format into output	*/
	)
	{
	va_list parameters;
	va_start( parameters, format );

	int written = 0;

	while( *format != '\0' )
		{
		size_t maxrem = INT_MAX - written;

		if( format[ 0 ] != '%' || format[ 1 ] == '%' )
			{
			if( format[ 0 ] == '%' )
				{
				format++;
				}
			
			size_t amount = 1;
			while( format[ amount ] && format[ amount ] != '%' )
				{
				amount++;
				}
			
			if( maxrem < amount )
				{
				// TODO: Set errno to EOVERFLOW.
				return -1;
				}
			
			if( !print( format, amount ) )
				{
				return -1;
				}
			
			format += amount;
			written += amount;
			continue;
			}

		const char * format_begun_at = format++;

		if( *format == 'c' )
			{
			format++;
			char c = (char)va_arg( parameters, int /* char promotes to int */ );
			
			if( !maxrem )
				{
				// TODO: Set errno to EOVERFLOW.
				return -1;
				}
			
			if( !print( &c, sizeof( c ) ) )
				{
				return -1;
				}
			written++;
			}
		else if( *format == 's' )
			{
			format++;
			const char * str = va_arg( parameters, const char * );
			size_t len = strlen( str );
			
			if( maxrem < len )
				{
				// TODO: Set errno to EOVERFLOW.
				return -1;
				}
			if( !print( str, len ) )
				{
				return -1;
				}
			
			written += len;
			}
		else
			{
			format = format_begun_at;
			size_t len = strlen( format );
			if( maxrem < len )
				{
				// TODO: Set errno to EOVERFLOW.
				return -1;
				}
			if( !print( format, len ) )
				{
				return -1;
				}

			written += len;
			format += len;
			}
		}

	va_end( parameters );
	return written;

	} /* printf() */


/*********************************************************************
*
*  	PROCEDURE NAME:
*      print
*
*  	DESCRIPTION:
*      	Print chars to stdio while theres still room.
*
*********************************************************************/
static bool print
	(
	const char * data,
	size_t 				length
	)
	{
	const unsigned char * bytes = (const unsigned char *)data;

	for( size_t i = 0; i < length; i++ )
		{
		if( putchar( bytes[ i ] ) == EOF )
			{
			return false;
			}
		}

	return true;

	} /* print() */

