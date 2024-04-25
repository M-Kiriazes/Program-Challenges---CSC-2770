CC = gcc
CFLAGS = -Wall -Wextra -g

all: http_client http_server

http_client: http_client.c
	$(CC) $(CFLAGS) -o http_client http_client.c

http_server: http_server.c
	$(CC) $(CFLAGS) -o http_server http_server.c

clean:
	rm -f http_client http_server