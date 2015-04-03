# Makefile for ConsoleMP
# Author: Henry J Schmale
# Date    March 18, 2015

# EXE
EXE       := ConsoleMP 

# Compiler
CXX       := g++
CXX_FLGS  := -std=c++11
LD_FLGS   := 

# Source Code
SRC       := main.cpp

OBJ       := $(SRC:.cpp=.o)

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
.cpp.o:
	$(CXX) -c $(CXX_FLGS) -o $@ $<

# ***********************************************
# Dependcy Resolution
# ***********************************************

depend: .depend

.depend: $(SRC) $(HEADERS)
	rm -f ./.depend
	$(CXX) $(CXX_FLGS) -MM $^ > ./.depend


