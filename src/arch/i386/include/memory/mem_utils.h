/*********************************************************************
*
*   MODULE:
*       mem_utils.h
*
*   DESCRIPTION:
*       Memory utilities
*
*********************************************************************/

#ifndef _KERNEL_MEM_UTILS_H
#define _KERNEL_MEM_UTILS_H

/*--------------------------------------------------------------------
                               INCLUDES
--------------------------------------------------------------------*/

#include <stdint.h>

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                                TYPES
--------------------------------------------------------------------*/

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

/*********************************************************************
*
*   PROCEDURE NAME:
*       port_in_byte
*
*   DESCRIPTION:
*       Read in a byte from the given port
*
*********************************************************************/
static inline uint8_t port_in_byte
    (
    uint16_t            port
    )
    {
    /*------------------------------------------------------
    Local Variables
    ------------------------------------------------------*/
    uint8_t ret;

    /*------------------------------------------------------
    Read byte from port
    ------------------------------------------------------*/
    __asm__ volatile ( "inb %1, %0"
                   : 
                   "=a"(ret)
                   : 
                   "Nd"(port) 
                   );
    
    return ret;

    } /* port_in_byte() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       port_out_byte
*
*   DESCRIPTION:
*       Write a byte out to the given port
*
*********************************************************************/
static inline void port_out_byte
    (
    uint16_t            port,
    uint8_t             val
    )
    {
    /*------------------------------------------------------
    Write byte out to port
    ------------------------------------------------------*/
    __asm__ volatile ( 
        "outb %0, %1" 
        : : 
        "a"(val), 
        "Nd"(port) 
        );

    /*------------------------------------------------------
    There's an outb %al, $imm8  encoding, for compile-time 
    constant port numbers that fit in 8b.  (N constraint).
    Wider immediate constants would be truncated at assemble
    time (e.g. "i" constraint). The  outb  %al, %dx encoding
    is the only option for all other cases. %1 expands to 
    %dx because port is a uint16_t. %w1 could be used if we 
    had the port number a wider C type
    ------------------------------------------------------*/
    } /* port_out_byte() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       port_io_wait
*
*   DESCRIPTION:
*       Wait for io opperation to complete
*
*********************************************************************/
static inline void port_io_wait
    (
    void
    )
    {
    /*------------------------------------------------------
    Port 0x80 is used for 'checkpoints' during POST.
    The Linux kernel seems to think it is free for use :-/ 
    ------------------------------------------------------*/
    __asm__ volatile ( "outb %%al, $0x80" : : "a"(0) );

    } /* port_io_wait() */


#ifdef __cplusplus
}
#endif
#endif /* _KERNEL_MEM_UTILS_H */