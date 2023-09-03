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

struct exception_frame
    {
    // registers pushed by pusha
    uint32_t    edi;
    uint32_t    esi;
    uint32_t    ebp;
    uint32_t    old_esp; // Value b4 pusha executed
    uint32_t    ebx;
    uint32_t    edx;
    uint32_t    ecx;
    uint32_t    eax;

    // Segment selectors and vector number
    uint16_t    gs;
    uint16_t    pad1;
    uint16_t    fs;
    uint16_t    pad2;
    uint16_t    es;
    uint16_t    pad3;
    uint16_t    ds;
    uint16_t    pad4;
    uint32_t    vec_num;

    // Below here pushed by x86 hardware
    uint32_t    error;
    uint32_t    eip;
    uint16_t    cs;
    uint16_t    pad5;
    uint32_t    eflags;
    
    // Below here only used when crossing rings, i.e. from user to kernel
    uint32_t    esp;
    uint16_t    ss;
    uint16_t    pad6;

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
    
    IRQ_NUM__FIRST,
    IRQ_NUM__00 = IRQ_NUM__FIRST,
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
    IRQ_NUM__LAST = IRQ_NUM__16,

    INT_NUM__CNT
    };

/*--------------------------------------------------------------------
                           MEMORY CONSTANTS
--------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t vector_table[];  // in vectors.S: array of 256 function pointers

/*--------------------------------------------------------------------
                              VARIABLES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                                MACROS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                              PROCEDURES
--------------------------------------------------------------------*/

void dflt_excep_hndlr
    (
    struct exception_frame *
                        frame
    );

void general_protection_hndlr
    (
    struct exception_frame *
                        frame
    );
    

#ifdef __cplusplus
}
#endif
#endif /* _KERNEL_IDT_H */