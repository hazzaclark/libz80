# libz80
Zilog Z80 Standalone Emulation Library

![2025-02-13-195423_1365x652_scrot](https://github.com/user-attachments/assets/24d8a57b-1441-4ec3-8acd-c6b633ec4b4c)


# Motive:

The motive behind this repository is to provide a surrogate means of aiding in relevant Zilog Z80 developments.

Such is the case with one of the many ambitions, is to aid in the audio processing for [mdemu](https://github.com/hazzaclark/mdemu) as well as providing a low-level, lightweight solution to transition into lightweight Z80 computation

Created using ANSI C99 standard, this project aims to promote an ease of use means of creating a library that emulates the likeness of these microprocessors

# Usage:

```
git clone this repository

make clean

make
```

the following output will vary depending on system:

### Windows:

```
libz80.a
```

### UNIX:

```
libz80.so
```

the makefile in question is universal, allowing for ease of use compiling on other systems

# Resources:

[Z80 PROGRAMMER MANUAL](https://www.zilog.com/docs/z80/um0080.pdf)

[Z80 INSTRUCTION SET](https://map.grauw.nl/resources/z80instr.php)
