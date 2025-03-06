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

Z80_MAKE_OPCODE(NOP)
{
    Z80_UNUSED(VALUE);
    Z->PC = 0;
}

Z80_MAKE_OPCODE(PUSH)
{
    WRITE_8(Z, Z->SP, VALUE >> 8 & 0xFF);
    WRITE_8(Z, Z->SP, VALUE & 0xFF);
}

Z80_MAKE_OPCODE_16(POP)
{
    const U16 RESULT = READ_16(Z, Z->SP);
    Z->SP += 2;

    return RESULT;
}

Z80_MAKE_OPCODE(JR)
{
    Z->PC = READ_8(Z, VALUE);
}

Z80_MAKE_OPCODE_8(NEXTB)
{
    return READ_8(Z, Z->PC++);
}

Z80_MAKE_OPCODE_16(NEXTW)
{
    return READ_16(Z, Z->PC);
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
    Z->FLAGS.FLAG_P = PARITY(RESULT);
    Z->FLAGS.FLAG_H = (Z80_GET_REGISTERS(Z, Z80_A) & 0x0F) + (VALUE & 0x0F) > 0x0F;
    Z->FLAGS.FLAG_B3 = IS_BIT_SET(RESULT, 3);
    Z->FLAGS.FLAG_B5 = IS_BIT_SET(RESULT, 5);
    Z->FLAGS.FLAG_Z = RESULT == 0;
    Z->FLAGS.FLAG_S = RESULT >> 7;

    Z80_GET_REGISTERS(Z, RESULT);
}

Z80_MAKE_OPCODE(ADD_IMM)
{
    ADD(Z, READ_8(Z, Z->PC + VALUE));
}

Z80_MAKE_OPCODE(AND)
{
    U8 RESULT = Z80_GET_REGISTERS(Z, Z80_A) & VALUE;

    Z->FLAGS.FLAG_C = 0;
    Z->FLAGS.FLAG_N = 0;
    Z->FLAGS.FLAG_P = PARITY(RESULT);
    Z->FLAGS.FLAG_H = 1;
    Z->FLAGS.FLAG_B3 = IS_BIT_SET(RESULT, 3);
    Z->FLAGS.FLAG_B5 = IS_BIT_SET(RESULT, 5);
    Z->FLAGS.FLAG_Z = RESULT == 0;
    Z->FLAGS.FLAG_S = RESULT >> 7;

    Z80_GET_REGISTERS(Z, RESULT);
}

Z80_MAKE_OPCODE(AND_IMM)
{
    AND(Z, READ_8(Z, Z->PC + VALUE));
}

Z80_MAKE_OPCODE(AND_R)
{
    AND(Z, READ_8(Z, Z->PC + VALUE));
}

Z80_MAKE_OPCODE(ADC)
{
    ADD(Z, VALUE + Z->FLAGS.FLAG_C);
}

Z80_MAKE_OPCODE(ADC_R)
{
    ADC(Z, READ_8(Z, Z->PC + VALUE));
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
    ADC(Z, READ_8(Z, Z->PC + VALUE));
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
    U16 READ_VALUE = READ_16(Z, VALUE);
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
    U8 BYTE = READ_8(Z, Z->PC + VALUE);
    CP(Z, BYTE);
}

Z80_MAKE_OPCODE(CP_R)
{
    CP(Z, READ_8(Z, Z->PC + VALUE));   
}

Z80_MAKE_OPCODE(CPI_CPD)
{
    int INCREMENT = 0;
    U16 HL = Z80_GET_PAIR(Z, Z80_H, Z80_L);
    U16 BC = Z80_GET_PAIR(Z, Z80_B, Z80_C);

    U8 READ_VALUE = READ_8(Z, HL);
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

    Z->FLAGS.FLAG_P = PARITY(Z80_A);
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
    U8 Z80_B_BUFFER = Z80_GET_REGISTERS(Z, Z80_B);

    --Z80_B_BUFFER;

    Z80_SET_REGISTERS(Z, Z80_B, Z80_B_BUFFER);

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
    Z80_UNUSED(VALUE);
    Z->HALT = 1;
}

