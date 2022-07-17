/*********************************************************************
*
*   MODULE:
*       page_frame.c
*
*   DESCRIPTION:
*       Page frame allocator
*
*********************************************************************/

/*--------------------------------------------------------------------
                               INCLUDES
--------------------------------------------------------------------*/

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/types.h>

#include <bit_array.h>

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

// TODO: Move arch to header
#define PAGE_SHIFT  12ULL
#define PAGE_SIZE   (1ULL << PAGE_SHIFT)
#define PAGE_MASK   (~(PAGE_SIZE - 1))

#define MAX_PAGES   ( ( UINTPTR_MAX + 1ULL ) >> PAGE_SHIFT )


/*--------------------------------------------------------------------
                                TYPES
--------------------------------------------------------------------*/

enum page_frame_flags
    {
    // __PAGE_FRAME_FLAGS__FREE = 1,
    __PAGE_FRAME_FLAGS__ALLOCATED = 1,
    __PAGE_FRAME_FLAGS__RESERVED,

    // PAGE_FRAME_FLAGS__FREE        = 1ULL << __PAGE_FRAME_FLAGS__FREE,
    PAGE_FRAME_FLAGS__ALLOCATED   = 1ULL << __PAGE_FRAME_FLAGS__ALLOCATED,
    PAGE_FRAME_FLAGS__RESERVED    = 1ULL << __PAGE_FRAME_FLAGS__RESERVED
    };

struct page_frame
    {
    enum page_frame_flags
                        flags;
    uintptr_t           p_addr;     /* Physical address     */
    };

/*--------------------------------------------------------------------
                                MACROS
--------------------------------------------------------------------*/

#define frame_alloc( pg_num )   BA_SET( used_pages, pg_num )
#define frame_free( pg_num )    BA_CLR( used_pages, pg_num )

#define addr2frame( pg_addr )    ( ( pg_addr ) >> PAGE_SHIFT )
#define frame2addr( pg_frame )   ( ( pg_frame ) << PAGE_SHIFT )
// #define frame_cnt( start, end )  ( ( ( end ) - ( start ) ) >> PAGE_SHIFT )

/*--------------------------------------------------------------------
                           MEMORY CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                              VARIABLES
--------------------------------------------------------------------*/

static BA_CREATE32( used_pages, MAX_PAGES );
                                    /* Bit array of used pages      */
static uint32_t         last_alloc_frame;
                                    /* Last allocated frame         */

static struct page_frame * 
                        page_frames;/* Array of page frames         */ 

/*--------------------------------------------------------------------
                              PROCEDURES
--------------------------------------------------------------------*/

void page_frame_reserve
    (
    uint32_t            page_frame,
    uint32_t            cnt
    );


