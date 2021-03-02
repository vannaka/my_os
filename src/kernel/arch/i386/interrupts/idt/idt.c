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

#include <interrupts/idt/idt.h>

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

#define INTR_DFLT_FLAGS \
    ( PRES( 1 ) | PRIV( RING0 ) | SEGM( 0 ) | INTR_GATE_32 )

/*--------------------------------------------------------------------
                                TYPES
--------------------------------------------------------------------*/

struct desc_struct
    {
    uint16_t            offset0;    /* Offset bits 0..15            */
    uint16_t            selector;   /* A code seg selector in GDT   */
    uint8_t             pad0;       /* Unused - Set to zero         */
    uint8_t             type: 4,    /* Gate type                    */
                        s: 1,       /* Storage segment              */
                        dpl: 2,     /* Privilege - Ring level       */
                        p: 1;       /* Present?                     */
    uint16_t            offset1;    /* Offset bits 16..31           */
    } __attribute__((packed));

struct desc_ptr 
    {
	uint16_t            size;
	uint32_t            address;
    } __attribute__((packed));


/*--------------------------------------------------------------------
                                MACROS
--------------------------------------------------------------------*/

/* #define IDT_ENTRY_INIT(flags, selec_num, offset) \
//     {                                           \
//     .offset0  = (uint16_t) (offset),            \
//     .offset1  = ((offset) >> 16) & 0xFFFF,      \
//     .selector = (uint16_t) (selec_num),          \
//     .pad0     = 0x0,                            \
//     .type     = ((flags) & 0x0F),               \
//     .s        = ((flags) >> 4) & 0x01,          \
//     .dpl      = ((flags) >> 5) & 0x03,          \
//     .p        = ((flags) >> 7) & 0x01,          \
//     } */

/*--------------------------------------------------------------------
                           MEMORY CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                              VARIABLES
--------------------------------------------------------------------*/

// TODO:LMM: Does the IDT need to be aligned?
// __attribute__((aligned(0x1000)))
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

void install_interrupt
    (
    enum intr_vector_num_t   
                        idx,
    void *              hndlr,
    uint8_t             flags
    );


/*********************************************************************
*
*   PROCEDURE NAME:
*       intrrupt_init
*
*   DESCRIPTION:
*       Initialize interrupts
*
*********************************************************************/
void intrrupt_init
    (
    void
    )
    {
    /*------------------------------------------------------
    Setup Default handlers
    ------------------------------------------------------*/
    for( int i = 0; i < MAX_IDT_ENTRIES; i++ )
        {
        install_interrupt( i, dflt_intr_hndlr, INTR_DFLT_FLAGS );
        }

    /*------------------------------------------------------
    Setup specific handlers
    ------------------------------------------------------*/
    install_interrupt( INT_NUM__GEN_PROT_FAULT, general_protection_hndlr, INTR_DFLT_FLAGS );

    /*------------------------------------------------------
    Install IDT
    ------------------------------------------------------*/
    do_install_idt( &idt_ptr );

    } /* intrrupt_init() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       install_interrupt
*
*   DESCRIPTION:
*       Install handler into the IDT
*
*********************************************************************/
void install_interrupt
    (
    enum intr_vector_num_t   
                        idx,
    void *              hndlr,
    uint8_t             flags
    )
    {
    /*------------------------------------------------------
    Local Variables
    ------------------------------------------------------*/
    uint16_t hndl_lo;
    uint16_t hndl_hi;

    /*------------------------------------------------------
    Validate inputs
    ------------------------------------------------------*/
    if( idx >= MAX_IDT_ENTRIES )
        {
        assert( true );
        return;
        }

    /*------------------------------------------------------
    Get the low/high bytes of handler
    ------------------------------------------------------*/
    hndl_lo = (uint32_t)hndlr & 0xFFFF;
    hndl_hi = ( (uint32_t)hndlr >> 16 ) & 0xFFFF;

    /*------------------------------------------------------
    Install handler
    ------------------------------------------------------*/                                         
    idt[ idx ].offset0  = hndl_lo;
    idt[ idx ].offset1  = hndl_hi;
    idt[ idx ].selector = (uint16_t)(0x8); // TODO:LMM Replace with define
    idt[ idx ].pad0     = 0x0;
    idt[ idx ].type     = ((flags) & 0x0F);
    idt[ idx ].s        = ((flags) >> 4) & 0x01;
    idt[ idx ].dpl      = ((flags) >> 5) & 0x03;
    idt[ idx ].p        = ((flags) >> 7) & 0x01;

    } /* install_interrupt() */