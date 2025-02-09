# compiler/ flags
CXX = g++
CC  = gcc
CXXFLAGS = -Wall -Wextra -std=c++17
CFLAGS = -Wall -Wextra -std=c11

# libraries
LDFLAGS = -lglfw -lGL

# src files
SRC_DIR = src
BIN_DIR = bin
INCLUDE_DIR = include

CXX_SRCS = $(SRC_DIR)/game.cpp $(SRC_DIR)/renderer.cpp
C_SRCS = $(SRC_DIR)/glad.c
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(CXX_SRCS)) \
       $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(C_SRCS))

# output
TARGET = $(BIN_DIR)/game

all: $(TARGET)

# link
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/renderer.h
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@
	
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@


# clean
clean:
	rm -rf $(BIN_DIR)/*

# run
run: $(TARGET)
	./$(TARGET)