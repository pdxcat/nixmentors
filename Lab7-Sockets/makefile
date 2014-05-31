client:
	gcc -g client.c -o client

server:
	gcc -g server.c -o server

all: client server

run:
	./server &
	./client

clean:
	rm -r *.gch *.o *.a server client 2> /dev/null
