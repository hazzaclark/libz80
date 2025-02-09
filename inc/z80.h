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

#endif
#endif
