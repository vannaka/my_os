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

#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
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
	char buf[ 33 ];
	size_t len;
	
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
			
			format++;
			written++;
			}
		else if( *format == 's' )
			{
			const char * str = va_arg( parameters, const char * );
			len = strlen( str );
			
			if( maxrem < len )
				{
				// TODO: Set errno to EOVERFLOW.
				return -1;
				}
			if( !print( str, len ) )
				{
				return -1;
				}
			
			format++;
			written += len;
			}
		else if( *format == 'd' 
			  || *format == 'i' )
			{
			int num = (int)va_arg( parameters, int );
						
			/*------------------------------------------------------
			itoa needs at least 33 bytes for every possible value.
			------------------------------------------------------*/
			if( sizeof( buf ) < 33 )
				{
				// TODO: Set errno to EOVERFLOW.
				return -1;
				}
			
			itoa( num, buf, 10 );
			
			len = strlen( buf );
			
			if( !print( buf, len ) )
				{
				return -1;
				}
			
			format++;
			written += len;
			
			}
		else if( *format == 'x'
			  || *format == 'X' )
			{
			uint32_t num = (int)va_arg( parameters, uint32_t );

			/*------------------------------------------------------
			itoa needs at least 33 bytes for every possible value.
			------------------------------------------------------*/
			if( sizeof( buf ) < 33 )
				{
				// TODO: Set errno to EOVERFLOW.
				return -1;
				}

			utoa( num, buf, 16 );

			len = strlen( buf );

			if( *format == 'X' )
				{
				for( size_t i = 0; i < len; i++ )
					{
					buf[ i ] = toupper( buf[ i ] );
					}
				}	
			
			if( !print( buf, len ) )
				{
				return -1;
				}

			format++;
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

