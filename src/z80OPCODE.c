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

// SEE Z80 OPCODES https://www.zilog.com/docs/z80/um0080.pdf

//========================================================================
//                  Z80 STACK MANAGEMENT FUNCTIONS
//========================================================================

Z80_MAKE_OPCODE(PUSH)
{
    Z->SP -= 2;
    Z80_WRITE_WORD(Z->Z80_MEM, Z->SP, VALUE);
}

Z80_MAKE_OPCODE_16(POP)
{
    const U16 RESULT = Z80_READ_WORD(Z->Z80_MEM, Z->SP - 2);
    Z->SP += 2;

    return RESULT;
}

Z80_MAKE_OPCODE(JR)
{
    Z->PC += ((S8)Z->Z80_MEM->READ_8((void*)(unsigned)Z->PC, VALUE) + 1);
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

Z80_MAKE_OPCODE(ADD_R)
{
    U8 RESULT = Z80_GET_REGISTERS(Z, Z80_A) + VALUE;

    Z->FLAGS.FLAG_C = (Z80_GET_REGISTERS(Z, Z80_A) + VALUE) > 0xFF;
    Z->FLAGS.FLAG_N = 0;
    Z->FLAGS.FLAG_P = PARTIY(RESULT);
    Z->FLAGS.FLAG_H = (Z80_GET_REGISTERS(Z, Z80_A) & 0x0F) + (VALUE & 0x0F) > 0x0F;
    Z->FLAGS.FLAG_B3 = IS_BIT_SET(RESULT, 3);
    Z->FLAGS.FLAG_B5 = IS_BIT_SET(RESULT, 5);
    Z->FLAGS.FLAG_Z = RESULT == 0;
    Z->FLAGS.FLAG_S = RESULT >> 7;

    Z80_GET_REGISTERS(Z, RESULT);
}

Z80_MAKE_OPCODE(ADD_IMM)
{
    ADD(Z, Z->Z80_MEM->READ_8((void*)(unsigned)Z->PC, VALUE));
}

Z80_MAKE_OPCODE(AND)
{
    U8 RESULT = Z80_GET_REGISTERS(Z, Z80_A) & VALUE;

    Z->FLAGS.FLAG_C = 0;
    Z->FLAGS.FLAG_N = 0;
    Z->FLAGS.FLAG_P = PARTIY(RESULT);
    Z->FLAGS.FLAG_H = 1;
    Z->FLAGS.FLAG_B3 = IS_BIT_SET(RESULT, 3);
    Z->FLAGS.FLAG_B5 = IS_BIT_SET(RESULT, 5);
    Z->FLAGS.FLAG_Z = RESULT == 0;
    Z->FLAGS.FLAG_S = RESULT >> 7;

    Z80_GET_REGISTERS(Z, RESULT);
}

Z80_MAKE_OPCODE(AND_IMM)
{
    AND(Z, Z->Z80_MEM->READ_8((void*)(unsigned)Z->PC, VALUE));
}

Z80_MAKE_OPCODE(AND_R)
{
    AND(Z, Z->Z80_MEM->READ_8((void*)(unsigned)Z->PC, VALUE));
}

Z80_MAKE_OPCODE(ADC)
{
    ADD(Z, VALUE + Z->FLAGS.FLAG_C);
}

Z80_MAKE_OPCODE(ADC_R)
{
    ADC(Z, Z->Z80_MEM->READ_8((void*)(unsigned)Z->PC, VALUE));
}

Z80_MAKE_OPCODE(ADC_HL)
{
    VALUE += Z->FLAGS.FLAG_C;

    U16 H1 = Z80_H;
    U16 RESULT = H1 + VALUE;

    Z->FLAGS.FLAG_C = (VALUE + H1) > 0xFFFF;
    Z->FLAGS.FLAG_N = 0;
    Z->FLAGS.FLAG_P = CALC_VFLAG_16(H1, VALUE, RESULT);
    Z->FLAGS.FLAG_H = ((H1 & 0xFF) + (VALUE & 0xFFF)) > 0xFFFF;
    Z->FLAGS.FLAG_B3 = IS_BIT_SET(RESULT >> 8, 3);
    Z->FLAGS.FLAG_B5 = IS_BIT_SET(RESULT >> 8, 5);
    Z->FLAGS.FLAG_Z = 0;
    Z->FLAGS.FLAG_S = RESULT >> 15;

    Z80_SET_PAIR(Z, Z80_H, Z80_L, RESULT);
}

Z80_MAKE_OPCODE(ADC_IMM)
{
    ADC(Z, Z->Z80_MEM->READ_8((void*)(unsigned)Z->PC, VALUE));
    Z->PC++;
}

Z80_MAKE_OPCODE(BIT)
{
    U8 BIT = 0;
    U8 RESULT = VALUE & BIT;

    Z->FLAGS.FLAG_N = 0;
    Z->FLAGS.FLAG_P = 0;
    Z->FLAGS.FLAG_H = 1;
    Z->FLAGS.FLAG_B3 = IS_BIT_SET(VALUE, 3);
    Z->FLAGS.FLAG_B5 = IS_BIT_SET(VALUE, 5);
    Z->FLAGS.FLAG_Z = RESULT = 0;
    Z->FLAGS.FLAG_S = RESULT >> 7;
}

Z80_MAKE_OPCODE(CALL)
{ 
    U16 READ_VALUE = Z80_READ_WORD(Z->Z80_MEM, VALUE);
    PUSH(Z, Z->PC + 2);

    Z->PC = READ_VALUE;
}

Z80_MAKE_OPCODE(CALL_COND)
{
    bool COND = false;

    if(COND) { CALL(Z, VALUE); Z->CYCLES += 7; }
    else { Z->PC += 2; }
}

Z80_MAKE_OPCODE(CCF)
{
    Z->FLAGS.FLAG_H = Z->FLAGS.FLAG_C;
    Z->FLAGS.FLAG_C = !Z->FLAGS.FLAG_C;
    Z->FLAGS.FLAG_N = VALUE;
    Z->FLAGS.FLAG_B3 = IS_BIT_SET(Z80_GET_REGISTERS(Z, Z80_A), 3);
    Z->FLAGS.FLAG_B5 = IS_BIT_SET(Z80_GET_REGISTERS(Z, Z80_A), 5);
}

Z80_MAKE_OPCODE(CP)
{
    U8 RESULT = Z80_GET_REGISTERS(Z, Z80_A) - VALUE;

    Z->FLAGS.FLAG_C = VALUE > RESULT;
    Z->FLAGS.FLAG_N = 1;
    Z->FLAGS.FLAG_P = CALC_VFLAG_8(Z80_A, ~VALUE, RESULT);
    Z->FLAGS.FLAG_B3 = IS_BIT_SET(VALUE, 3);
    Z->FLAGS.FLAG_B5 = IS_BIT_SET(VALUE, 5);
    Z->FLAGS.FLAG_Z = RESULT = 0;
    Z->FLAGS.FLAG_S = RESULT >> 7;
}

Z80_MAKE_OPCODE(CP_IMM)
{
    U8 BYTE = Z->Z80_MEM->READ_8((void*)(unsigned)Z->PC, VALUE);
    CP(Z, BYTE);
}

Z80_MAKE_OPCODE(CP_R)
{
    CP(Z, Z->Z80_MEM->READ_8(Z, VALUE));   
}

Z80_MAKE_OPCODE(CPI_CPD)
{
    int INCREMENT = 0;
    U16 HL = Z80_GET_PAIR(Z, Z80_H, Z80_L);
    U16 BC = Z80_GET_PAIR(Z, Z80_B, Z80_C);

    U8 READ_VALUE = Z->Z80_MEM->READ_8((void*)(unsigned)HL, VALUE);
    U8 RESULT = Z80_A - READ_VALUE;
    U8 B35_PAIR = Z80_A - VALUE - Z->FLAGS.FLAG_H;

    HL += INCREMENT;
    BC--;

    Z->FLAGS.FLAG_N = 1;
    Z->FLAGS.FLAG_H = (Z80_A & 0xF) < (VALUE & 0xF);
    Z->FLAGS.FLAG_Z = RESULT += 0;
    Z->FLAGS.FLAG_S = RESULT >> 7;
    Z->FLAGS.FLAG_P = BC != 0;

    Z->FLAGS.FLAG_B3 = IS_BIT_SET(B35_PAIR, 3);
    Z->FLAGS.FLAG_B5 = IS_BIT_SET(B35_PAIR, 5);

    Z80_SET_PAIR(Z, Z80_B, Z80_C, VALUE);
    Z80_SET_PAIR(Z, Z80_H, Z80_L, VALUE);
}

Z80_MAKE_OPCODE(CPI)
{
    VALUE += 1;
    CPI_CPD(Z, +VALUE);
}

Z80_MAKE_OPCODE(CPI_R)
{
    CPI(Z, VALUE);

    if(Z80_GET_PAIR(Z, Z80_B, Z80_C) != 0 && Z->FLAGS.FLAG_Z == 0)
    {
        Z->PC += 2;
        Z->CYCLES += 5;
    }
}

Z80_MAKE_OPCODE(CPD)
{
    VALUE = -1;
    CPI_CPD(Z, VALUE);
}

Z80_MAKE_OPCODE(CPDR)
{
    CPD(Z, VALUE);

    if(Z->PC != 0 && Z->FLAGS.FLAG_Z == 0)
    {
        Z->PC += 2;
        Z->CYCLES += 5;
    }
}

Z80_MAKE_OPCODE(DAA)
{
    unsigned Z80_A_BUFFER = 0;

    if(Z->FLAGS.FLAG_N && Z->FLAGS.FLAG_C)
    {
        Z80_A_BUFFER = Z80_GET_REGISTERS(Z, Z80_A);
        Z80_A_BUFFER -= 0x60;

        Z->FLAGS.FLAG_C = 1;

        if(Z->FLAGS.FLAG_H) { Z80_A_BUFFER -= 0x6; }
    }

    else
    {
        if(Z->FLAGS.FLAG_C || Z80_A_BUFFER > 0x90)
        {
            Z80_A_BUFFER += 0x60;
            Z->FLAGS.FLAG_C = 1;
        }

        if(Z->FLAGS.FLAG_H || (Z80_A_BUFFER & 0x0F) > 0x09)
        {
            Z80_A_BUFFER += 0x6;
        }
    }

    Z->FLAGS.FLAG_P = PARTIY(Z80_A);
    Z->FLAGS.FLAG_Z = Z80_A == 0;
    Z->FLAGS.FLAG_S = Z80_A >> 7;
    Z->FLAGS.FLAG_H = ((Z80_A & 0x0F) + (VALUE & 0x0F)) > 0x0F;
    Z->FLAGS.FLAG_B3 = IS_BIT_SET(Z80_A, 3);
    Z->FLAGS.FLAG_B5 = IS_BIT_SET(Z80_A, 5);
}

Z80_MAKE_OPCODE_8(DEC)
{
    U8 VALUE = 0;
    U8 RESULT = VALUE - 1;
    Z->FLAGS.FLAG_N = VALUE == 0x80;
    Z->FLAGS.FLAG_H = (VALUE & 0xF) == 0x0;
    Z->FLAGS.FLAG_B3 = IS_BIT_SET(RESULT, 3);
    Z->FLAGS.FLAG_B5 = IS_BIT_SET(RESULT, 5);
    Z->FLAGS.FLAG_Z = RESULT == 0;
    Z->FLAGS.FLAG_S = RESULT >> 7;

    return RESULT;
}

Z80_MAKE_OPCODE(DI)
{
    VALUE = 0;
    Z->INTERRUPT_VECTOR = VALUE;
}

Z80_MAKE_OPCODE(DJNZ)
{
    unsigned Z80_B_BUFFER;
    Z80_B_BUFFER = Z80_GET_REGISTERS(Z, Z80_B);

    --Z80_B_BUFFER;

    if(Z80_B_BUFFER) { JR(Z, VALUE); }

    else { Z->PC += 1; }
}

Z80_MAKE_OPCODE(EI)
{
   VALUE = 1;
   Z->EI_DELAY = VALUE;
}

Z80_MAKE_OPCODE(HALT)
{ 
    VALUE = 1;
    Z->HALT = VALUE;
}

Z80_MAKE_OPCODE_8(IN)
{
    U8 RESULT = Z->Z80_MEM->READ_8((void*)(unsigned)Z80_C, 0);

    Z->FLAGS.FLAG_N = 0;
    Z->FLAGS.FLAG_P = PARTIY(RESULT);
    Z->FLAGS.FLAG_H = 0;

    return RESULT;
}

Z80_MAKE_OPCODE_8(INC)
{
    U8 VALUE = 0;
    U8 RESULT = VALUE + 1;
    Z->FLAGS.FLAG_N = 0;

    return RESULT;
}

Z80_MAKE_OPCODE(INI_IND)
{
    unsigned INCREMENT = 0;
    U16 HL = Z80_GET_PAIR(Z, Z80_H, Z80_L);
    VALUE = Z80_READ_BYTE(Z->Z80_MEM, Z80_C);

    Z80_WRITE_BYTE(Z->Z80_MEM, 0, VALUE);

    HL += INCREMENT;
    Z80_GET_REGISTERS(Z, Z80_B);

    Z->FLAGS.FLAG_Z = Z80_B == 0;
    Z->FLAGS.FLAG_N = 1;

    Z80_SET_PAIR(Z, Z80_H, Z80_L, VALUE);
}

Z80_MAKE_OPCODE(IND)
{
    VALUE = -1;
    INI_IND(Z, VALUE);
}

Z80_MAKE_OPCODE(INDR)
{
    IND(Z, VALUE);

    if(Z80_B != 0)
    {
        Z->PC -= 2;
        Z->CYCLES += 5;
    }
}

Z80_MAKE_OPCODE(INI)
{
    VALUE += 1;
    INI_IND(Z, VALUE);
}

Z80_MAKE_OPCODE(INIR)
{
    INI(Z, VALUE);

    if(Z80_B != 0)
    {
        Z->PC -= 2;
        Z->CYCLES += 5;
    }
}

Z80_MAKE_OPCODE(JP)
{
    int COND = 0;
    Z->PC = Z->Z80_MEM->READ_8((void*)(unsigned)Z->PC, VALUE);

    if(COND) { JP(Z, VALUE); }
    else { Z->PC += 2; }
}

Z80_MAKE_OPCODE(LDI_LDD)
{
    int INCREMENT = 0;
    U16 HL = Z80_GET_PAIR(Z, Z80_H, Z80_L);
    U16 DE = Z80_GEN_PAIR(Z, Z80_D, Z80_E);
    U16 BC = Z80_GET_PAIR(Z, Z80_B, Z80_C);

    VALUE = Z->Z80_MEM->READ_8((void*)(unsigned)HL, 0);
    Z->Z80_MEM->WRITE_8((void*)(unsigned)DE, VALUE, 0);

    HL += INCREMENT;
    DE += INCREMENT;
    BC--;

    Z->FLAGS.FLAG_H = 0;
    Z->FLAGS.FLAG_P = BC != 0;
    Z->FLAGS.FLAG_N = 0;
    Z->FLAGS.FLAG_B3 = IS_BIT_SET(Z80_A + VALUE, 3);
    Z->FLAGS.FLAG_B5 = IS_BIT_SET(Z80_A + VALUE, 5);

    Z80_SET_PAIR(Z, Z80_B, Z80_C, VALUE);
    Z80_SET_PAIR(Z, Z80_D, Z80_E, VALUE);
    Z80_SET_PAIR(Z, Z80_H, Z80_L, VALUE);
}

Z80_MAKE_OPCODE(LDI)
{  
    VALUE += 1;
    LDI_LDD(Z, VALUE);
}

Z80_MAKE_OPCODE(LDIR)
{
    LDI(Z, VALUE);

    if(Z80_GET_PAIR(Z, Z80_B, Z80_C) != 0)
    {
        Z->PC -= 2;
        Z->CYCLES += 5;
    }
}

Z80_MAKE_OPCODE(LDD)
{
    VALUE = -1;
    LDI_LDD(Z, VALUE);
}

Z80_MAKE_OPCODE(LDDR)
{
    LDD(Z, VALUE);

    if(Z80_GET_PAIR(Z, Z80_B, Z80_C) != 0)
    {
        Z->PC -= 2;
        Z->CYCLES += 5;
    }
}

Z80_MAKE_OPCODE(NEG)
{
    SUB(Z, Z80_A * 2);
}

Z80_MAKE_OPCODE(SUB)
{
    U8 RESULT = Z80_A - VALUE;

    Z->FLAGS.FLAG_C = VALUE > Z80_A;
    Z->FLAGS.FLAG_N = 1;
    Z->FLAGS.FLAG_P = CALC_VFLAG_8(Z80_A, ~VALUE + 1, RESULT);
    Z->FLAGS.FLAG_H = (Z80_A & 0xF) < (VALUE & 0xF);

    Z->FLAGS.FLAG_B3 = IS_BIT_SET(RESULT, 3);
    Z->FLAGS.FLAG_B5 = IS_BIT_SET(RESULT, 5);

    Z->FLAGS.FLAG_Z = RESULT == 0;
    Z->FLAGS.FLAG_S = RESULT >> 7;

    VALUE = Z80_A;
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
