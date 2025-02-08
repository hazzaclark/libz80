## COPYRIGHT (C) HARRY CLARK 2025
## LIGHTWEIGHT, VERSATILE Z80 EMULATOR

#########################################################################
##				PREFIXES			       ##
#########################################################################

CC 			=		$(PREFIX)gcc
AR			=		$(PREFIX)ar
WARNINGS	=		-std=c99 -Wall -Wextra -Wparentheses -Werror -pedantic
SRC			= 		src
INC			=		inc

#########################################################################
##				FILE ACCESS			       ##
#########################################################################

Z80_FILES	=		$(wildcard $(SRC)/*.c)
Z80_OBJS	=		$(Z80_FILES:$(SRC)/%.c=$(SRC)/%.o)

#########################################################################
##			    TARGET LINKING			       ##
#########################################################################

TARGET 		= 		libz80.exe  
LIBRARY		= 		libz80.so libz80.a     

all: $(TARGET)

$(LIBRARY): $(Z80_OBJS)
	$(AR) rcs $@ $^

$(TARGET): $(LIBRARY)
	$(CC) -o $@ -L. -lz80 $(WARNINGS)  

$(SRC)/%.o: $(SRC)/%.c
	$(CC) $(WARNINGS) -I$(INC) -c $< -o $@

clean:
	rm -f $(SRC)/*.o $(TARGET) $(LIBRARY)

.PHONY: clean

#########################################################################
##				MISC.				       ##
#########################################################################

include rules.mk