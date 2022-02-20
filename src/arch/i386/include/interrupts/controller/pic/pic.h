/*********************************************************************
*
*   MODULE:
*       pic.h
*
*   DESCRIPTION:
*       Programmable Interrupt Controller definitions
*
*********************************************************************/

#ifndef _KERNEL_PIC_H
#define _KERNEL_PIC_H

/*--------------------------------------------------------------------
                               INCLUDES
--------------------------------------------------------------------*/

#include <stdint.h>

#include <kernel/interrupts/intc_types.h>

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

#define INTC_PIC_IRQ_CNT 16

/*--------------------------------------------------------------------
                                TYPES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                           MEMORY CONSTANTS
--------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

extern const uint32_t   pic_irq_hndlr_cnt;
                                    /* The count of hndlers array   */

/*--------------------------------------------------------------------
                              VARIABLES
--------------------------------------------------------------------*/

extern struct irq_hndlr_type
                        pic_irq_hndlrs[ INTC_PIC_IRQ_CNT ]; 
                                    /* Registered irq handlers      */

/*--------------------------------------------------------------------
                                MACROS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                              PROCEDURES
--------------------------------------------------------------------*/

void pic_init
    (
    void
    );

void pic_send_eoi
    (
    uint32_t             irq     /* The irq to send eoi for          */     
    );

void pic_irq_enable
    (
    uint32_t            irq,
    bool                enable
    );

void pic_irq_trigger_type
    (
    uint32_t            irq,
    enum intc_trigger_type
                        trigger_type
    );

uint16_t pic_get_irr
    (
    void
    );

uint16_t pic_get_isr
    (
    void
    );



#ifdef __cplusplus
}
#endif
#endif /* _KERNEL_PIC_H */
