# Compiler and Flags
CXX = g++
CXXFLAGS = -g -std=c++17 -I include -I include/imgui
LDFLAGS = -lglfw -lGL -lassimp

# Source Directories
SRC_DIR = source
IMGUI_DIR = $(SRC_DIR)/imgui
BUILD_DIR = build

# Source and Object Files
SRCS = $(wildcard *.c *.cpp $(SRC_DIR)/*.cpp $(IMGUI_DIR)/*.cpp)
OBJS = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRCS)))

# Target Executable
TARGET = main

# Default Rule
all: $(TARGET)

# Link Objects to Create Executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Compile Source Files to Object Files
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create Build Directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/$(SRC_DIR) $(BUILD_DIR)/$(IMGUI_DIR)

# Clean Build Files
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Run the Program
run: all
	./$(TARGET)
