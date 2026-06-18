CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
LIBS = -lncurses
BUILD_DIR = build
TARGET = $(BUILD_DIR)/snake

.PHONY: all run clean

all: $(TARGET)

$(TARGET): main.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET) $(LIBS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: $(TARGET)
	$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
