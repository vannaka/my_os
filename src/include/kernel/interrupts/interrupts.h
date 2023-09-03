/*********************************************************************
*
*   MODULE:
*       interrupts.h
*
*   DESCRIPTION:
*       Interrupt definitions
*
*********************************************************************/

#ifndef _KERNEL_INTERRUPTS_H
#define _KERNEL_INTERRUPTS_H

/*--------------------------------------------------------------------
                               INCLUDES
--------------------------------------------------------------------*/

#include "interrupts_arch.h"

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

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

/*------------------------------------------------------
Architecture defines this to setup interrupts
------------------------------------------------------*/
void idt_init
    (
    void
    );


#ifdef __cplusplus
}
#endif
#endif /* _KERNEL_INTERRUPTS_H */