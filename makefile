# The following just specifies some variables for "flexibility".

# Specify the C++ compiler
CXX     = g++

# Specify options to pass to the compiler. Here it sets the optimisation
# level, outputs debugging info for gdb, and C++ version to use.
CXXFLAGS = -O0 -g3 -std=c++14

main: Zombie.h Zombie.cpp
	g++ Zombie.cpp -o main

# Some cleanup functions, invoked by typing "make clean" or "make deepclean"
deepclean: 
	rm -f *~ *.o main *.exe *.stackdump

clean:
	rm -f *~ *.o *.stackdump

