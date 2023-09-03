/*********************************************************************
*
*   MODULE:
*       intc_arch.c
*
*   DESCRIPTION:
*       Arch interrupt controller definitions
*
*********************************************************************/

/*--------------------------------------------------------------------
                               INCLUDES
--------------------------------------------------------------------*/

#include <stddef.h>
#include <stdint.h>

#include <kernel/interrupts/intc_contract.h>

#include <interrupts/controller/pic/pic.h>

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

/*--------------------------------------------------------------------
                              PROCEDURES
--------------------------------------------------------------------*/

/*********************************************************************
*
*   PROCEDURE NAME:
*       install_pic_driver
*
*   DESCRIPTION:
*       Install the low level PIC driver
*
*********************************************************************/
void install_pic_driver
    (
    struct intc_cnfg *  cnfg,
    struct irq_hndlr_type **   
                        irq_hndlrs,
    uint32_t *          irq_hndlrs_cnt
    )
    {
    /*------------------------------------------------------
    Install low level driver
    ------------------------------------------------------*/
    cnfg->ack = pic_send_eoi;
    cnfg->enable = pic_irq_enable;
    cnfg->init = pic_init;
    cnfg->trigger_type = pic_irq_trigger_type;

    *irq_hndlrs = pic_irq_hndlrs;
    *irq_hndlrs_cnt = pic_irq_hndlr_cnt;

    } /* install_pic_driver() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       intc_install_low_level_driver
*
*   DESCRIPTION:
*       Install the low level interrupt controller driver
*
*********************************************************************/
void intc_install_low_level_driver
    (
    struct intc_cnfg *  cnfg,
    struct irq_hndlr_type **   
                        irq_hndlrs,
    uint32_t *          irq_hndlrs_cnt
    )
    {
    /*------------------------------------------------------
    Install low level driver

    TODO:LMM: Once APIC is supported, add logic here to
    install its driver instead.
    ------------------------------------------------------*/
    install_pic_driver(cnfg, irq_hndlrs, irq_hndlrs_cnt);

    } /* intc_install_low_level_driver() */