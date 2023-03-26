CC=gcc

all:	clean client server

client:	src/client.c
	${CC} -o client src/client.c

server: src/server.c
	${CC} -o server src/server.c -lpthread

clean: 
	rm -f client
	rm -f server

