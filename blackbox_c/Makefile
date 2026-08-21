CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Iinclude

TARGET = build/blackbox.exe

SOURCES = src/main.c src/event.c
OBJECTS = build/main.o build/event.o

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

build/main.o: src/main.c include/event.h
	$(CC) $(CFLAGS) -c src/main.c -o build/main.o

build/event.o: src/event.c include/event.h
	$(CC) $(CFLAGS) -c src/event.c -o build/event.o

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f build/*.o build/blackbox.exe