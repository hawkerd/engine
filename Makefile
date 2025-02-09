# compiler/ flags
CXX = g++
CC  = gcc
CXXFLAGS = -Wall -Wextra -std=c++17
CFLAGS = -Wall -Wextra -std=c11

# libraries
LDFLAGS = -lglfw -lGL

# src files
CXX_SRCS = game.cpp renderer.cpp
C_SRCS = glad.c
OBJS = $(CXX_SRCS:.cpp=.o) $(C_SRCS:.c=.o)

# output
TARGET = game

all: $(TARGET)

# link
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp renderer.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# clean
clean:
	rm -f $(OBJS) $(TARGET)

# run
run: $(TARGET)
	./$(TARGET)