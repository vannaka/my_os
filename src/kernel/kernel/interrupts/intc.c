/*********************************************************************
*
*   MODULE:
*       intc.c
*
*   DESCRIPTION:
*       Interupt controller driver
*
*********************************************************************/

/*--------------------------------------------------------------------
                               INCLUDES
--------------------------------------------------------------------*/

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/interrupts/intc.h>
#include <kernel/interrupts/intc_contract.h>

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                                TYPES
--------------------------------------------------------------------*/



/*--------------------------------------------------------------------
                                MACROS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                           MEMORY CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                              VARIABLES
--------------------------------------------------------------------*/

static struct intc_cnfg intc_cnfg;  /* low level driver callbacks   */
static struct irq_type * 
                        irq_info;   /* Registered irq info          */
static uint32_t         irq_info_cnt;
                                    /* The count of info array      */

/*--------------------------------------------------------------------
                              PROCEDURES
--------------------------------------------------------------------*/

/*********************************************************************
*
*   PROCEDURE NAME:
*       intc_init
*
*   DESCRIPTION:
*       Init interrupt controller driver.
*
*********************************************************************/
void intc_init
    (
    void
    )
    {
    /*------------------------------------------------------
    Init variables
    ------------------------------------------------------*/
    memset( irq_info, 0, sizeof( irq_info ) );
    memset( &intc_cnfg, 0, sizeof( intc_cnfg ) );

    /*------------------------------------------------------
    Let arch install low level driver
    ------------------------------------------------------*/
    intc_install_low_level_driver( &intc_cnfg, irq_info, &irq_info_cnt );

    /*------------------------------------------------------
    Validate inputs
    ------------------------------------------------------*/
    assert( intc_cnfg.intc_init != NULL );
    assert( intc_cnfg.intc_ack != NULL );

    /*------------------------------------------------------
    Call low level driver init
    ------------------------------------------------------*/
    intc_cnfg.intc_init();

    }; /* intc_init() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       intc_ack
*
*   DESCRIPTION:
*       Send EOI to interrupt controller
*
*********************************************************************/
void intc_ack
    (
    uint32_t            irq         /* The IRQ number to clear      */
    )
    {
    /*------------------------------------------------------
    Call low level driver ack
    ------------------------------------------------------*/
    intc_cnfg.intc_ack( irq );

    } /* intc_ack() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       intc_register_irq_hndlr
*
*   DESCRIPTION:
*       Register handler for irq
*
*********************************************************************/
void intc_register_irq_hndlr
    (
    uint32_t            irq,
    enum intc_trigger_type
                        trigger_type,
    bool                auto_ack,
    irq_hndlr_type *    irq_hndlr
    )
    {
    /*------------------------------------------------------
    Validate inputs
    ------------------------------------------------------*/
    assert( irq < irq_info_cnt );
    assert( irq_hndlr != NULL );

    /*------------------------------------------------------
    Register handler w/ high level driver
    ------------------------------------------------------*/
    irq_info[ irq ].trigger_type = trigger_type;
    irq_info[ irq ].auto_ack = auto_ack;
    irq_info[ irq ].hndlr = irq_hndlr;

    /*------------------------------------------------------
    Register handler w/ low level driver
    ------------------------------------------------------*/
    intc_cnfg.intc_reg_hndlr( irq, trigger_type, auto_ack );

    } /* intc_register_irq_hndlr() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       intc_hndlr
*
*   DESCRIPTION:
*       Interrupt controller irq handler. Call this from the low level 
*       architecture specific handler.
*
*********************************************************************/
void intc_hndlr
    (
    uint32_t            irq
    )
    {
    /*------------------------------------------------------
    irq should never excede cnt set by low level driver
    ------------------------------------------------------*/
    assert( irq < irq_info_cnt );

    /*------------------------------------------------------
    Call registered irq handler
    ------------------------------------------------------*/
    irq_info[ irq ].hndlr( irq, irq_info[ irq ].auto_ack );

    /*------------------------------------------------------
    Call low level driver ack
    ------------------------------------------------------*/
    if( irq_info[ irq ].auto_ack )
        {
        intc_ack( irq );
        }
    
    } /* intc_hndlr() */