// COPYRIGHT (C) HARRY CLARK 2025
// LIGHTWEIGHT, VERSATILE Z80 EMULATOR

// THIS FILE PERTAINS TOWARDS THE MAIN FUNCTIONALITY ENCOMPASSING CONFIG
// BECAUSE THE COMPILER IS ASS

#ifndef Z80_CONF
#define Z80_CONF

// NESTED INCLUDES

#include "z80.h"

/*===============================================================================*/
/*-------------------------------------------------------------------------------*/
//                      Z80 EXTERNAL READ AND WRITE FUNCTIONS
/*-------------------------------------------------------------------------------*/
/*===============================================================================*/

static Z80_MEMORY* Z;

U8 Z80_READ_BYTE(U16 ADDR)
{
    return Z->READ_8(Z->USER_DATA, ADDR);
}

U16 Z80_READ_WORD(U16 ADDR)
{
    return (Z->READ_8(Z->USER_DATA, ADDR + 1) << 8) | Z->READ_8(Z->USER_DATA, ADDR);
}

void Z80_WRITE_BYTE(U16 ADDR, U8 VAL)
{
    Z->WRITE_8(Z->USER_DATA, ADDR, VAL);
}

void Z80_WRITE_WORD(U16 ADDR, U16 VAL)
{
    Z->WRITE_8(Z->USER_DATA, ADDR, VAL & 0xFF);
    Z->WRITE_8(Z->USER_DATA, ADDR + 1, (VAL >> 8) & 0xFF);
}

U8 PARITY(U8 VALUE)
{
    U8 BIT_COUNT = 0;

    for (int INDEX = 0; INDEX < 8; INDEX++)
    {
        BIT_COUNT += ((VALUE >> INDEX) & 1);
    }

    return BIT_COUNT % 2;
}


#endif
