/*********************************************************************
*
*   MODULE:
*       idt.c
*
*   DESCRIPTION:
*       Intterupt Descriptor Table
*
*********************************************************************/

/*--------------------------------------------------------------------
                               INCLUDES
--------------------------------------------------------------------*/

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include <kernel/types.h>
#include <kernel/interrupts/intc.h>

#include <interrupts/idt/idt.h>
#include <memory/mmu.h>

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

#define MAX_IDT_ENTRIES 256

/*----------------------------------------------------------
Privilege level
----------------------------------------------------------*/
#define RING0 0
#define RING1 1
#define RING2 2
#define RING3 3

/*----------------------------------------------------------
Attributes
----------------------------------------------------------*/
#define PRES( x )  ( ( x ) << 0x07 )
#define PRIV( x )  ( ( ( x ) & 0x03 ) << 0x05 )
#define SEGM( x )  ( ( x ) << 0x04 )

/*----------------------------------------------------------
Gate types
----------------------------------------------------------*/
#define TASK_GATE_32 0x5
#define INTR_GATE_16 0x6
#define TRAP_GATE_16 0x7
#define INTR_GATE_32 0xE
#define TRAP_GATE_32 0xF

/*--------------------------------------------------------------------
                                TYPES
--------------------------------------------------------------------*/

struct desc_struct
    {
    uint16_t            off_15_0;   /* Offset bits 0..15            */
    uint16_t            selector;   /* A code seg selector in GDT   */
    uint8_t             pad0;       /* Unused - Set to zero         */
    uint8_t             type: 4,    /* Gate type                    */
                        s: 1,       /* Storage segment              */
                        dpl: 2,     /* Privilege - Ring level       */
                        p: 1;       /* Present?                     */
    uint16_t            off_31_16;  /* Offset bits 16..31           */
    } __attribute__((packed));

struct desc_ptr 
    {
	uint16_t            size;
	uint32_t            address;
    } __attribute__((packed));


/*--------------------------------------------------------------------
                                MACROS
--------------------------------------------------------------------*/


/*********************************************************************
*
*   PROCEDURE NAME:
*       set_descriptor
*
*   DESCRIPTION:
*       Set up a normal interrupt/trap gate descriptor.
*     
*     - istrap: 1 for a trap (= exception) gate, 0 for an interrupt gate.
*       interrupt gate clears FL_IF, trap gate leaves FL_IF alone
*     - sel: Code segment selector for interrupt/trap handler
*     - off: Offset in code segment for interrupt/trap handler
*     - ring: Descriptor Privilege Level -
*            the privilege level required for software to invoke
*            this interrupt/trap gate explicitly using an int instruction.
*
*********************************************************************/
#define set_descriptor(gate, istrap, sel, off, ring)      \
{                                                         \
  (gate).off_15_0 = (uint32_t)(off) & 0xffff;             \
  (gate).selector = (sel);                                \
  (gate).pad0 = 0;                                        \
  (gate).type = (istrap) ? TRAP_GATE_32 : INTR_GATE_32;   \
  (gate).s = 0;                                           \
  (gate).dpl = (ring);                                    \
  (gate).p = 1;                                           \
  (gate).off_31_16 = (uint32_t)(off) >> 16;               \
}

/*--------------------------------------------------------------------
                           MEMORY CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                              VARIABLES
--------------------------------------------------------------------*/

static struct desc_struct idt[ MAX_IDT_ENTRIES ];

static struct desc_ptr idt_ptr =
    {
    .size = sizeof(idt) - 1,
    .address = (uint32_t)idt
    };


/*--------------------------------------------------------------------
                              PROCEDURES
--------------------------------------------------------------------*/

extern void do_install_idt
    (
    struct desc_ptr *
    );


/*********************************************************************
*
*   PROCEDURE NAME:
*       idt_init
*
*   DESCRIPTION:
*       Initialize iterrupt descriptor table
*
*********************************************************************/
void idt_init
    (
    void
    )
    {
    /*------------------------------------------------------
    Setup low level vector handlers
    ------------------------------------------------------*/
    for( int i = 0; i < MAX_IDT_ENTRIES; i++ )
        {
        set_descriptor( idt[i], false, SEG_KCODE<<3, vector_table[i], RING0 );
        }

    /*------------------------------------------------------
    Install IDT
    ------------------------------------------------------*/
    do_install_idt( &idt_ptr );

    } /* idt_init() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       vector_common_hi
*
*   DESCRIPTION:
*       All 256 exception vectors are sent here after the assembly
*       routines setup the exception frame.
*
*********************************************************************/
void vector_common_hi
    (
    struct exception_frame *frame
    )
    {
    
    if(INT_NUM__GEN_PROT_FAULT == frame->vec_num)
        {
        general_protection_hndlr(frame);
        }
    else if( ( IRQ_NUM__FIRST <= frame->vec_num )
          && ( IRQ_NUM__LAST  >= frame->vec_num ) )
        {
        /*--------------------------------------------------
        Call generic interrupt controller handler
        --------------------------------------------------*/
        intc_hndlr( IRQ_NUM__FIRST - frame->vec_num );
        }
    else
        {
        dflt_excep_hndlr(frame);
        }
    }
