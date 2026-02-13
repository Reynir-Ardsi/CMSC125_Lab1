CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS = mysh.c parser.c external.c builtin.c jobs.c
OBJS = $(SRCS:.c=.o)

TARGET = mysh

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean