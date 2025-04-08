// COPYRIGHT (C) HARRY CLARK 2025
// LIGHTWEIGHT, VERSATILE Z80 EMULATOR

// THIS FILE PERTAINS TOWARDS THE MAIN FUNCTIONALITY OF THE PROGRAM

// NESTED INCLUDES

#include "common.h"
#include "z80.h"
#include "z80OPCODE.h"

// SYSTEM INCLUDES

#include <stdio.h>
#include <stdlib.h>

// AS OF RIGHT NOW, I AM TESTING THE WATERS REGARDING WHICH OPCODES WORK AND WHAT HAVE YOU
// OF COURSE, BEING AS THOUGH THAT THE OPCODE LIST IS DIRECTLY FROM LIB68K, THE SCHEMA HAS BEEN TRIED AND TESTED

// AS OF RIGHT NOPW, I AM DOING A SIMPLE LD IMM TO EACH RESPECTIVE REGISTER AND RECORDING THEIR READ VALUE
// THIS IS ALL WITHIN CONJUNCTION OF WHICH OPCODES ARE BEING EXECUTED IN IT'S DESIGNATED FUNCTION

void EXECUTE_OPCODE(CPU_Z80* Z, U8 OPCODE)
{
    for (size_t i = 0; i < sizeof(BUILD_OPCODE_TABLE) / sizeof(BUILD_OPCODE_TABLE[0]); i++)
    {
        if (BUILD_OPCODE_TABLE[i].MASK == OPCODE)
        {
            printf("EXECUTING OPCODE: %-8s, 0x%02X\n", BUILD_OPCODE_TABLE[i].NAME, OPCODE);
            switch (BUILD_OPCODE_TABLE[i].TYPE)
            {
                case 0: 
                    BUILD_OPCODE_TABLE[i].FUNCTION_PTRS.HANDLER(Z, OPCODE);
                    break;

                case 1:  
                    {
                        U8 RESULT = BUILD_OPCODE_TABLE[i].FUNCTION_PTRS.HANDLER_8(Z);
                        printf("8 BIT RESULT: 0x%02X\n", RESULT);
                    }
                    break;

                case 2:  
                    {
                        U16 RESULT = BUILD_OPCODE_TABLE[i].FUNCTION_PTRS.HANDLER_16(Z);
                        printf("16 BIT RESULT: 0x%04X\n", RESULT);
                    }
                    break;

                default:
                    break;
            }

            Z80_OUTPUT(Z);
            return;
        }
    }

    printf("UNKNOWN OPCODE: 0x%02X\n", OPCODE);
    Z80_OUTPUT(Z);
}


// SIMPLE LOAD INTO MEMORY IN ACCORDANCE WITH THE MAX SIZE

void LOAD_TEST_PROGRAM(CPU_Z80* Z, U8* PROGRAM, U16 SIZE)
{
    for (U16 INDEX = 0; INDEX < SIZE; INDEX++)
    {
        U16 ADDR = 0x0000 + INDEX;
        WRITE_8(Z, ADDR, PROGRAM[INDEX]);
    }
}

int main(void)
{
    printf("====================================================\n");
    printf("        HARRY CLARK - ZILOG Z80 EMULATOR            \n");
    printf("====================================================\n");

    CPU_Z80 Z80;
    Z80_INIT(&Z80);

    printf("Z80 IS RUNNING: %p\n", (void*)&Z80);

    U8 TEST_PROGRAM[] = 
    {
        0x3E, 0x42,      
        0x06, 0x10,       
        0x0E, 0x20,       
        0x80,             
        0x78,             
        0x41,            
        0x80,            
        0x76              
    };

    LOAD_TEST_PROGRAM(&Z80, TEST_PROGRAM, sizeof(TEST_PROGRAM));

    while (1)
    {
        U8 OPCODE = READ_8(&Z80, Z80.PC);
        Z80.PC++;

        EXECUTE_OPCODE(&Z80, OPCODE);

        if (Z80.HALT)
        {
            break;
        }
    }

    printf("FINAL CPU STATE:\n");
    Z80_OUTPUT(&Z80);

    for (int INDEX = 0; INDEX < 64; INDEX++)
    {
        free(Z80.READ_MAPPER[INDEX]);
    }

    return 0;
}
