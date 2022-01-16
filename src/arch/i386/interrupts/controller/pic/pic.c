/*********************************************************************
*
*   MODULE:
*       pic.c
*
*   DESCRIPTION:
*       Programable Interupt Controller driver
*
*********************************************************************/

/*--------------------------------------------------------------------
                               INCLUDES
--------------------------------------------------------------------*/

#include <stdbool.h>
#include <stdint.h>

#include <kernel/types.h>
#include <kernel/interrupts/intc_types.h>

#include <interrupts/controller/pic/pic.h>
#include <interrupts/idt/idt.h>
#include <memory/mem_utils.h>

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

/*----------------------------------------------------------
PIC port addresses
----------------------------------------------------------*/
#define PIC1		    0x20		/* IO base addr for master PIC  */
#define PIC2		    0xA0		/* IO base addr for slave PIC   */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	    ( PIC1 + 1 )
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	    ( PIC2 + 1 )

/*----------------------------------------------------------
PIC command codes
----------------------------------------------------------*/
#define PIC_EOI		    0x20		/* End-of-interrupt command code*/
#define PIC_READ_IRR    0x0a        /* OCW3 irq ready next CMD read */
#define PIC_READ_ISR    0x0b        /* OCW3 irq service next CMD read*/

/*----------------------------------------------------------
Initialization Command Words
----------------------------------------------------------*/
#define ICW1_ICW4	0x01		    /* ICW4 (not) needed            */
#define ICW1_SINGLE	0x02		    /* Single (cascade) mode        */
#define ICW1_INTERVAL4	0x04        /* Call address interval 4 (8)  */
#define ICW1_LEVEL	0x08		    /* Level triggered (edge) mode  */
#define ICW1_INIT	0x10		    /* Initialization - required!   */
    
#define ICW4_8086	0x01		    /* 8086/88 (MCS-80/85) mode     */
#define ICW4_AUTO	0x02		    /* Auto (normal) EOI            */
#define ICW4_BUF_SLAVE	0x08	    /* Buffered mode/slave          */
#define ICW4_BUF_MASTER	0x0C	    /* Buffered mode/master         */
#define ICW4_SFNM	0x10		    /* Special fully nested (not)   */


/*--------------------------------------------------------------------
                                TYPES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                                MACROS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                           MEMORY CONSTANTS
--------------------------------------------------------------------*/
const uint32_t   pic_irq_hndlr_cnt = CNT_OF_ARRAY( pic_irq_hndlrs );
                                    /* The count of hndlers array   */

/*--------------------------------------------------------------------
                              VARIABLES
--------------------------------------------------------------------*/

struct irq_hndlr_type
                        pic_irq_hndlrs[ INTC_PIC_IRQ_CNT ]; 
                                    /* Registered irq handlers      */
/*--------------------------------------------------------------------
                              PROCEDURES
--------------------------------------------------------------------*/

static void pic_remap
    (
    uint8_t             offset1
    );


/*********************************************************************
*
*   PROCEDURE NAME:
*       pic_init
*
*   DESCRIPTION:
*       Initialize the PIC hardware
*
*********************************************************************/
void pic_init
    (
    void
    )
    {
    pic_remap( IRQ_NUM__00 );

    // TODO: Mask all irq's

    } /* pic_init() */
 

