/*********************************************************************
*
*   MODULE:
*       idt.h
*
*   DESCRIPTION:
*       description
*
*********************************************************************/

#ifndef _KERNEL_IDT_H
#define _KERNEL_IDT_H

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

struct interrupt_frame
    {
    uint32_t            ip;
    uint32_t            cs;
    uint32_t            flags;
    } __attribute__((packed));

typedef uint32_t exception_code_t;

enum intr_vector_num_t
    {
    INT_NUM__FIRST = 0,

    INT_NUM__DIV_BY_ZERO = INT_NUM__FIRST,
    INT_NUM__DEBUG,
    INT_NUM__NMI,
    INT_NUM__BREAKPOINT,
    INT_NUM__OVERFLOW,
    INT_NUM__BOUND_RNG_EXCEDED,
    INT_NUM__INVALID_OPCODE,
    INT_NUM__DEV_NOT_AVAIL,
    INT_NUM__DOUBLE_FAULT,
    INT_NUM__RESERVED_09,
    INT_NUM__INVAL_TSS,
    INT_NUM__SEG_NOT_PRESSENT,
    INT_NUM__STACK_SEG_FAULT,
    INT_NUM__GEN_PROT_FAULT,
    INT_NUM__PAGE_FAULT,
    INT_NUM__RESERVED_15,
    INT_NUM__X87_FLT_PNT_EXCEPTION,
    INT_NUM__ALIGN_CHECK,
    INT_NUM__MACHINE_CHECK,
    INT_NUM__SIMD_FLT_PNT_EXCEPTION,
    INT_NUM__VIRT_EXCEPTION,
    INT_NUM__RESERVED_21,
    INT_NUM__RESERVED_22,
    INT_NUM__RESERVED_23,
    INT_NUM__RESERVED_24,
    INT_NUM__RESERVED_25,
    INT_NUM__RESERVED_26,
    INT_NUM__RESERVED_27,
    INT_NUM__RESERVED_28,
    INT_NUM__RESERVED_29,
    INT_NUM__SECURITY_EXCEPTION,
    INT_NUM__RESSERVED_31,
    
    IRQ_NUM__00,
    IRQ_NUM__01,
    IRQ_NUM__02,
    IRQ_NUM__03,
    IRQ_NUM__04,
    IRQ_NUM__05,
    IRQ_NUM__06,
    IRQ_NUM__07,
    IRQ_NUM__08,
    IRQ_NUM__09,
    IRQ_NUM__10,
    IRQ_NUM__11,
    IRQ_NUM__12,
    IRQ_NUM__13,
    IRQ_NUM__14,
    IRQ_NUM__15,
    IRQ_NUM__16,

    INT_NUM__CNT
    };

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


void dflt_intr_hndlr
    (
    struct interrupt_frame * 
                        frame
    );

void dflt_excep_hndlr
    (
    struct interrupt_frame *
                        frame,
    exception_code_t    e_code
    );

void general_protection_hndlr
    (
    struct interrupt_frame *
                        frame,
    exception_code_t    e_code
    );

#ifdef __cplusplus
}
#endif
#endif /* _KERNEL_IDT_H */