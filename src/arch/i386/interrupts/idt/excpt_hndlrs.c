/*********************************************************************
*
*   MODULE:
*       intr_handlers.c
*
*   DESCRIPTION:
*       Interupt Handlers
*
*********************************************************************/

/*--------------------------------------------------------------------
                               INCLUDES
--------------------------------------------------------------------*/

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <kernel/interrupts/intc.h>

#include <interrupts/idt/idt.h>
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

#define GET_SEL_ERR_E( x )      ( ( x ) & 0x01 )
#define GET_SEL_ERR_TBL( x )    ( ( ( x ) >> 1 ) & 0x03 )
#define GET_SEL_ERR__SEL( x )   ( ( ( x ) >> 3 ) & 0x01FFF )

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
*       dflt_excep_hndlr
*
*   DESCRIPTION:
*       The default exception handler
*
*********************************************************************/
void dflt_excep_hndlr
    (
    struct exception_frame *
                        frame
    )
    {
    assert(false);
    
    } /* dflt_excep_hndlr() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       general_protection_hndlr
*
*   DESCRIPTION:
*       The general protection fault exception handler
*
*********************************************************************/
void general_protection_hndlr
    (
    struct exception_frame *
                        frame
    )
    {
    static const char name[][3] = { "GDT", "IDT", "LDT", "IDT" };

    /*------------------------------------------------------
    Fault was caused by bad segment selector
    ------------------------------------------------------*/
    if( frame->error )
        {
        
        if( GET_SEL_ERR_E( frame->error ) )
            {
            printf("GP Fault - External - E_CODE: %x", frame->error );
            }
        else
            {
            printf( 
                "GP Fault - Selector - Table: %s, Selector: %d, E_CODE: %x", 
                name[(uint8_t)GET_SEL_ERR_TBL( frame->error )], 
                (uint16_t)GET_SEL_ERR__SEL( frame->error ), 
                frame->error 
                );
            }
        }
    /*------------------------------------------------------
    Something else happened
    ------------------------------------------------------*/
    else
        {
        printf( "GP Fault - Unknown - E_CODE: %x", frame->error );
        }

    } /* general_protection_hndlr() */
