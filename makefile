CC = gcc
CCFLAGS = -Wall -g
TARGET = dynamic

all: dynamic

dynamic: dynamic.o main.o
	$(CC) $(CCFLAGS) -o books dynamic.o main.o

main.o: main.c
	$(CC) $(CCFLAGS) -c main.c

dynamic.o: dynamic.c
	$(CC) $(CCFLAGS) -c dynamic.c
