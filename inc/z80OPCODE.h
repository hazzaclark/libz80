// COPYRIGHT (C) HARRY CLARK 2025
// LIGHTWEIGHT, VERSATILE Z80 EMULATOR

// THIS FILE PERTAINS TOWARDS THE MAIN FUNCTIONALITY ENCOMPASSING THE OPCODES

#ifndef Z80_OPCODE
#define Z80_OPCODE

// NESTED INCLUDES

#include "z80.h"
#include "common.h"

/* SYSTEM INCLUDES */

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#if defined(BUILD_OP_TABLE)
    #define BUILD_OP_TABLE
    #else
    #define BUILD_OP_TABLE

    #define     OPCODE_BIT_MASK         0xFF
    #define     OPCODE_MAX              0x100

    // OPCODE DECODING MACROS - USED FOR BIT SHIFTS
    // Y = 5-3, Z = 2-0, P = 5-4, Q = 4-3   

    #define     OPCODE_Y(VALUE)         (((OPCODE) >> 3) & 0x07)
    #define     OPCODE_Z(VALUE)         ((OPCODE) & 0x07)
    #define     OPCODE_P(VALUE)         (((OPCODE) >> 4) & 0x03)
    #define     OPCODE_Q(VALUE)         (((OPCODE) >> 3) & 0x03)

    #define     OPCODE_LD_A_I           0x57
    #define     OPCODE_LD_I_A           0x47

    #define     OPCODE_LDI              0xa0    
    #define     OPCODE_LDIR             0xb0
    #define     OPCODE_CPI              0xa1
    #define     OPCODE_CPIR             0xb1

    #define     OPCODE_RLD              0x6f


    U8 Z80_GET_OPCODE_CYCLES(U8, U8);

#endif
#endif
