# $@ and $^ are the left and right sides of the :, respectively
# $< is the first item in the dependencies list

BDIR = build
SDIR = source

EXECUTABLE = $(BDIR)/filters

SOURCES = $(wildcard $(SDIR)/*.cpp)

DEPENDENCIES = $(wildcard $(SDIR)/*.hpp)

_OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS = $(patsubst $(SDIR)/%,$(BDIR)/%,$(_OBJECTS))

#CXX = g++

CXXFLAGS = -Wall -Wpedantic -std=c++14 

ifeq ($(DEBUG), 1)
	CXXFLAGS += -O0 -g3
else
	CXXFLAGS += -O2 -g0
endif

$(BDIR)/%.o : $(SDIR)/%.cpp $(DEPENDENCIES)
	$(CXX) $(CXXFLAGS) -c $< -o $@  `pkg-config --cflags --libs opencv`

$(EXECUTABLE) : $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@  `pkg-config --cflags --libs opencv`

.PHONY : cleanobj clean

cleanobj :
	rm $(OBJECTS)

clean : cleanobj
	rm $(EXECUTABLE)