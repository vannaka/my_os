/*********************************************************************
*
*   MODULE:
*       gdt.h
*
*   DESCRIPTION:
*       description
*
*********************************************************************/

#ifndef _KERNEL_GDT_H
#define _KERNEL_GDT_H

/*--------------------------------------------------------------------
                               INCLUDES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

/*----------------------------------------------------------
Segment selectors

NOTE: shift left by three to use a selector
----------------------------------------------------------*/
#define SEG_KCODE 1  // kernel code
#define SEG_KDATA 2  // kernel data+stack
#define SEG_UCODE 3  // user code
#define SEG_UDATA 4  // user data+stack
#define SEG_TSS   5  // this process's task state


#define SEG_CNT	  5

#ifndef __ASSEMBLER__
/*--------------------------------------------------------------------
                                TYPES
--------------------------------------------------------------------*/

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

#ifdef __cplusplus
}
#endif
#endif /* __ASSEMBLER__ */
#endif /* _KERNEL_GDT_H */