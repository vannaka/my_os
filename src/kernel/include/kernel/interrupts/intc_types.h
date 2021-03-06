/*********************************************************************
*
*   MODULE:
*       intc_contract.h
*
*   DESCRIPTION:
*       Architecture specific interrupt controller definitions
*
*********************************************************************/

#ifndef _KERNEL_INTC_TYPES_H
#define _KERNEL_INTC_TYPES_H

/*--------------------------------------------------------------------
                               INCLUDES
--------------------------------------------------------------------*/

#include <stdbool.h>
#include <stdint.h>

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                                TYPES
--------------------------------------------------------------------*/

enum intc_trigger_type
    {
    INTC_TRIGGER__EDGE,
    INTC_TRIGGER__RISING
    };

typedef void (irq_hndlr_func)(uint32_t irq );

struct intc_cnfg
    {
    void (*init)( void );
    void (*ack)( uint32_t irq );
    void (*hndlr)( uint32_t irq );
    void (*trigger_type)( uint32_t irq, enum intc_trigger_type trigger_type );
    void (*enable)( uint32_t irq, bool enable );
    };

struct irq_hndlr_type
    {
    irq_hndlr_func *    hndlr;
    bool                auto_ack;
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


#ifdef __cplusplus
}
#endif
#endif /* _KERNEL_INTC_TYPES_H */