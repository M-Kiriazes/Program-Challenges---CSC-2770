CC = gcc
CFLAGS = -Wall -pthread

SRCS = HTTP_Client.c HTTP_Server.c
OBJS = $(SRCS:.c=.o)
TARGET = HTTP_Client HTTP_Server

all: $(TARGET)

$(TARGET): $(OBJS)
    $(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

clean:
    rm -f $(OBJS) $(TARGET)