/*********************************************************************
*
*   PROCEDURE NAME:
*       pic_enable
*
*   DESCRIPTION:
*       En/disable the given irq in the PIC
*
*********************************************************************/
void pic_enable
    (
    uint32_t            irq,
    bool                enable
    )
    {
    // TODO: un/mask irq in PIC

    } /* pic_enable() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       pic_send_eoi
*
*   DESCRIPTION:
*       Send EOI to programmable interrupt controller
*
*********************************************************************/
void pic_send_eoi
    (
    uint32_t             irq     /* The irq to send eoi for          */     
    )
    {
	if( irq >= 8 )
        {
		port_out_byte( PIC2_COMMAND, PIC_EOI );
        }
    
	port_out_byte( PIC1_COMMAND, PIC_EOI );

    } /* pic_send_eoi() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       pic_irq_enable
*
*   DESCRIPTION:
*       Set/clear irq bit in irq mask
*
*********************************************************************/
void pic_irq_enable
    (
    uint32_t            irq,
    bool                enable
    )
    {
    /*------------------------------------------------------
    Local Variables
    ------------------------------------------------------*/
    uint16_t port;
    uint8_t value;
 
    /*------------------------------------------------------
    Get correct port for irq
    ------------------------------------------------------*/
    if( irq < 8 ) 
        {
        port = PIC1_DATA;
        } 
    else 
        {
        port = PIC2_DATA;
        irq -= 8;
        }

    /*------------------------------------------------------
    Get cur mask and un/set bit for irq
    ------------------------------------------------------*/
    value = port_in_byte( port );

    if( enable )
        {
        value &= ~( 1 << irq );
        }
    else
        {
        value |= ( 1 << irq );
        }

    port_out_byte( port, value );

    } /* pic_irq_enable() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       pic_irq_trigger_type
*
*   DESCRIPTION:
*       Set trigger type for given irq
*
*********************************************************************/
void pic_irq_trigger_type
    (
    uint32_t            irq,
    enum intc_trigger_type
                        trigger_type
    )
    {
    // TODO: Can we set this on a per irq basis?

    } /* pic_irq_trigger_type() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       pic_remap
*
*   DESCRIPTION:
*       Remap irqs from idt entries 0x8 and 0x70 to the indexes 
*       provided.
*
*********************************************************************/
void pic_remap
    (
    uint8_t             offset1
    )
    {
    /*------------------------------------------------------
    Local Variables
    ------------------------------------------------------*/
	uint8_t mask_1;
    uint8_t mask_2;
 
    /*------------------------------------------------------
    Save masks
    ------------------------------------------------------*/
	mask_1 = port_in_byte( PIC1_DATA );
	mask_2 = port_in_byte( PIC2_DATA );
 
    /*------------------------------------------------------
    Start the init sequence in cascade mode w/ ICW4 needed
    ------------------------------------------------------*/
	port_out_byte( PIC1_COMMAND, ICW1_INIT | ICW1_ICW4 );
	port_io_wait();
	port_out_byte( PIC2_COMMAND, ICW1_INIT | ICW1_ICW4 );
	port_io_wait();

    /*------------------------------------------------------
    ICW2 - Set new offsets
    ------------------------------------------------------*/
	port_out_byte( PIC1_DATA, offset1 );
	port_io_wait();
	port_out_byte( PIC2_DATA, offset1 + 8 );
	port_io_wait();

    /*------------------------------------------------------
    ICW3 - Setup cascade of PICs
    ------------------------------------------------------*/
	port_out_byte( PIC1_DATA, 4 );
	port_io_wait();
	port_out_byte( PIC2_DATA, 2 );
	port_io_wait();
 
    /*------------------------------------------------------
    ICW4 - Set 8086 mode
    ------------------------------------------------------*/
	port_out_byte( PIC1_DATA, ICW4_8086 );
	port_io_wait();
	port_out_byte( PIC2_DATA, ICW4_8086 );
	port_io_wait();
 
    /*------------------------------------------------------
    Restore saved masks
    ------------------------------------------------------*/
	port_out_byte( PIC1_DATA, mask_1 );
	port_out_byte( PIC2_DATA, mask_2 );

    } /* pic_remap() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       __pic_get_irq_reg
*
*   DESCRIPTION:
*       Get data from given pic data reg
*
*********************************************************************/
static uint16_t __pic_get_irq_reg( int ocw3 )
    {
    /* OCW3 to PIC CMD to get the register values.  PIC2 is chained, and
     * represents IRQs 8-15.  PIC1 is IRQs 0-7, with 2 being the chain */
    port_out_byte( PIC1_COMMAND, ocw3 );
    port_out_byte( PIC2_COMMAND, ocw3 );

    return( port_in_byte( PIC2_COMMAND ) << 8) | port_in_byte( PIC1_COMMAND );
    
    } /* __pic_get_irq_reg() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       pic_get_irr
*
*   DESCRIPTION:
*       Returns the combined value of the cascaded PICs irq request 
*       registers. The set bits coresponds to the IRQs that have been
*       asserted.
*
*********************************************************************/
uint16_t pic_get_irr
    (
    void
    )
    {
    return __pic_get_irq_reg( PIC_READ_IRR );

    } /* pic_get_irr() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       pic_get_isr
*
*   DESCRIPTION:
*       Returns the combined value of the cascaded PICs in-service 
*       registers. The set bit coresponds to the IRQ that the cpu
*       is currently servicing. 
*
*********************************************************************/
uint16_t pic_get_isr
    (
    void
    )
    {
    return __pic_get_irq_reg( PIC_READ_ISR );

    } /* pic_get_isr() */
