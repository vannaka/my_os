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
#include <stdio.h>
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
static struct irq_hndlr_type * 
                        irq_hndlrs; /* Registered irq handlers      */
static uint32_t         irq_hndlr_cnt;
                                    /* The count of hndlers array   */

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
    memset( &intc_cnfg, 0, sizeof( intc_cnfg ) );
    irq_hndlrs = NULL;
    irq_hndlr_cnt = 0;

    /*------------------------------------------------------
    Let arch install low level driver
    ------------------------------------------------------*/
    intc_install_low_level_driver( &intc_cnfg, &irq_hndlrs, &irq_hndlr_cnt );

    /*------------------------------------------------------
    Validate inputs
    ------------------------------------------------------*/
    assert( intc_cnfg.init != NULL );
    assert( intc_cnfg.ack != NULL );
    assert( intc_cnfg.trigger_type != NULL );
    assert( intc_cnfg.enable != NULL );

    /*------------------------------------------------------
    Call low level driver init
    ------------------------------------------------------*/
    intc_cnfg.init();

    }; /* intc_init() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       intc_trigger_type
*
*   DESCRIPTION:
*       Set trigger type
*
*********************************************************************/
void intc_trigger_type
    (
    uint32_t            irq,
    enum intc_trigger_type
                        trigger_type
    )
    {
    intc_cnfg.trigger_type( irq, trigger_type );

    } /* intc_trigger_type() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       intc_enable
*
*   DESCRIPTION:
*       Enable/Disable irq
*
*********************************************************************/
void intc_enable
    (
    uint32_t            irq,
    bool                enable
    )
    {
    intc_cnfg.enable( irq, enable );

    } /* intc_enable() */


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
    intc_cnfg.ack( irq );

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
    bool                enable,
    irq_hndlr_func *    irq_hndlr
    )
    {
    /*------------------------------------------------------
    Validate inputs
    ------------------------------------------------------*/
    assert( irq < irq_hndlr_cnt );
    assert( irq_hndlr != NULL );

    /*------------------------------------------------------
    Register handler w/ high level driver
    ------------------------------------------------------*/
    irq_hndlrs[ irq ].auto_ack = auto_ack;
    irq_hndlrs[ irq ].hndlr = irq_hndlr;

    /*------------------------------------------------------
    Configure low level driver
    ------------------------------------------------------*/
    intc_trigger_type( irq, trigger_type );
    intc_enable( irq, enable );

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
    assert( irq < irq_hndlr_cnt );

    /*------------------------------------------------------
    Call registered irq handler
    ------------------------------------------------------*/
    if( irq_hndlrs[ irq ].hndlr != NULL )
        {
        irq_hndlrs[ irq ].hndlr( irq );

        /*--------------------------------------------------
        Call low level driver ack
        --------------------------------------------------*/
        if( irq_hndlrs[ irq ].auto_ack )
            {
            intc_ack( irq );
            }
        }
    else
        {
        /*--------------------------------------------------
        Allways ack irq without a handler
        --------------------------------------------------*/
        intc_ack( irq );
        }
    
    } /* intc_hndlr() */
