#===========================================

# Makefile for CSC300 Data Structures.

# Author: John M. Weiss, Ph.D.
# Written 12-Sep-2007.

# Usage:        make target1 target2 ...

#===========================================

# Define some useful vars

# GNU C/C++ compiler and linker:
CC = g++
LINK = g++

# Turn on optimization and warnings (add -g for debugging with gdb):
# CPPFLAGS =            # no preprocessor flags
CC_FLAGS = -O0 -Wall -g
LIBS = -lbcm2835 -lbluetooth -lisp2 -lignitech
#LIBS = -lbcm2835 -lerrno -lfcntl -lstring -ltermios -lunistd

# File names
EXEC = data_logger
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

#===========================================
# Targets:
#===========================================
all:data_logger

check:

distcheck:

# Main target
$(EXEC): $(OBJECTS)
	$(LINK) $(OBJECTS) $(LIBS) -o $(EXEC)

# To obtain object files
%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $< -o $@
 
#===========================================
# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)
clean_object:
	rm -f *.o

depend: .depend

.depend: $(SOURCES)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ -MF  ./.depend;

include .depend
 
