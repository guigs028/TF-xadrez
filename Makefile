CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -O2
SRCS=main.c board.c score.c
OBJS=$(SRCS:.c=.o)
TARGET=mines

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET) $(OBJS)
