CC = gcc
CFLAGS = -g -Wall -I.
SRCS = ca.c main.c
OBJS = $(SRCS:.c=.o)
TARGET = odca

all: $(OBJS)
	@$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $^

.PHONY: clean
clean:
	@echo removing \"*.o\"
	@rm *.o
	@echo removing \"$(TARGET)\"
	@rm $(TARGET)
