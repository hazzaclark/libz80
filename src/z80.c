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

void Z80_SET_REGISTERS(CPU_Z80* const Z80, int REGISTER, U8 VALUE)
{
    switch (REGISTER) 
    {
        case Z80_B: Z80->REGISTER_BASE[Z80_B] = VALUE; break;
        case Z80_C: Z80->REGISTER_BASE[Z80_C] = VALUE; break;
        case Z80_D: Z80->REGISTER_BASE[Z80_D] = VALUE; break;
        case Z80_E: Z80->REGISTER_BASE[Z80_E] = VALUE; break;
        case Z80_H: Z80->REGISTER_BASE[Z80_E] = VALUE; break;
        case Z80_L: Z80->REGISTER_BASE[Z80_L] = VALUE; break;
        case Z80_A: Z80->REGISTER_BASE[Z80_A] = VALUE; break;
        case Z80_F: Z80->REGISTER_BASE[Z80_F] = VALUE; break;
        case Z80_IXH: Z80->REGISTER_BASE[Z80_IXH] = VALUE; break;
        case Z80_IXL: Z80->REGISTER_BASE[Z80_IXL] = VALUE; break;
        case Z80_IYH: Z80->REGISTER_BASE[Z80_IYH] = VALUE; break;
        case Z80_IYL: Z80->REGISTER_BASE[Z80_IYL] = VALUE; break;
        case Z80_I: Z80->REGISTER_BASE[Z80->INTERRUPT_VECTOR] = VALUE; break;
        case Z80_R: Z80->REGISTER_BASE[Z80->MEMORY_REFRESH] = VALUE; break;
        default: break; 
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

    Z80_SET_PAIR(Z80, Z80_A, Z80_F, 0x0000); // AF
    Z80_SET_PAIR(Z80, Z80_B, Z80_C, 0x0000); // BC
    Z80_SET_PAIR(Z80, Z80_D, Z80_E, 0x0000); // DE
    Z80_SET_PAIR(Z80, Z80_H, Z80_L, 0x0000); // HL
    Z80_SET_PAIR(Z80, Z80_IXH, Z80_IXL, 0x0000); // IX
    Z80_SET_PAIR(Z80, Z80_IYH, Z80_IYL, 0x0000); // IY
    Z80_SET_REGISTERS(Z80, Z80_I, 0x00);
    Z80_SET_REGISTERS(Z80, Z80_R, 0x00);

    for(int INDEX = 0; INDEX < 64; INDEX++)
    {
        Z80->READ_MAPPER[INDEX] = (U8*)malloc(1024 * sizeof(U8));
        Z80->WRITE_MAPPER[INDEX] = Z80->READ_MAPPER[INDEX];
    }
}

// GET THE DEBUG OUTPUT FOR THE EMULATOR TO BE ABLE TO KEEP A TRACK OF TRACES
// ACROSS THE RESPECTIVE FACTIONS OF THE ARCH

void Z80_OUTPUT(CPU_Z80* const Z80)
{
    printf("PC: %04X, AF: %04X (A: %02X F: %02X), BC: %04X (B: %02X C: %02X), "
           "DE: %04X (D: %02X E: %02X), HL: %04X (H: %02X L: %02X)\n"
           "SP: %04X, IX: %04X, IY: %04X, I: %02X, R: %02X\n",
           Z80->PC,
           (Z80_GET_REGISTERS(Z80, Z80_A) << 8) | Z80_GET_REGISTERS(Z80, Z80_F),
           Z80_GET_REGISTERS(Z80, Z80_A),
           Z80_GET_REGISTERS(Z80, Z80_F),
           (Z80_GET_REGISTERS(Z80, Z80_B) << 8) | Z80_GET_REGISTERS(Z80, Z80_C),
           Z80_GET_REGISTERS(Z80, Z80_B),
           Z80_GET_REGISTERS(Z80, Z80_C),
           (Z80_GET_REGISTERS(Z80, Z80_D) << 8) | Z80_GET_REGISTERS(Z80, Z80_E),
           Z80_GET_REGISTERS(Z80, Z80_D),
           Z80_GET_REGISTERS(Z80, Z80_E),
           (Z80_GET_REGISTERS(Z80, Z80_H) << 8) | Z80_GET_REGISTERS(Z80, Z80_L),
           Z80_GET_REGISTERS(Z80, Z80_H),
           Z80_GET_REGISTERS(Z80, Z80_L),
           Z80->SP,
           (Z80_GET_REGISTERS(Z80, Z80_IXH) << 8) | Z80_GET_REGISTERS(Z80, Z80_IXL),
           (Z80_GET_REGISTERS(Z80, Z80_IYH) << 8) | Z80_GET_REGISTERS(Z80, Z80_IYL),
           Z80_GET_REGISTERS(Z80, Z80_I),
           Z80_GET_REGISTERS(Z80, Z80_R),
           printf("=======================================================\n")
    );
}

/*===============================================================================*/
/*-------------------------------------------------------------------------------*/
//                        Z80 READ AND WRITE FUNCTIONS
/*-------------------------------------------------------------------------------*/
/*===============================================================================*/

// VERY SIMPLE READ AND WRITE SCHEMA FOR THE 16-BIT WIDE BUS THAT THE Z80 OFFERS
// THE IDEA IS THAT THE READER AND WRITER WILL EVALUATE THE SPACE DESIGNATED FOR THE BUS

// AND BIT SHIFT ACCORDINGLY TO THE MAX FLAG VALUE POSSIBLE

// IN THE CASE OF THE 16 BIT READ AND WRITE VALUES, WE WILL LOOK TO DETERMINE
// THE HI AND LO VALUES DESIGNATED FOR THE HI AND LO REGISTERS RESPECTIVELY
// AND JUST ADD THE REMAINING BIT MANTISSA TO IT

U8 READ_8(CPU_Z80* Z, U16 ADDR)
{
    assert(Z->READ_MAPPER[ADDR >> 10]);
    return Z->READ_MAPPER[ADDR >> 10][ADDR & 0x3FF];
}

void WRITE_8(CPU_Z80* Z, U16 ADDR, U8 VALUE)
{
    assert(Z->READ_MAPPER[ADDR >> 10]);
    Z->WRITE_MAPPER[ADDR >> 10][ADDR & 0x3FF] = VALUE;
}

U16 READ_16(CPU_Z80* Z, U16 ADDR)
{
    U16 const LO = READ_8(Z, ADDR + 0); 
    U16 const HI = READ_8(Z, ADDR + 1);

    return (HI << 8) | LO;
}

void WRITE_16(CPU_Z80* Z, U16 ADDR, U8 VALUE)
{
    WRITE_8(Z, ADDR + 0, VALUE & 0xFF);
    WRITE_8(Z, ADDR + 1, VALUE >> 8);
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
