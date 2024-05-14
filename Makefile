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

client.o: Client.cpp
	$(CC) -c $(CFLAGS) Client.cpp -o client.o

client_main.o: client_main.cpp
	$(CC) -c $(CFLAGS) client_main.cpp -o client_main.o

client: client_main.o client.o buffer.o helpers.o requests.o
	$(CC) $(CFLAGS) client_main.o client.o buffer.o helpers.o requests.o -o client

clean:
	rm -rf *.o $(TARGETS)

pack:
	zip -FSr 323CA_Zaharia_Marius-Tudor_Tema4PC Makefile *.cpp *.h *.hpp README.md

.PHONY: all clean pack
