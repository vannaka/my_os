/*********************************************************************
*
*	MODULE:
*		i386/tty.c
*
*	DESCRIPTION:
*		x86 tty terminal driver
*
*********************************************************************/

/*--------------------------------------------------------------------
                              INCLUDES
--------------------------------------------------------------------*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include "vga.h"

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
// #define VGA_MEMORY  0xB8000
#define VGA_MEMORY  0xC03FF000 // Addr in higher half kernel mappings.

/*--------------------------------------------------------------------
                                TYPES
--------------------------------------------------------------------*/

enum update_type
    {
    UPDT_ONE,
    UPDT_TAB,
    UPDT_NEW_LINE
    };

/*--------------------------------------------------------------------
                          MEMORY CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                              VARIABLES
--------------------------------------------------------------------*/

static size_t       terminal_row;
static size_t       terminal_column;
static uint8_t      terminal_color;
static uint16_t *   terminal_buffer;

/*--------------------------------------------------------------------
                               MACROS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                             PROCEDURES
--------------------------------------------------------------------*/

void updt_cursor
    (
    enum update_type    updt_type
    );

bool hndl_spcl_chars
    (
    uint8_t             c
    );

/*********************************************************************
*
*  	PROCEDURE NAME:
*      terminal_initialize
*
*  	DESCRIPTION:
*      	Init tty terminal interface
*
*********************************************************************/
void terminal_initialize
    (
    void
    )
    {
    /*------------------------------------------------------
    Init module state vars
    ------------------------------------------------------*/
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color( VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK );
    terminal_buffer = (uint16_t*)VGA_MEMORY;

    /*------------------------------------------------------
    Clear screen
    ------------------------------------------------------*/
    for( size_t y = 0; y < VGA_HEIGHT; y++ )
        {
        for( size_t x = 0; x < VGA_WIDTH; x++ )
            {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[ index ] = vga_entry( ' ', terminal_color );
            }
        }
    } /* terminal_initialize() */


/*********************************************************************
*
*  	PROCEDURE NAME:
*      terminal_setcolor
*
*  	DESCRIPTION:
*      	Set the color of the terminal
*
*********************************************************************/
void terminal_setcolor
    (
    uint8_t             color       /* Color for future term chars  */
    )
    {
    terminal_color = color;

    } /* terminal_setcolor() */


/*********************************************************************
*
*  	PROCEDURE NAME:
*      terminal_putentryat
*
*  	DESCRIPTION:
*      	Put char at x,y char coordinate of terminal.
*
*********************************************************************/
void terminal_putentryat
    (
    unsigned char 		c, 			/* Char to write to term		*/
    uint8_t 			color, 		/* Color of char				*/
    size_t 				x, 			/* x coordinate					*/
    size_t 				y			/* y coordinate					*/
    )
    {
    /*------------------------------------------------------
    Local Variables
    ------------------------------------------------------*/
    const size_t index = y * VGA_WIDTH + x;
    
    /*------------------------------------------------------
    Write char to screen
    ------------------------------------------------------*/
    terminal_buffer[ index ] = vga_entry( c, color );

    } /* terminal_putentryat() */


/*********************************************************************
*
*  	PROCEDURE NAME:
*      terminal_putchar
*
*  	DESCRIPTION:
*      	Put char at cursor.
*
*********************************************************************/
void terminal_putchar
    (
    char 				c			/* char to write to term		*/
    )
    {
    /*------------------------------------------------------
    Local Variables
    ------------------------------------------------------*/
    unsigned char uc = c;
    
    /*------------------------------------------------------
    If char is special, handle is and return.
    ------------------------------------------------------*/
    if( hndl_spcl_chars( uc ) )
        return;
    
    /*------------------------------------------------------
    Put char at cursor
    ------------------------------------------------------*/
    terminal_putentryat( uc, terminal_color, terminal_column, terminal_row );

    /*------------------------------------------------------
    Update cursor position
    ------------------------------------------------------*/
    updt_cursor( UPDT_ONE );
    
    } /* terminal_putchar () */


/*********************************************************************
*
*  	PROCEDURE NAME:
*      terminal_initialize
*
*  	DESCRIPTION:
*      	Write string of chars to terminal.
*
*********************************************************************/
void terminal_write
    (
    const char *        data, 		/* String to write to term		*/
    size_t 				size		/* Number of chars to write		*/
    )
    {
    /*------------------------------------------------------
    Write chars to terminal
    ------------------------------------------------------*/
    for( size_t i = 0; i < size; i++ )
        {
        terminal_putchar( data[ i ] );
        }

    } /* terminal_write() */


/*********************************************************************
*
*  	PROCEDURE NAME:
*      terminal_writestring
*
*  	DESCRIPTION:
*      	Write null terminated string to terminal
*
*********************************************************************/
void terminal_writestring
    (
    const char *        data        /* Null terminated string       */
    )
    {
    /*------------------------------------------------------
    Write string to terminal
    ------------------------------------------------------*/
    terminal_write( data, strlen( data ) );

    } /* terminal_writestring() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       move_cursor
*
*   DESCRIPTION:
*       Move cursor
*
*********************************************************************/
void updt_cursor
    (
    enum update_type    updt_type
    )
    {
    switch( updt_type )
        {
        case UPDT_ONE:
            if( ++terminal_column == VGA_WIDTH )
                {
                terminal_column = 0;

                if( ++terminal_row == VGA_HEIGHT )
                    {
                    terminal_row = 0;
                    }
                }
            break;
        case UPDT_NEW_LINE:
            terminal_column = 0;
            
            if( ++terminal_row == VGA_HEIGHT )
                {
                terminal_row = 0;
                }
            break;
            
        case UPDT_TAB:
            if( terminal_column += 4 == VGA_WIDTH )
                {
                terminal_column = 0;
                }
            break;
            
        default:
            // TODO: THIS SHOULD NOT BE HIT. How do I want to handel cases like this. I need a logging function.
            break;
        }
    } /* move_cursor() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       hndl_spcl_chars
*
*   DESCRIPTION:
*       Move the cursor for special characters. Returns true if the
*       char was handled and shouldn't be written to the terminal.
*
*********************************************************************/
bool hndl_spcl_chars
    (
    uint8_t c
    )
    {
    bool ret = true;
    
    switch( c )
        {
        case '\n':
            updt_cursor( UPDT_NEW_LINE );
            break;
        
        case '\t':
            updt_cursor( UPDT_TAB );
            break;
            
        default:
            ret = false;
            break;
        }
    
    return ret;
    
    } /* hndl_spcl_chars() */
