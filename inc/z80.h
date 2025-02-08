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

#endif



#endif
