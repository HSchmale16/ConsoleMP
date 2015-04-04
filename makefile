# Makefile for ConsoleMP
# Author: Henry J Schmale
# Date    March 18, 2015

# EXE
EXE       := ConsoleMP 

# Compiler
CC        := gcc
C_FLGS    := -std=c11 -I/usr/include/ffmpeg
CXX       := g++
CXX_FLGS  := -std=c++11 -I/usr/include/ffmpeg
LD_FLGS   := -lavformat -lavcodec -lswscale -lz -lSDL

# Source Code
SRC       := main.cpp

OBJ       := $(SRC:.cpp=.cpp.o)

# **********************************************
# Begin Targets
# **********************************************

all: depend $(EXE)

$(EXE): $(OBJ)
	$(CXX) $(LD_FLGS) -o $@ $(OBJ)

clean:
	rm -rf *.o
	if [ -e $(EXE) ] ; then rm $(EXE) ; fi

# ***********************************************		
# Rules to make various files
# ***********************************************

# C++ Files
%.cpp.o: %.cpp
	$(CXX) -c $(CXX_FLGS) -o $@ $<

# C Files
%.c.o: %.c
	$(CC) -c $(C_FLGS) -o $@ $<

# ***********************************************
# Dependcy Resolution
# ***********************************************

depend: .depend

.depend: $(SRC) $(HEADERS)
	rm -f ./.depend
	$(CXX) $(CXX_FLGS) -MM $^ > ./.depend


