# Makefile for ConsoleMP
# Author: Henry J Schmale
# Date    March 18, 2015

# EXE
EXE       := ConsoleMP 

# Compiler
CC        := gcc
C_FLGS    := -I/usr/include/ffmpeg
CXX       := g++
CXX_FLGS  := -std=c++11 -I/usr/include/ffmpeg
LD_FLGS   := -lffmpeg 

# Source Code
SRC       := main.c

CPPOBJ    := $(SRC:.cpp=.o) 
COBJ      := $(SRC:.c=.o)

# **********************************************
# Begin Targets
# **********************************************

all: depend $(EXE)

$(EXE): $(CPPOBJ) $(COBJ)
	$(CC) $(LD_FLGS) -o $@ $(COBJ) $(CPPOBJ)

clean:
	rm -rf *.o
	if [ -e $(EXE) ] ; then rm $(EXE) ; fi

# ***********************************************		
# Rules to make various files
# ***********************************************

# C++ Files
%.o: %.cpp 
	$(CXX) -c $(CXX_FLGS) -o $@ $<

# C Files
%.o: %.c
	$(CC) $(C_FLGS) -o $@ $<

# ***********************************************
# Dependcy Resolution
# ***********************************************

depend: .depend

.depend: $(SRC) $(HEADERS)
	rm -f ./.depend
	$(CXX) $(CXX_FLGS) -MM $^ > ./.depend


