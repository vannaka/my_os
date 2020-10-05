/*********************************************************************
*
*  	MODULE:
*      	vga.h
*
*  	DESCRIPTION:
*      	VGA graphics.
*
*********************************************************************/

#ifndef ARCH_I386_VGA_H
#define ARCH_I386_VGA_H

/*--------------------------------------------------------------------
							   INCLUDES
--------------------------------------------------------------------*/

#include <stdint.h>

/*--------------------------------------------------------------------
						  LITERAL CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
								TYPES
--------------------------------------------------------------------*/
	
enum vga_color
	{
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
	};
	
/*--------------------------------------------------------------------
						   MEMORY CONSTANTS
--------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

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
*  	PROCEDURE NAME:
*      vga_entry_color
*
*  	DESCRIPTION:
*      	Given foreground and backgroud color, return value encoding
*		both.
*
*********************************************************************/
static inline uint8_t vga_entry_color
	(
	enum vga_color fg,
	enum vga_color bg
	)
	{
	return fg | bg << 4;

	} /* vga_entry_color() */


/*********************************************************************
*
*  PROCEDURE NAME:
*      vga_entry
*
*  DESCRIPTION:
*      Given char and color, return value ready to be sent to screen.
*
*********************************************************************/
static inline uint16_t vga_entry
	( 
	unsigned char uc, 
	uint8_t color 
	)
	{
	return (uint16_t)uc | (uint16_t)color << 8;

	} /* vga_entry() */


#ifdef __cplusplus
}
#endif
#endif /* ARCH_I386_VGA_H */
