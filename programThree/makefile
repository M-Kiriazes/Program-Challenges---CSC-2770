CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

SRCS = programChallenge3.c
OBJS = $(SRCS:.c=.o)
TARGET = programChallenge3

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)
