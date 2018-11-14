EXE = tracer
CXX = clang++-6.0
CXXFLAGS= -Wall -Werror -I. -I./BRDFS -I./BTDFS -I./Cameras -I./GeometricObjects -I./Lights -I./Materials -I./Tracers -I./Utilities -I./World
SRC = $(wildcard *.cpp) \
      $(wildcard */*.cpp)
OBJ = $(SRC:.cpp=.o)
DEP = $(OBJ:.o=.d)

$(EXE): $(OBJ)
	$(CXX) -o $@ $^

-include $(DEP)
%.d: %.cpp
	@$(CXX) $(CXXFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: clean
clean:
	rm -f $(OBJ) $(EXE) $(DEP)

# Makefile assistance:
# http://nuclear.mutantstargoat.com/articles/make/
# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

# main: main.cpp
#	clang++-6.0 -o main -I World -I Tracers -I Utilities -I GeometricObjects *.cpp */*.cpp
#   compiler, exe name, include all folders, compile all cpp in current folder and all sub folders
