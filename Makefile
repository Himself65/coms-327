CC = g++
CFLAGS = -std=c++11 -g -Wall -I.
SRCS = GraphicsClient.cpp CellularAutomaton.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = gol

all: $(OBJS)
	@$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	@$(CC) $(CFLAGS) -o $@ -c $^

.PHONY: clean
clean:
	@echo removing \"*.o\"
	@rm *.o
	@echo removing \"$(TARGET)\"
	@rm $(TARGET)
