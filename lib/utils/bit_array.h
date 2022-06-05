/*********************************************************************
*
*   MODULE:
*       bit_array.h
*
*   DESCRIPTION:
*       Bit Array manipulation
*
*********************************************************************/

#ifndef _BIT_ARRAY_H
#define _BIT_ARRAY_H

/*--------------------------------------------------------------------
                               INCLUDES
--------------------------------------------------------------------*/

#include <limits.h>

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

#define BA_WIDTH( ba )      ( sizeof( (ba)[0] ) * CHAR_BIT )
#define BA_MASK( ba )       ( ( 1ULL << BA_WIDTH( ba ) ) - 1UL )
#define BA_BYTE( ba, i )    ( ( i ) / BA_WIDTH( ba ) )
#define BA_BIT( ba, i )     ( ( i ) % BA_WIDTH( ba ) )

#define BA_CREATE64(name, cnt) uint64_t name[ ( cnt / ( sizeof(uint64_t) * CHAR_BIT ) ) ]
#define BA_CREATE32(name, cnt) uint32_t name[ ( cnt / ( sizeof(uint32_t) * CHAR_BIT ) ) ]
#define BA_CREATE16(name, cnt) uint16_t name[ ( cnt / ( sizeof(uint16_t) * CHAR_BIT ) ) ]
#define BA_CREATE8(name, cnt) uint8_t name[ ( cnt / ( sizeof(uint8_t) * CHAR_BIT ) ) ]

#define BA_SET( ba, i ) ( ba[ BA_BYTE( ba, i ) ] |= ( 1 << BA_BIT( ba, i ) ) )
#define BA_CLR( ba, i ) ( ba[ BA_BYTE( ba, i ) ] &= ~( 1 << BA_BIT( ba, i ) ) )

/*--------------------------------------------------------------------
                              PROCEDURES
--------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif /* _BIT_ARRAY_H */
