CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
LIBS = -lncurses
BUILD_DIR = build
TARGET = $(BUILD_DIR)/snake

# Phony files
.PHONY: all run clean

# Default: Compile all
all: $(TARGET)

# Compile and create build folder
$(TARGET): main.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) main.cpp $(LIBS) -o $(TARGET)

# Compile and launch build
run: $(TARGET)
	./$(TARGET)

# Clean up
clean:
	rm -rf $(BUILD_DIR)
