# Compiler settings
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

# Source and object files
SOURCES = main.cpp Classes/Body/BacktrackSolver.cpp Classes/Body/BooleanFormula.cpp Classes/Body/Clause.cpp Classes/Body/Literal.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = backtrack_OrozcoAniceto

# Directories for headers
INCLUDE_DIRS = -IClasses/Headers

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)
