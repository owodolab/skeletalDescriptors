all: graspi 

BOOST= /Users/devyanijivani/Documents/Documents/GraSPI-2.4/src/boost_1_71_0 

CXXFLAGS=-std=c++0x -O3 -I. -I${BOOST}

COPT = $(CXXFLAGS) #-g3 -DDEBUG

graspi: mainSKEL.cpp graph_constructors.cpp
	g++ -o graspi -Wno-deprecated ${COPT} mainSKEL.cpp graph_constructors.cpp

clean:
	rm -rf *.o graspi 
