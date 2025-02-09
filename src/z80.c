// COPYRIGHT (C) HARRY CLARK 2025
// LIGHTWEIGHT, VERSATILE Z80 EMULATOR

// THIS FILE PERTAINS TOWARDS THE MAIN FUNCTIONALITY ENCOMPASSING THE UTILITY
// OF THE CPU AND IT'S SUBSEQUENT LOGIC, FUNCTION AND NECESSITIES

// NESTED INCLUDES

#include "z80.h"
#include "z80OPCODE.h"

// HELPER FUNCTION FOR BEING ABLE TO GET THE REGISTERS
// A STATICALLY-USED STRUCTURE FOR THE CPU WILL BE USED ON EVERY SUBSEQUENT ACCESS OF THE REGISTERS

// THIS DOESNT SEEM TO BE "TOO BAD" OF A SOLUTION DUE TO THE LACK OF REGISTERS TO BEGIN WITH

unsigned Z80_GET_REGISTERS(CPU_Z80* const Z80, int REGISTER)
{
    	/* IF THERE IS NO ACCESSIBLE METHODS */
	/* WE SPECIFICALLY ALLOCATE MEMORY SAID ACCESSING OF REGISTERS */

    switch (REGISTER)
    {
        case Z80_B: return Z80->REGISTER_BASE[Z80_B];
        case Z80_C: return Z80->REGISTER_BASE[Z80_C];
        case Z80_D: return Z80->REGISTER_BASE[Z80_D];
        case Z80_E: return Z80->REGISTER_BASE[Z80_E];
        case Z80_H: return Z80->REGISTER_BASE[Z80_H];
        case Z80_L: return Z80->REGISTER_BASE[Z80_L];
        case Z80_A: return Z80->REGISTER_BASE[Z80_A];
        case Z80_F: return Z80->REGISTER_BASE[Z80_F];
        case Z80_IXH: return Z80->REGISTER_BASE[Z80_IXH];
        case Z80_IXL: return Z80->REGISTER_BASE[Z80_IXL];
        case Z80_IYH: return Z80->REGISTER_BASE[Z80_IYH];
        case Z80_IYL: return Z80->REGISTER_BASE[Z80_IYL];
        case Z80_I: return Z80->REGISTER_BASE[Z80->INTERRUPT_VECTOR];
        case Z80_R: return Z80->REGISTER_BASE[Z80->MEMORY_REFRESH];

        default:
            return 0;
    }
}

// INITIALISE THE CPU STRUCTURE BASED ON IT'S RESPECTIVE MEMBERS
// MUCH TO THE CONTEMPORAIRES OF LIB68K, THIS WILL FOLLOW A MUCH SIMPLER INITIALISATION SCHEMA BY COMP

// THIS IS LARGE IN PART DUE TO THE FACT THAT THE Z80 IS OBVIOUSLY A LESS COMPLICATED
// MICROPROCESSOR TO WORK WITH

void Z80_INIT(CPU_Z80* const Z80)
{
    Z80->PC = 0x0000;
    Z80->SP = 0xFFFF;
    Z80->IRQ_MODE = 0;
    Z80->HALT = 0;
    Z80->INT_PENDING = 0;
    Z80->NMI_PENDING = 0;
    Z80->MEMORY_REFRESH = 0;
    Z80->CYCLES = 0;
    Z80->Z80_MEM->MEMORY_BASE = NULL;
    Z80->Z80_MEM->READ_8 = NULL;
    Z80->Z80_MEM->READ_16 = NULL;
    Z80->Z80_MEM->WRITE_8 = NULL;
    Z80->Z80_MEM->WRITE_16 = NULL;
}

// GET THE DEBUG OUTPUT FOR THE EMULATOR TO BE ABLE TO KEEP A TRACK OF TRACES
// ACROSS THE RESPECTIVE FACTIONS OF THE ARCH

void Z80_OUTPUT(CPU_Z80* const Z80)
{
    printf("PC: %04X, AF: %04X, BC: %04X, DE: %04X, HL: %04X, SP: %04X, "
           "IX: %04X, IY: %04X, I: %02X, R: %02X\n",
           Z80->PC,
           (Z80_GET_REGISTERS(Z80, Z80_A) << 8) | Z80_GET_REGISTERS(Z80, Z80_F),
           (Z80_GET_REGISTERS(Z80, Z80_B) << 8) | Z80_GET_REGISTERS(Z80, Z80_C), 
           (Z80_GET_REGISTERS(Z80, Z80_D) << 8) | Z80_GET_REGISTERS(Z80, Z80_E), 
           (Z80_GET_REGISTERS(Z80, Z80_H) << 8) | Z80_GET_REGISTERS(Z80, Z80_L),
           Z80->SP,
           (Z80_GET_REGISTERS(Z80, Z80_IXH) << 8) | Z80_GET_REGISTERS(Z80, Z80_IXL),
           (Z80_GET_REGISTERS(Z80, Z80_IYH) << 8) | Z80_GET_REGISTERS(Z80, Z80_IYL),
           Z80_GET_REGISTERS(Z80, Z80_I),
           Z80_GET_REGISTERS(Z80, Z80_R)
    );
}
