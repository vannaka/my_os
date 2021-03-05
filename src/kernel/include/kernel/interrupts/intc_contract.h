/*********************************************************************
*
*   MODULE:
*       intc_contract.h
*
*   DESCRIPTION:
*       Architecture specific interrupt controller definitions
*
*********************************************************************/

#ifndef _KERNEL_INTC_CONTRACT_H
#define _KERNEL_INTC_CONTRACT_H

/*--------------------------------------------------------------------
                               INCLUDES
--------------------------------------------------------------------*/

#include <stdbool.h>
#include <stdint.h>

#include "intc.h"

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                                TYPES
--------------------------------------------------------------------*/

struct intc_cnfg
    {
    void (*intc_init)( void );
    void (*intc_ack)( uint32_t irq );
    void (*intc_hndlr)( uint32_t irq );
    void (*intc_reg_hndlr)
        (
        uint32_t            irq,
        enum intc_trigger_type
                            trigger_type,
        bool                auto_ack
        );
    };

struct irq_type
    {
    irq_hndlr_type *    hndlr;
    bool                auto_ack;
    enum intc_trigger_type   
                        trigger_type;
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


/*------------------------------------------------------
Arch defines this to install its low level int controler 
driver
------------------------------------------------------*/
void intc_install_low_level_driver
    (
    struct intc_cnfg *  cnfg,
    struct irq_type *   irq_info,
    uint32_t *          irq_info_cnt
    );


#ifdef __cplusplus
}
#endif
#endif /* _KERNEL_INTC_CONTRACT_H */