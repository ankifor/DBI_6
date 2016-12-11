BASEDIR = .
ODIR = $(BASEDIR)/Release
ORUNTIME = $(BASEDIR)/Runtime
TARGET = DBI_6
#perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,branch-misses,faults,migrations -p

INCLUDES = -I.

CXX = g++
CXXFLAGS = -g -O3 -fexpensive-optimizations -std=c++11 -Wall -DNDEBUG $(INCLUDES)
LIBS = -L. -lrt -ldl -rdynamic
#flags for objects, needed for runtime compilation
CXXFLAGS_RUNTIME = -O3 -fexpensive-optimizations -std=c++11 -Wall -DNDEBUG $(INCLUDES) -fPIC


$(ODIR)/%.cpp.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(ORUNTIME)/%.cpp.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS_RUNTIME)

SRC = main.cpp Schema.cpp code_generation.cpp helpers.cpp Parser_Schema.cpp Parser_Query.cpp schema_1.cpp Types.cpp olap_query.cpp

OBJ0 = $(addsuffix .cpp.o, $(basename $(SRC)))
OBJ = $(patsubst %,$(ODIR)/%,$(OBJ0))

SRC_RUNTIME = schema_1.cpp Types.cpp
OBJ_RUNTIME0 = $(addsuffix .cpp.o, $(basename $(SRC_RUNTIME)))
OBJ_RUNTIME = $(patsubst %,$(ORUNTIME)/%,$(OBJ_RUNTIME0))


$(TARGET): prepare $(OBJ) 
	$(CXX) -o $@ $(OBJ) $(CXXFLAGS) $(LIBS)
	
runtime: prepare_runtime $(OBJ_RUNTIME)

prepare:
	mkdir -p $(ODIR)

prepare_runtime:
	mkdir -p $(ORUNTIME)

clean:
	rm -r -f $(ODIR)
