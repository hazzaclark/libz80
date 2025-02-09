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

#define         Z80_READ_BYTE(VALUE, ADDRESS)               (VALUE)[(ADDRESS) ^ 1]
#define         Z80_READ_WORD(VALUE, ADDRESS)               (((VALUE)[ADDRESS]<<8) | (VALUE)[(ADDRESS)+1])

#define         Z80_WRITE_BYTE(VALUE, ADDRESS, DATA)        (VALUE)[(ADDRESS) ^ 1] = (DATA)

#define         Z80_WRITE_WORD(MEM, ADDRESS, DATA) do { \
                (MEM)[(ADDRESS)] = ((DATA) >> 8) & 0xFF; \
                (MEM)[(ADDRESS) + 1] = (DATA) & 0xFF; \
                } while(0)

#define         Z80_LOW_NIBBLE(VALUE)
#define         Z80_HIGH_NIBBLE(VALUE)                             ((VALUE >> 9) << 16)

#define         Z80_GET_BIT(NTH, VALUE)                     (((VALUE) >> (NTH)) & 1)

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
    U8* READ_8;
    U16* READ_16;
    U8* WRITE_8;
    U16* WRITE_16;

} Z80_MEMORY;

typedef struct CPU_Z80
{
    struct Z80_MEMORY* Z80_MEM[Z80_BASE_BITMASK];

    U16 CYCLES;
    U16 PC;
    U16 SP;

    U8 INTERRUPT_VECTOR;
    U8 MEMORY_REFRESH;

    U8 IRQ_MODE;

    unsigned HALT : 1;
    unsigned INT_PENDING : 1;
    unsigned NMI_PENDING : 1;

} CPU_Z80;


#define     Z80_MEMORY_BASE             CPU.Z80_MEM
#define     Z80_MASTER_CYCLES           CPU.CYCLES
#define     Z80_PC                      CPU.PC
#define     Z80_SP                      CPU.SP
#define     Z80_IRQ                     CPU.INTERRUPT_VECTOR
#define     Z80_IRQ_MODE                CPU.IRQ_MODE
#define     Z80_REF                     CPU.MEMORY_REFRESH
#define     Z80_HALT                    CPU.HALT
#define     Z80_INT_PEND                CPU.INT_PENDING
#define     Z80_NMI                     CPU.NMI_PENDING

static CPU_Z80 CPU;

#endif
