// COPYRIGHT (C) HARRY CLARK 2025
// LIGHTWEIGHT, VERSATILE Z80 EMULATOR

// THIS FILE PERTAINS TOWARDS THE MAIN FUNCTIONALITY ENCOMPASSING THE UTILITY
// OF THE CPU AND IT'S SUBSEQUENT LOGIC, FUNCTION AND NECESSITIES

// ANY AND ALL OF THE CORRESPONDING INFORMATION CAN BE ATTRIBUTED TO THE FOLLLOWING DOCUMENTATION:

// Z80 PROGRAMMER MANUAL: https://www.zilog.com/docs/z80/um0080.pdf

#ifndef Z80_BASE
#define Z80_BASE

/* NESTED INCLUDES */

#include "common.h"

/* SYSTEM INCLUDES */

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

#if defined(USE_Z80)
    #define USE_Z80
        #else 
        #define USE_Z80


        #if defined(USE_OPCODE)
        #define USE_OPCODE
            #else
        #define USE_OPCODE
        #include "z80OPCODE.h"
        #endif

#endif

/*===============================================================================*/
/*                          Z80 MAIN CPU FUNCTIONALIY							 */
/*===============================================================================*/


#define         Z80_DATA_WIDTH          0xF
#define         Z80_ADDRESS_WIDTH       0xFF
#define         Z80_BASE_BITMASK        256


/*===============================================================================*/
/*-------------------------------------------------------------------------------*/
/*                          Z80 MEMORY UTILITY MACROS                            */
/*                      FOR READING THE BYTE ORDER OF MEMORY                     */
/*-------------------------------------------------------------------------------*/
/*              THE FOLLOWING MACROS ARE DESIGNED FOR READING MEMORY             */
/*                      FROM A PREDEFINED MEMORY MAPPER                          */
/*-------------------------------------------------------------------------------*/
/*===============================================================================*/

#define         Z80_LOW_NIBBLE(VALUE)
#define         Z80_HIGH_NIBBLE(VALUE)                             ((VALUE >> 9) << 16)

#define         Z80_GET_BIT(NTH, VALUE)                            (((VALUE) >> (NTH)) & 1)

#define         Z80_B                   0
#define         Z80_C                   1
#define         Z80_D                   2
#define         Z80_E                   3
#define         Z80_H                   4
#define         Z80_L                   5
#define         Z80_A                   6
#define         Z80_F                   7
#define         Z80_IXH                 8
#define         Z80_IXL                 9
#define         Z80_IYH                 10  
#define         Z80_IYL                 11
#define         Z80_I                   12
#define         Z80_R                   13

#define         Z80_S_FLAG_SHIFT        7       
#define         Z80_Z_FLAG_SHIFT        6
#define         Z80_Y_FLAG_SHIFT        5
#define         Z80_H_FLAG_SHIFT        4
#define         Z80_X_FLAG_SHIFT        3
#define         Z80_PV_FLAG_SHIFT       2
#define         Z80_N_FLAG_SHIFT        1
#define         Z80_C_FLAG_SHIFT        0

#define         Z80_S_FLAG              (1 << Z80_S_FLAG_SHIFT)
#define         Z80_Z_FLAG              (1 << Z80_Z_FLAG_SHIFT)
#define         Z80_Y_FLAG              (1 << Z80_Y_FLAG_SHIFT)
#define         Z80_H_FLAG              (1 << Z80_H_FLAG_SHIFT)
#define         Z80_X_FLAG              (1 << Z80_X_FLAG_SHIFT)
#define         Z80_PV_FLAG             (1 << Z80_PV_FLAG_SHIFT)
#define         Z80_N_FLAG              (1 << Z80_N_FLAG_SHIFT)
#define         Z80_C_FLAG              (1 << Z80_C_FLAG_SHIFT)

#define         Z80_P_FLAG_SHIFT        Z80_PV_FLAG_SHIFT
#define         Z80_V_FLAG_SHIFT        Z80_PV_FLAG_SHIFT
#define         Z80_P_FLAG              Z80_PV_FLAG
#define         Z80_V_FLAG              Z80_PV_FLAG

typedef struct Z80_MEMORY
{
    unsigned(*MEMORY_BASE);
    U8(*READ_8)(void*, U16);
    U16* READ_16;
    void(*WRITE_8)(void*, U16, U8);
    void(*WRITE_16)(void*, U16, U8);

    void* USER_DATA;

} Z80_MEMORY;

typedef struct CPU_Z80
{
    struct Z80_MEMORY Z80_MEM[Z80_BASE_BITMASK];

    U16 CYCLES;
    U16 PC;
    U16 SP;

    U8 INTERRUPT_VECTOR;
    U8 MEMORY_REFRESH;

    U8 IRQ_MODE;

    unsigned HALT : 1;
    unsigned INT_PENDING : 1;
    unsigned NMI_PENDING : 1;

    U16 REGISTER_BASE[16];

} CPU_Z80;

/*===============================================================================*/
/*-------------------------------------------------------------------------------*/
//                          Z80 FUNCTION PROTOTYPES
/*-------------------------------------------------------------------------------*/
/*===============================================================================*/

void Z80_INIT(CPU_Z80* const Z80);
void Z80_OUTPUT(CPU_Z80* const Z80);

unsigned Z80_GET_REGISTERS(CPU_Z80* const Z80, int REGISTER);
void Z80_STEP(CPU_Z80* const Z80);
void Z80_GEN_NMI(CPU_Z80* const Z80);
void Z80_GEN_INT_DATA(CPU_Z80* const Z80);

/*===============================================================================*/
/*-------------------------------------------------------------------------------*/
//                      Z80 EXTERNAL READ AND WRITE FUNCTIONS
/*-------------------------------------------------------------------------------*/
/*===============================================================================*/

extern U8 Z80_READ_BYTE(Z80_MEMORY* const Z, U16 ADDR)
{
    return Z->READ_8(Z->USER_DATA, ADDR);
}

extern U16 Z80_READ_WORD(Z80_MEMORY* const Z, U16 ADDR)
{
    return (Z->READ_8(Z->USER_DATA, ADDR + 1) << 8) | Z->READ_8(Z->USER_DATA, ADDR);
}

extern void Z80_WRITE_BYTE(Z80_MEMORY* const Z, U16 ADDR, U8 VAL)
{
    Z->WRITE_8(Z->USER_DATA, ADDR, VAL);
}

extern void Z80_WRITE_WORD(Z80_MEMORY* const Z, U16 ADDR, U8 VAL)
{
    Z->WRITE_8(Z->USER_DATA, ADDR, VAL & 0xFF);
    Z->WRITE_8(Z->USER_DATA, ADDR + 1, VAL >> 8);
}

#endif
