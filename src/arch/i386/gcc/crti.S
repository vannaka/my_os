/*********************************************************************
*
*   MODULE:
*       crti.s
*
*   DESCRIPTION:
*       Contains the headers for _init and _fini functions.
*
*********************************************************************/

/*--------------------------------------------------------------------
							   INCLUDES
--------------------------------------------------------------------*/

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
*       _init
*
*   DESCRIPTION:
*       Header of _init which will call global constuctors.
*
*	NOTE:
*		This is only the header of the function. The footer is in
*		crtn.s and gcc inserts the body automagically.
*
*********************************************************************/
.section .init
.global _init
.type _init, @function
_init:
	push %ebp
	movl %esp, %ebp
	/* gcc will nicely put the contents of crtbegin.o's .init section here. */


/*********************************************************************
*
*   PROCEDURE NAME:
*       _init
*
*   DESCRIPTION:
*       Header of _fini which will call global destuctors.
*
*	NOTE:
*		This is only the header of the function. The footer is in
*		crtn.s and gcc inserts the body automagically.
*
*********************************************************************/
.section .fini
.global _fini
.type _fini, @function
_fini:
	push %ebp
	movl %esp, %ebp
	/* gcc will nicely put the contents of crtbegin.o's .fini section here. */
