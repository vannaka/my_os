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
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/types.h>

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

// TODO: Move arch to header
#define PAGE_SHIFT 12

#define MAX_PAGES ( ( UINTPTR_MAX + 1 ) >> PAGE_SHIFT )

#define BIT_ARR_WIDTH   sizeof(uint32_t)

#define BIT_ARR_MASK    ( ( 1 << BIT_ARR_WIDTH ) - 1 )

/*--------------------------------------------------------------------
                                TYPES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                                MACROS
--------------------------------------------------------------------*/

#define page_idx( x ) ( ( x ) / BIT_ARR_WIDTH )
#define page_bit( x ) ( ( x ) % BIT_ARR_WIDTH )

#define frame_alloc( x ) used_pages[ page_idx( x ) ] |= (uint32_t)page_bit( x )
#define frame_free( x ) used_pages[ page_idx( x ) ] &= ~(uint32_t)page_bit( x )

/*--------------------------------------------------------------------
                           MEMORY CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                              VARIABLES
--------------------------------------------------------------------*/

static uint32_t used_pages[ MAX_PAGES / BIT_ARR_WIDTH ];

static uint32_t last_alloc_frame;

/*--------------------------------------------------------------------
                              PROCEDURES
--------------------------------------------------------------------*/


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
    void
    )
    {
    /*------------------------------------------------------
    Init module variables
    ------------------------------------------------------*/
    memset( used_pages, 0x0, sizeof( used_pages ) );
    last_alloc_frame = 0;

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
*   TODO: Return page frame number or physical address?
*
*********************************************************************/
void * page_frame_alloc
    (
    void
    )
    {
    /*------------------------------------------------------
    Local Variables
    ------------------------------------------------------*/
    uint32_t page_frame;
    uint32_t idx = 0;
    uint32_t bit = 0;
    uint32_t i, j;

    /*------------------------------------------------------
    Get next free index and bit number
    ------------------------------------------------------*/
    idx = page_idx( last_alloc_frame );
    bit = page_bit( last_alloc_frame );
    
    bit += 1;
    if( bit >= BIT_ARR_WIDTH )
        {
        bit = 0;
        idx++;
        }

    /*------------------------------------------------------
    Search for a free page, starting at last alloc'd page
    frame.
    ------------------------------------------------------*/
    for( i = idx; i < page_idx( MAX_PAGES ); i++ )
        {
        /*--------------------------------------------------
        No free pages at this index
        --------------------------------------------------*/
        if( used_pages[ i ] & BIT_ARR_MASK )
            {
            continue;
            }

        /*--------------------------------------------------
        Search each bit for a free page
        --------------------------------------------------*/
        for( j = bit; j < BIT_ARR_WIDTH; j++ )
            {
            if( !( used_pages[ i ] & ( 1 << j ) ) )
                {
                break;
                }
            }

        /*--------------------------------------------------
        A free page was found
        --------------------------------------------------*/
        if( BIT_ARR_WIDTH != j )
            {
            break;
            }

        }

    /*------------------------------------------------------
    Out of pages
    ------------------------------------------------------*/
    if( ( i >= page_idx( MAX_PAGES ) )
     || ( j >= BIT_ARR_WIDTH         ) )
        {
        assert( false );
        }

    /*------------------------------------------------------
    Get page frame number
    ------------------------------------------------------*/
    idx = i;
    bit = j;
    page_frame = ( idx * BIT_ARR_WIDTH ) + bit;

    /*------------------------------------------------------
    Return physical address of allocated page frame
    ------------------------------------------------------*/
    return page_frame * ( 1 << PAGE_SHIFT );

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