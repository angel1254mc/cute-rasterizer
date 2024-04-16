CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra
LIB_DIR := lib
SRC_DIR := src
OBJ_DIR := $(LIB_DIR)/obj

# Recursively find all cpp files and determine object file paths
SRCS := $(shell find $(SRC_DIR) -type f -name '*.cpp')
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
# Extract directory structure from source files and create corresponding subfolders in OBJ_DIR
OBJ_SUBDIRS := $(sort $(dir $(OBJS)))
# Ensure OBJ_DIR and all subfolders are created before generating object files
$(shell mkdir -p $(OBJ_SUBDIRS))

LIBRARY := $(LIB_DIR)/libruda.a

.PHONY: all clean

all: $(LIBRARY)

$(LIBRARY): $(OBJS)
	mkdir -p $(LIB_DIR)
	ar rcs $@ $^

# Rule to generate object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(LIB_DIR)/*.a $(OBJ_DIR)/*.o
