// COPYRIGHT (C) HARRY CLARK 2025
// LIGHTWEIGHT, VERSATILE Z80 EMULATOR

// THIS FILE PERTAINS TOWARDS THE MAIN FUNCTIONALITY ENCOMPASSING THE OPCODES

// NESTED INCLUDES

#include "z80OPCODE.h"

#ifdef BUILD_OP_TABLE

#define         Z80_OPCODE_MASK     0xC0
#define         Z80_OPCODE_PATTERN  0x40


// THE FOLLOWING IMPLEMENTS A SIMILAR, IF NOT, THE EXACT SAME SCHEMA DEFINED IN LIB68K
// WHEREBY, A MACRO ENCOMPASSES THE CREATION OF OPCODES ACROSS ALL FILES

// SUCH THAT IT IS MODULAR ENOUGH TO BE INCLUDED ELSEWHERE WITHOUT THE NEED FOR REIMPLEMENTATION

// SEE z80OCODE.h FOR IMPLEMENTATION

//========================================================================
//                  Z80 STACK MANAGEMENT FUNCTIONS
//========================================================================

Z80_MAKE_OPCODE(PUSHW)
{
    Z->SP -= 2;
    Z80_WRITE_WORD(Z->Z80_MEM, Z->SP, VALUE);
}

Z80_MAKE_OPCODE_16(POPW)
{
    const U16 RESULT = Z80_READ_WORD(Z->Z80_MEM, Z->SP - 2);
    Z->SP += 2;

    return RESULT;
}

Z80_MAKE_OPCODE_8(NEXTB)
{
    return Z80_READ_BYTE(Z->Z80_MEM, Z->PC++);
}

Z80_MAKE_OPCODE_16(NEXTW)
{
    return Z80_READ_WORD(Z->Z80_MEM, Z->PC - 2);
}

Z80_MAKE_OPCODE(ADD)
{
    U8 RESULT = Z80_A + VALUE;

    Z->FLAGS.FLAG_C = (Z80_A + VALUE) > 0xFF;
    Z->FLAGS.FLAG_N = 0;
    Z->FLAGS.FLAG_P = CALC_VFLAG_8(Z80_A, VALUE, RESULT);
    Z->FLAGS.FLAG_B3 = IS_BIT_SET(RESULT, 3);
    Z->FLAGS.FLAG_B5 = IS_BIT_SET(RESULT, 5);
    Z->FLAGS.FLAG_Z = RESULT == 0;
    Z->FLAGS.FLAG_S = RESULT >> 7;

    RESULT = (Z80_GET_REGISTERS(Z, Z80_A) << 8);
}

// A PROFICIENT WAY OF BEING ABLE TO ACCES THE OPCODE MASK TTPES
// BASED ON THE MAJORITY OF INSTRUCTIONS, IMM LOADS AND NOP/ILLEGAL

U8 Z80_GET_OPCODE_CYCLES(U8 OPCODE)
{
    U8 PREFIX = 0;

    switch (PREFIX)
    {
        case 0xED:
            return ((OPCODE & Z80_OPCODE_MASK) == Z80_OPCODE_PATTERN) ? 0x0C : 0x08;

        case 0xDD:
        case 0xFD:
            return ((OPCODE & Z80_OPCODE_MASK) == Z80_OPCODE_PATTERN) ? 0x13 : 0x04;
    
        default:
            return (OPCODE & Z80_OPCODE_MASK) ? 0x06 : ((OPCODE & 0xC7) == 0x06) ? 0x07 : 0x04;   
    }
}

void Z80_EXEC(CPU_Z80* const Z80, U8 OPCODE)
{
    Z80->CYCLES += Z80_GET_OPCODE_CYCLES(OPCODE);
}

#endif
