CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra
LIB_DIR := lib
SRC_DIR := src

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:.cpp=.o)
LIBRARY := $(LIB_DIR)/libruda.a

.PHONY: all clean

all: $(LIBRARY)

$(LIBRARY): $(OBJS)
	mkdir -p $(LIB_DIR)
	ar rcs $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(LIB_DIR)/*.a $(SRC_DIR)/*.o
