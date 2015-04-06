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
LD_FLGS   := -lavformat \
			 -lavcodec \
			 -lavfilter \
			 -lavutil \
			 -lswscale \
			 -lz \
			 -lSDL \
			 -lsqlite3

# Source Code
HEADERS   := audioDecoder.h

CXX_SRC   := main.cpp

C_SRC     := audioDecoder.c

CXX_OBJ   := $(CXX_SRC:.cpp=.cpp.o)

C_OBJ     := $(C_SRC:.c=.c.o)

# **********************************************
# Begin Targets
# **********************************************

all: depend $(EXE)

$(EXE): $(CXX_OBJ) $(C_OBJ)
	$(CXX) $(LD_FLGS) -o $@ $(CXX_OBJ) $(C_OBJ)

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

.depend: $(CXX_SRC) $(C_SRC) $(HEADERS)
	rm -f ./.depend
	$(CXX) $(CXX_FLGS) -MM $^ > ./.depend