Z80_MAKE_OPCODE_8(IN)
{
    U8 RESULT = READ_8(Z, Z80_C);

    Z->FLAGS.FLAG_N = 0;
    Z->FLAGS.FLAG_P = PARITY(RESULT);
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
    VALUE = READ_8(Z, Z80_C);

    WRITE_8(Z, 0, VALUE);

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
    Z->PC = READ_8(Z, Z->PC + VALUE);

    if(COND) { JP(Z, VALUE); }
    else { Z->PC += 2; }
}

Z80_MAKE_OPCODE(LDI_LDD)
{
    int INCREMENT = 0;
    U16 HL = Z80_GET_PAIR(Z, Z80_H, Z80_L);
    U16 DE = Z80_GET_PAIR(Z, Z80_D, Z80_E);
    U16 BC = Z80_GET_PAIR(Z, Z80_B, Z80_C);

    VALUE = READ_8(Z, HL);
    WRITE_8(Z, DE, VALUE + 0);

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

Z80_MAKE_OPCODE(LD_A_B)
{
    (void)VALUE;
    Z80_SET_REGISTERS(Z, Z80_A, Z80_GET_REGISTERS(Z, Z80_B));
}

Z80_MAKE_OPCODE(LD_A_C)
{
    (void)VALUE;
    Z80_SET_REGISTERS(Z, Z80_A, Z80_GET_REGISTERS(Z, Z80_C));
}

Z80_MAKE_OPCODE(LD_A_IMM)
{
    VALUE = READ_8(Z, Z->PC++);
    Z80_SET_REGISTERS(Z, Z80_A, VALUE);
}

Z80_MAKE_OPCODE(LD_B_IMM)
{
    VALUE = READ_8(Z, Z->PC++);
    Z80_SET_REGISTERS(Z, Z80_B, VALUE);
}

Z80_MAKE_OPCODE(LD_C_IMM)
{
    VALUE = READ_8(Z, Z->PC++);
    Z80_SET_REGISTERS(Z, Z80_C, VALUE);
}

Z80_MAKE_OPCODE(SUB)
{
    U8 RESULT = Z80_A - VALUE;

    Z->FLAGS.FLAG_C = VALUE > Z80_A;
    Z->FLAGS.FLAG_N = 1;
    Z->FLAGS.FLAG_P = CALC_VFLAG_8(Z80_A, ~VALUE, RESULT);
    Z->FLAGS.FLAG_H = (Z80_A & 0xF) < (VALUE & 0xF);

    Z->FLAGS.FLAG_B3 = IS_BIT_SET(RESULT, 3);
    Z->FLAGS.FLAG_B5 = IS_BIT_SET(RESULT, 5);

    Z->FLAGS.FLAG_Z = RESULT == 0;
    Z->FLAGS.FLAG_S = RESULT >> 7;

    VALUE = Z80_A;
}

Z80_MAKE_OPCODE(NEG)
{
    VALUE = 2;
    SUB(Z, Z80_A * VALUE);
}

Z80_MAKE_OPCODE(OR)
{
    U8 RESULT = Z80_A | VALUE;

    Z->FLAGS.FLAG_C = 0;
    Z->FLAGS.FLAG_N = 0;
    Z->FLAGS.FLAG_P = PARITY(RESULT);
    Z->FLAGS.FLAG_H = 0;
    Z->FLAGS.FLAG_B3 = IS_BIT_SET(RESULT, 3);
    Z->FLAGS.FLAG_B5 = IS_BIT_SET(RESULT, 5);
    Z->FLAGS.FLAG_Z = RESULT == 0;
    Z->FLAGS.FLAG_S = RESULT >> 7;

    RESULT = Z80_A;
}

Z80_MAKE_OPCODE(OUTI_OUTD)
{
    int INCREMENT = 0;
    U16 HL = Z80_GET_PAIR(Z, Z80_H, Z80_L);
    VALUE = READ_8(Z, HL);

    HL += INCREMENT;
    Z->FLAGS.FLAG_Z = Z80_B == 0;
    Z->FLAGS.FLAG_N = 1;

    Z80_SET_PAIR(Z, Z80_H, Z80_L, VALUE);
}

Z80_MAKE_OPCODE(OUTD)
{
    VALUE = -1;
    OUTI_OUTD(Z, VALUE);
}

Z80_MAKE_OPCODE(OUTI)
{
    VALUE = 1;
    OUTI_OUTD(Z, VALUE);
}

Z80_MAKE_OPCODE(OTIR)
{
    VALUE = 1;
    OUTI(Z, VALUE);

    if(Z80_B != 0)
    {
        Z->PC -= 2;
        Z->CYCLES += 5;
    }
}

Z80_MAKE_OPCODE(OTDR)
{
    OUTD(Z, VALUE);

    if(Z80_B != 0)
    {
        Z->PC -= 2;
        Z->CYCLES += 5;
    }
}

Z80_MAKE_OPCODE(OUT)
{
    WRITE_16(Z, Z80_C, VALUE);
}

Z80_MAKE_OPCODE_8(RES)
{
    Z80_UNUSED(Z);

    U8 const VALUE = 0;
    U8 const BIT = 0;
    U8 RESULT = VALUE & ~BIT;

    return RESULT;
}

Z80_MAKE_OPCODE(RET)
{
    Z80_UNUSED(VALUE);
    Z->PC = POP(Z);
}

Z80_MAKE_OPCODE(RET_CC)
{
    int CONDITION = 0;

    if(CONDITION)
    {
        RET(Z, VALUE);
        Z->CYCLES += 6;
    }
}

Z80_MAKE_OPCODE(RETI)
{
    Z80_UNUSED(VALUE);
    Z->PC = POP(Z);
}

Z80_MAKE_OPCODE(RETN)
{
    Z80_UNUSED(VALUE);
    Z->PC = POP(Z);
}

Z80_MAKE_OPCODE_8(RL)
{
    U8 VALUE = 0;
    U8 RESULT = (VALUE << 1);

    Z->FLAGS.FLAG_C = VALUE >> 7;
    Z->FLAGS.FLAG_N = 0;
    Z->FLAGS.FLAG_P = PARITY(RESULT);
    Z->FLAGS.FLAG_H = 0;
    Z->FLAGS.FLAG_B3 = IS_BIT_SET(RESULT, 3);
    Z->FLAGS.FLAG_B5 = IS_BIT_SET(RESULT, 5);
    Z->FLAGS.FLAG_Z = RESULT == 0;
    Z->FLAGS.FLAG_S = RESULT >> 7;

    return RESULT;  
}

Z80_MAKE_OPCODE_8(RLC)
{
    return RL(Z);
}

Z80_MAKE_OPCODE(SBC)
{
    SUB(Z, VALUE + Z->FLAGS.FLAG_C);
}

Z80_MAKE_OPCODE(SCF)
{
    Z80_UNUSED(VALUE);
    Z->FLAGS.FLAG_C = 1;
    Z->FLAGS.FLAG_H = 0;
    Z->FLAGS.FLAG_N = 0;
    Z->FLAGS.FLAG_B3 = IS_BIT_SET(Z80_A, 3);
    Z->FLAGS.FLAG_B5 = IS_BIT_SET(Z80_A, 5);
}

Z80_MAKE_OPCODE_8(SET)
{
    Z80_UNUSED(Z);

    U8 VALUE = 0;
    U8 BIT = 0;
    U8 RESULT = VALUE | BIT;

    return RESULT;
}

Z80_MAKE_OPCODE_8(SLA)
{
    Z80_UNUSED(Z);

    U8 VALUE = 0;
    U8 RESULT = VALUE << 1;

    return RESULT;
}

Z80_MAKE_OPCODE_8(SRA)
{
    Z80_UNUSED(Z);

    U8 VALUE = 0;
    U8 RESULT = VALUE >> 1;

    return RESULT;
}

Z80_MAKE_OPCODE(XOR)
{
    U8 RESULT = Z80_A ^ VALUE;

    Z->FLAGS.FLAG_C = 0;
    Z->FLAGS.FLAG_N = 0;
    Z->FLAGS.FLAG_P = PARITY(RESULT);
    Z->FLAGS.FLAG_H = 0;
    Z->FLAGS.FLAG_B3 = IS_BIT_SET(RESULT, 3);
    Z->FLAGS.FLAG_B5 = IS_BIT_SET(RESULT, 5);
    Z->FLAGS.FLAG_Z = RESULT == 0;
    Z->FLAGS.FLAG_S = RESULT >> 7;

    RESULT = Z80_A;
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

// BUILD THE ENCOMPASSING OPCODE TABLE
// THIS WILL REPLACE THE SWITCH CASE IN main.c

OPCODE_HANDLER BUILD_OPCODE_TABLE[] =
{
    { .FUNCTION_PTRS.HANDLER = NOP,         .MASK = 0x00, .TYPE = 0 },  // NOP
    { .FUNCTION_PTRS.HANDLER = PUSH,        .MASK = 0xC5, .TYPE = 0 },  // PUSH BC
    { .FUNCTION_PTRS.HANDLER_16 = POP,      .MASK = 0xC1, .TYPE = 2 },  // POP BC
    { .FUNCTION_PTRS.HANDLER = JR,          .MASK = 0x18, .TYPE = 0 },  // JR
    { .FUNCTION_PTRS.HANDLER = LD_A_B,      .MASK = 0x78, .TYPE = 0 },  // LD A, B
    { .FUNCTION_PTRS.HANDLER = LD_A_C,      .MASK = 0x79, .TYPE = 0 },  // LD A, C
    { .FUNCTION_PTRS.HANDLER = LD_A_IMM,    .MASK = 0x3E, .TYPE = 0 },  // LD A, IMM
    { .FUNCTION_PTRS.HANDLER_8 = INC,       .MASK = 0x04, .TYPE = 1 },  // INC B
    { .FUNCTION_PTRS.HANDLER_8 = DEC,       .MASK = 0x05, .TYPE = 1 },  // DEC B
    { .FUNCTION_PTRS.HANDLER_16 = NEXTW,    .MASK = 0x01, .TYPE = 2 },  // NEXTW
    { .FUNCTION_PTRS.HANDLER = ADD,         .MASK = 0x80, .TYPE = 0 },  // ADD A, B
    { .FUNCTION_PTRS.HANDLER = ADD_R,       .MASK = 0x81, .TYPE = 0 },  // ADD A, C
    { .FUNCTION_PTRS.HANDLER = ADD_IMM,     .MASK = 0xC6, .TYPE = 0 },  // ADD A, IMM
    { .FUNCTION_PTRS.HANDLER = AND,         .MASK = 0xA0, .TYPE = 0 },  // AND A, B
    { .FUNCTION_PTRS.HANDLER = AND_IMM,     .MASK = 0xE6, .TYPE = 0 },  // AND A, IMM
    { .FUNCTION_PTRS.HANDLER = AND_R,       .MASK = 0xA1, .TYPE = 0 },  // AND A, C
    { .FUNCTION_PTRS.HANDLER = ADC,         .MASK = 0x88, .TYPE = 0 },  // ADC A, B
    { .FUNCTION_PTRS.HANDLER = ADC_R,       .MASK = 0x89, .TYPE = 0 },  // ADC A, C
    { .FUNCTION_PTRS.HANDLER = ADC_HL,      .MASK = 0xED4A, .TYPE = 0 }, // ADC HL, BC
    { .FUNCTION_PTRS.HANDLER = ADC_IMM,     .MASK = 0xCE, .TYPE = 0 },  // ADC A, IMM
    { .FUNCTION_PTRS.HANDLER = BIT,         .MASK = 0x40, .TYPE = 0 },  // BIT 0, B
    { .FUNCTION_PTRS.HANDLER = CALL,        .MASK = 0xCD, .TYPE = 0 },  // CALL nn
    { .FUNCTION_PTRS.HANDLER = CALL_COND,   .MASK = 0xC4, .TYPE = 0 },  // CALL NZ, nn
    { .FUNCTION_PTRS.HANDLER = CCF,         .MASK = 0x3F, .TYPE = 0 },  // CCF
    { .FUNCTION_PTRS.HANDLER = CP,          .MASK = 0xB8, .TYPE = 0 },  // CP A, B
    { .FUNCTION_PTRS.HANDLER = CP_IMM,      .MASK = 0xFE, .TYPE = 0 },  // CP A, IMM
    { .FUNCTION_PTRS.HANDLER = CP_R,        .MASK = 0xB9, .TYPE = 0 },  // CP A, C
    { .FUNCTION_PTRS.HANDLER = CPI_CPD,     .MASK = 0xA1, .TYPE = 0 },  // CPI
    { .FUNCTION_PTRS.HANDLER = CPI,         .MASK = 0xA1, .TYPE = 0 },  // CPI
    { .FUNCTION_PTRS.HANDLER = CPI_R,       .MASK = 0xA1, .TYPE = 0 },  // CPI_R
    { .FUNCTION_PTRS.HANDLER = CPD,         .MASK = 0xA9, .TYPE = 0 },  // CPD
    { .FUNCTION_PTRS.HANDLER = CPDR,        .MASK = 0xB9, .TYPE = 0 },  // CPDR
    { .FUNCTION_PTRS.HANDLER = DAA,         .MASK = 0x27, .TYPE = 0 },  // DAA
    { .FUNCTION_PTRS.HANDLER_8 = DEC,       .MASK = 0x05, .TYPE = 1 },  // DEC B
    { .FUNCTION_PTRS.HANDLER = DI,          .MASK = 0xF3, .TYPE = 0 },  // DI
    { .FUNCTION_PTRS.HANDLER = DJNZ,        .MASK = 0x10, .TYPE = 0 },  // DJNZ
    { .FUNCTION_PTRS.HANDLER = EI,          .MASK = 0xFB, .TYPE = 0 },  // EI
    { .FUNCTION_PTRS.HANDLER = HALT,        .MASK = 0x76, .TYPE = 0 },  // HALT
    { .FUNCTION_PTRS.HANDLER_8 = IN,        .MASK = 0xDB, .TYPE = 1 },  // IN A, (n)
    { .FUNCTION_PTRS.HANDLER_8 = INC,       .MASK = 0x04, .TYPE = 1 },  // INC B
    { .FUNCTION_PTRS.HANDLER = INI_IND,     .MASK = 0xA2, .TYPE = 0 },  // INI
    { .FUNCTION_PTRS.HANDLER = IND,         .MASK = 0xAA, .TYPE = 0 },  // IND
    { .FUNCTION_PTRS.HANDLER = INDR,        .MASK = 0xBA, .TYPE = 0 },  // INDR
    { .FUNCTION_PTRS.HANDLER = INI,         .MASK = 0xA2, .TYPE = 0 },  // INI
    { .FUNCTION_PTRS.HANDLER = INIR,        .MASK = 0xB2, .TYPE = 0 },  // INIR
    { .FUNCTION_PTRS.HANDLER = JP,          .MASK = 0xC3, .TYPE = 0 },  // JP nn
    { .FUNCTION_PTRS.HANDLER = LDI_LDD,     .MASK = 0xA0, .TYPE = 0 },  // LDI
    { .FUNCTION_PTRS.HANDLER = LDI,         .MASK = 0xA0, .TYPE = 0 },  // LDI
    { .FUNCTION_PTRS.HANDLER = LDIR,        .MASK = 0xB0, .TYPE = 0 },  // LDIR
    { .FUNCTION_PTRS.HANDLER = LDD,         .MASK = 0xA8, .TYPE = 0 },  // LDD
    { .FUNCTION_PTRS.HANDLER = LDDR,        .MASK = 0xB8, .TYPE = 0 },  // LDDR
    { .FUNCTION_PTRS.HANDLER = LD_A_B,      .MASK = 0x78, .TYPE = 0 },  // LD A, B
    { .FUNCTION_PTRS.HANDLER = LD_A_C,      .MASK = 0x79, .TYPE = 0 },  // LD A, C
    { .FUNCTION_PTRS.HANDLER = LD_A_IMM,    .MASK = 0x3E, .TYPE = 0 },  // LD A, IMM
    { .FUNCTION_PTRS.HANDLER = LD_B_IMM,    .MASK = 0x06, .TYPE = 0 },  // LD B, IMM
    { .FUNCTION_PTRS.HANDLER = LD_C_IMM,    .MASK = 0x0E, .TYPE = 0 },  // LD C, IMM
    { .FUNCTION_PTRS.HANDLER = SUB,         .MASK = 0x90, .TYPE = 0 },  // SUB A, B
    { .FUNCTION_PTRS.HANDLER = NEG,         .MASK = 0xED44, .TYPE = 0 }, // NEG
    { .FUNCTION_PTRS.HANDLER = OR,          .MASK = 0xB0, .TYPE = 0 },  // OR A, B
    { .FUNCTION_PTRS.HANDLER = OUTI_OUTD,   .MASK = 0xA3, .TYPE = 0 },  // OUTI
    { .FUNCTION_PTRS.HANDLER = OUTD,        .MASK = 0xAB, .TYPE = 0 },  // OUTD
    { .FUNCTION_PTRS.HANDLER = OUTI,        .MASK = 0xA3, .TYPE = 0 },  // OUTI
    { .FUNCTION_PTRS.HANDLER = OTIR,        .MASK = 0xB3, .TYPE = 0 },  // OTIR
    { .FUNCTION_PTRS.HANDLER = OTDR,        .MASK = 0xBB, .TYPE = 0 },  // OTDR
    { .FUNCTION_PTRS.HANDLER = OUT,         .MASK = 0xD3, .TYPE = 0 },  // OUT (n), A
    { .FUNCTION_PTRS.HANDLER_8 = RES,       .MASK = 0x80, .TYPE = 1 },  // RES 0, B
    { .FUNCTION_PTRS.HANDLER = RET,         .MASK = 0xC9, .TYPE = 0 },  // RET
    { .FUNCTION_PTRS.HANDLER = RET_CC,      .MASK = 0xC0, .TYPE = 0 },  // RET NZ
    { .FUNCTION_PTRS.HANDLER = RETI,        .MASK = 0xED4D, .TYPE = 0 }, // RETI
    { .FUNCTION_PTRS.HANDLER = RETN,        .MASK = 0xED45, .TYPE = 0 }, // RETN
    { .FUNCTION_PTRS.HANDLER_8 = RL,        .MASK = 0x10, .TYPE = 1 },  // RL B
    { .FUNCTION_PTRS.HANDLER_8 = RLC,       .MASK = 0x00, .TYPE = 1 },  // RLC B
    { .FUNCTION_PTRS.HANDLER = SBC,         .MASK = 0x98, .TYPE = 0 },  // SBC A, B
    { .FUNCTION_PTRS.HANDLER = SCF,         .MASK = 0x37, .TYPE = 0 },  // SCF
    { .FUNCTION_PTRS.HANDLER_8 = SET,       .MASK = 0xC0, .TYPE = 1 },  // SET 0, B
    { .FUNCTION_PTRS.HANDLER_8 = SLA,       .MASK = 0x20, .TYPE = 1 },  // SLA B
    { .FUNCTION_PTRS.HANDLER_8 = SRA,       .MASK = 0x28, .TYPE = 1 },  // SRA B
    { .FUNCTION_PTRS.HANDLER = XOR,         .MASK = 0xA8, .TYPE = 0 },  // XOR A, B
};
#endif
