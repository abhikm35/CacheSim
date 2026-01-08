CXX := g++
CXXFLAGS := -std=c++2a -O2 -Wall -Wextra -Isrc

SRC_DIR := src
BUILD_DIR := build
TARGET := CacheIQ

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: $(TARGET)
	./$(TARGET) traces/cache_trace_common_case.cmt 32 4 LRU 1

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
