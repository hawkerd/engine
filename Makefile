# Compiler and Flags
CXX = g++
CC  = gcc
CXXFLAGS = -Wall -Wextra -std=c++17
CFLAGS = -Wall -Wextra -std=c11

# Directories
SRC_DIR = src
BIN_DIR = bin
INCLUDE_DIR = include
LIB_DIR = libs

# Libraries
LDFLAGS = -Llibs/glfw/lib-mingw-w64 -lglfw3 -lopengl32 -lgdi32

# Source Files
CXX_SRCS = $(SRC_DIR)/renderer.cpp $(SRC_DIR)/stb_image.cpp
C_SRCS = $(SRC_DIR)/glad.c
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(CXX_SRCS)) \
       $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(C_SRCS))

# Output
TARGET = $(BIN_DIR)/renderer.exe

all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compiling C++ Files
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/stb_image.h
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Compiling C Files
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Clean
clean:
	del /s /q $(BIN_DIR)\*
	rmdir /s /q $(BIN_DIR)

# Run
run: $(TARGET)
	$(TARGET)
