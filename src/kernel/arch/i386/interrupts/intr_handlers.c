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

#include <interrupts/idt.h>

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
*       dflt_intr_hndlr
*
*   DESCRIPTION:
*       The default interrupt handler
*
*********************************************************************/
__attribute__((interrupt))
void dflt_intr_hndlr
    (
    struct interrupt_frame * 
                        frame
    )
    {
    printf("\nHello from the default interrupt handler!\n");
    } /* dflt_hndlr() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       dflt_excep_hndlr
*
*   DESCRIPTION:
*       The default exception handler
*
*********************************************************************/
__attribute__((interrupt))
void dflt_excep_hndlr
    (
    struct interrupt_frame *
                        frame,
    exception_code_t    e_code
    )
    {

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
__attribute__((interrupt))
void general_protection_hndlr
    (
    struct interrupt_frame *
                        frame,
    exception_code_t    e_code
    )
    {
    static const char name[][3] = { "GDT", "IDT", "LDT", "IDT" };

    /*------------------------------------------------------
    Fault was caused by bad segment selector
    ------------------------------------------------------*/
    if( e_code )
        {
        
        if( GET_SEL_ERR_E( e_code ) )
            {
            printf("GP Fault - External - E_CODE: %x", e_code );
            }
        else
            {
            printf( 
                "GP Fault - Selector - Table: %s, Selector: %d, E_CODE: %x", 
                name[(uint8_t)GET_SEL_ERR_TBL( e_code )], 
                (uint16_t)GET_SEL_ERR__SEL( e_code ), 
                e_code 
                );
            }
        }
    /*------------------------------------------------------
    Something else happened
    ------------------------------------------------------*/
    else
        {
        printf( "GP Fault - Unknown - E_CODE: %x", e_code );
        }

    } /* general_protection_hndlr() */
