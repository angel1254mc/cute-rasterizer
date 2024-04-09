# Compiler settings
CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -pedantic
LDFLAGS := -L../../lib -lruda -L/usr/X11R6/lib -lX11

# Directories
SRCDIR := ../../src
BINDIR := ../../bin
TESTDIR := .

# Target executable name
EXECUTABLE := $(BINDIR)/test_pipeline

# Source file
SRC := $(TESTDIR)/testPipeline.cpp

# Targets
all: $(EXECUTABLE)

# Rule for compiling the test program
$(EXECUTABLE): $(SRC) $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/*.cpp)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

# Phony targets
.PHONY: clean

clean:
	rm -rf $(BINDIR)
