CC = g++
CFLAGS = -Wall -Wextra -std=c++17 -g

TARGETS = client

all: $(TARGETS)

buffer.o: buffer.cpp
	$(CC) -c $(CFLAGS) buffer.cpp -o buffer.o

helpers.o: helpers.cpp
	$(CC) -c $(CFLAGS) helpers.cpp -o helpers.o

requests.o: requests.cpp
	$(CC) -c $(CFLAGS) requests.cpp -o requests.o

client.o: client.cpp
	$(CC) -c $(CFLAGS) client.cpp -o client.o

client: client.o buffer.o helpers.o requests.o
	$(CC) $(CFLAGS) client.o buffer.o helpers.o requests.o -o client

clean:
	rm -rf *.o $(TARGETS)

pack:
	zip -FSr Zaharia_Marius-Tudor_323CA_Tema3PC Makefile *.cpp *.h README.md

.PHONY: all clan pack