/*********************************************************************
*
*   PROCEDURE NAME:
*       page_frame_pwrp
*
*   DESCRIPTION:
*       Power up page frame module
*
*********************************************************************/
void page_frame_pwrp
    (
    uint32_t            ram_sz,     /* Size of RAM in bytes         */
    uintptr_t           ram_base    /* 4k aligned addr after end of */
                                    /* kernel                       */
    )
    {
    /*------------------------------------------------------
    Local Variables
    ------------------------------------------------------*/
    uint32_t pgs_in_system = ( ram_sz + PAGE_SIZE - 1 ) >> PAGE_SHIFT;
    uint32_t page_frames_sz = pgs_in_system * sizeof( struct page_frame );

    /*------------------------------------------------------
    Validate input
    ------------------------------------------------------*/
    assert( (ram_sz & PAGE_MASK) == 0 );
    assert( (ram_base & PAGE_MASK) == 0 );

    /*------------------------------------------------------
    Init module variables
    ------------------------------------------------------*/
    memset( used_pages, 0x0, sizeof( used_pages ) );
    last_alloc_frame = 0;

    /*------------------------------------------------------
    Allocate page frame structures
    ------------------------------------------------------*/
    page_frames = (struct page_frame *)ram_base;

    /*------------------------------------------------------
    Mark page_frames[MAX_PAGES] as used
    ------------------------------------------------------*/
    page_frame_reserve( addr2frame(ram_base), ( ( page_frames_sz + PAGE_SIZE - 1 ) >> PAGE_SHIFT ) );

    } /* page_frame_pwrp() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       page_frame_reserve
*
*   DESCRIPTION:
*       Mark page frames as allocated
*
*********************************************************************/
void page_frame_reserve
    (
    uint32_t            page_frame,
    uint32_t            cnt
    )
    {
    /*------------------------------------------------------
    Validate inputs
    ------------------------------------------------------*/
    assert( page_frame < MAX_PAGES );

    /*------------------------------------------------------
    Mark page frames as used
    ------------------------------------------------------*/
    for( int i = 0; i < cnt; i++ )
        {
        frame_alloc( page_frame + i );

        page_frames[ i ].flags |= PAGE_FRAME_FLAGS__RESERVED;
        }

    } /* page_frame_reserve() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       page_frame_alloc
*
*   DESCRIPTION:
*       Allocate a page frame
*
*********************************************************************/
struct page_frame * page_frame_alloc
    (
    void
    )
    {
    /*------------------------------------------------------
    Local Variables
    ------------------------------------------------------*/
    uint32_t frame_num;
    uint32_t idx = 0;
    uint32_t bit = 0;
    uint32_t i, j;
    struct page_frame * frame = NULL;

    /*------------------------------------------------------
    Get next free index and bit number
    ------------------------------------------------------*/
    // idx = BA_BYTE( used_pages, last_alloc_frame );
    // bit = BA_BIT( used_pages, last_alloc_frame );
    
    // bit += 1;
    // if( bit >= BA_WIDTH( used_pages ) )
    //     {
    //     bit = 0;
    //     idx++;
    //     }

    /*------------------------------------------------------
    Search for a free page, starting at last alloc'd page
    frame.
    ------------------------------------------------------*/
    for( i = idx; i < CNT_OF_ARRAY( used_pages ); i++ )
        {
        /*--------------------------------------------------
        No free pages at this index. I.e. all bits are set.
        --------------------------------------------------*/
        // TODO: Consider using cleared bit to represent used
        // page frame. Then we could check for zero here.
        if( used_pages[ i ] == BA_MASK( used_pages ) )
            {
            continue;
            }

        /*--------------------------------------------------
        Search each bit for a free page
        --------------------------------------------------*/
        for( j = bit; j < BA_WIDTH( used_pages ); j++ )
            {
            if( !( used_pages[ i ] & ( 1 << j ) ) )
                {
                break;
                }
            }

        /*--------------------------------------------------
        A free page was found
        --------------------------------------------------*/
        if( BA_WIDTH( used_pages ) != j )
            {
            break;
            }
        }

    /*------------------------------------------------------
    Out of pages
    ------------------------------------------------------*/
    if( ( i >= CNT_OF_ARRAY( used_pages ) )
     || ( j >= BA_WIDTH( used_pages )     ) )
        {
        assert( false );
        }

    /*------------------------------------------------------
    Get page frame
    ------------------------------------------------------*/
    frame = &page_frames[ ( i * BA_WIDTH( used_pages ) ) + j ];

    frame->flags |= PAGE_FRAME_FLAGS__ALLOCATED;

    /*------------------------------------------------------
    Return page frame
    ------------------------------------------------------*/
    return frame;

    } /* page_frame_alloc() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       page_frame_free
*
*   DESCRIPTION:
*       Mark page frame as free
*
*********************************************************************/
void page_frame_free
    (
    uint32_t            page_frame
    )
    {
    /*------------------------------------------------------
    Validate inputs
    ------------------------------------------------------*/
    assert( page_frame < MAX_PAGES );

    frame_free( page_frame );

    } /* page_frame_free() */