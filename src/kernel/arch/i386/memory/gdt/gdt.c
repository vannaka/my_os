/*********************************************************************
*
*   MODULE:
*       gdt.c
*
*   DESCRIPTION:
*       Global Descriptor Table
*
*********************************************************************/

/*--------------------------------------------------------------------
                               INCLUDES
--------------------------------------------------------------------*/

#include <assert.h>
#include <stdint.h>

#include <kernel/types.h>

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

#define GDT_ENTRIES		5

// Each define here is for a specific flag in the descriptor.
// Refer to the intel documentation for a description of what each one does.
#define SEG_DESCTYPE(x)  ((x) << 0x04) // Descriptor type (0 for system, 1 for code/data)
#define SEG_PRES(x)      ((x) << 0x07) // Present
#define SEG_SAVL(x)      ((x) << 0x0C) // Available for system use
#define SEG_LONG(x)      ((x) << 0x0D) // Long mode
#define SEG_SIZE(x)      ((x) << 0x0E) // Size (0 for 16-bit, 1 for 32)
#define SEG_GRAN(x)      ((x) << 0x0F) // Granularity (0 for 1B - 1MB, 1 for 4KB - 4GB)
#define SEG_PRIV(x)     (((x) &  0x03) << 0x05)   // Set privilege level (0 - 3)
 
#define SEG_DATA_RD        0x00 // Read-Only
#define SEG_DATA_RDA       0x01 // Read-Only, accessed
#define SEG_DATA_RDWR      0x02 // Read/Write
#define SEG_DATA_RDWRA     0x03 // Read/Write, accessed
#define SEG_DATA_RDEXPD    0x04 // Read-Only, expand-down
#define SEG_DATA_RDEXPDA   0x05 // Read-Only, expand-down, accessed
#define SEG_DATA_RDWREXPD  0x06 // Read/Write, expand-down
#define SEG_DATA_RDWREXPDA 0x07 // Read/Write, expand-down, accessed
#define SEG_CODE_EX        0x08 // Execute-Only
#define SEG_CODE_EXA       0x09 // Execute-Only, accessed
#define SEG_CODE_EXRD      0x0A // Execute/Read
#define SEG_CODE_EXRDA     0x0B // Execute/Read, accessed
#define SEG_CODE_EXC       0x0C // Execute-Only, conforming
#define SEG_CODE_EXCA      0x0D // Execute-Only, conforming, accessed
#define SEG_CODE_EXRDC     0x0E // Execute/Read, conforming
#define SEG_CODE_EXRDCA    0x0F // Execute/Read, conforming, accessed
 
#define FLAGS_CODE_KERNEL \
    ( SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
      SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
      SEG_PRIV(0)     | SEG_CODE_EXRD )

#define FLAGS_DATA_KERNEL \
    ( SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
      SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
      SEG_PRIV(0)     | SEG_DATA_RDWR )
 
#define FLAGS_CODE_USER \
    ( SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
      SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
      SEG_PRIV(3)     | SEG_CODE_EXRD )
 
#define FLAGS_DATA_USER \
    ( SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
      SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
      SEG_PRIV(3)     | SEG_DATA_RDWR )

/*--------------------------------------------------------------------
                                TYPES
--------------------------------------------------------------------*/


/* 8 byte segment descriptor */
struct desc_struct 
    {
    uint16_t            limit0;
    uint16_t            base0;
    uint16_t            base1: 8, 
                        type: 4, 
                        s: 1, 
                        dpl: 2, 
                        p: 1;
    uint16_t            limit1: 4, 
                        avl: 1, 
                        l: 1, 
                        d: 1, 
                        g: 1, 
                        base2: 8;
    } __attribute__((packed));

struct desc_ptr 
    {
	uint16_t            size;
	uint32_t            address;
    } __attribute__((packed));

/*--------------------------------------------------------------------
                                MACROS
--------------------------------------------------------------------*/

#define GDT_ENTRY_INIT(flags, base, limit)  \
    {                                       \
    .limit0 = (uint16_t) (limit),           \
    .limit1 = ((limit) >> 16) & 0x0F,       \
    .base0  = (uint16_t) (base),            \
    .base1  = ((base) >> 16) & 0xFF,        \
    .base2  = ((base) >> 24) & 0xFF,        \
    .type   = (flags & 0x0f),               \
    .s      = (flags >> 4) & 0x01,          \
    .dpl    = (flags >> 5) & 0x03,          \
    .p      = (flags >> 7) & 0x01,          \
    .avl    = (flags >> 12) & 0x01,         \
    .l      = (flags >> 13) & 0x01,         \
    .d      = (flags >> 14) & 0x01,         \
    .g      = (flags >> 15) & 0x01,         \
    }

/*--------------------------------------------------------------------
                           MEMORY CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                              VARIABLES
--------------------------------------------------------------------*/

__attribute__((aligned(0x1000)))
const struct desc_struct gdt[] =
    {
    [0] = { 0 },
    [1] = GDT_ENTRY_INIT( FLAGS_CODE_KERNEL, 0x0, 0xFFFFFFFF ),
    [2] = GDT_ENTRY_INIT( FLAGS_DATA_KERNEL, 0x0, 0xFFFFFFFF ),
    [3] = GDT_ENTRY_INIT( FLAGS_CODE_USER,   0x0, 0xFFFFFFFF ),
    [4] = GDT_ENTRY_INIT( FLAGS_DATA_USER,   0x0, 0xFFFFFFFF )
    };
static_assert( CNT_OF_ARRAY(gdt) == GDT_ENTRIES, "Too many or not enough GDT entries." );

struct desc_ptr gdt_ptr =
    {
    .size = sizeof(gdt) - 1,
    .address = (uint32_t)&gdt
    };

/*--------------------------------------------------------------------
                              PROCEDURES
--------------------------------------------------------------------*/

extern void do_install_gdt
       (
       struct desc_ptr *
       );

/*********************************************************************
*
*   PROCEDURE NAME:
*       gdt_init
*
*   DESCRIPTION:
*       Initialize GDT module
*
*********************************************************************/
void gdt_init
    (
    void
    )
    {
    do_install_gdt( &gdt_ptr );

    } /* gdt_init() */