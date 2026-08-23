CC = gcc

CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Iblackbox_c/include

TARGET = blackbox_c/build/blackbox.exe

OBJECTS = blackbox_c/build/main.o \
          blackbox_c/build/event.o \
          blackbox_c/build/queue.o \
          blackbox_c/build/timestamp.o \
          blackbox_c/build/entity.o \
		  blackbox_c/build/scenario.o

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

blackbox_c/build/main.o: blackbox_c/src/main.c \
                         blackbox_c/include/entity.h \
                         blackbox_c/include/event.h \
                         blackbox_c/include/queue.h \
                         blackbox_c/include/timestamp.h \
						 blackbox_c/include/scenario.h
	$(CC) $(CFLAGS) -c blackbox_c/src/main.c -o blackbox_c/build/main.o

blackbox_c/build/event.o: blackbox_c/src/event.c \
                          blackbox_c/include/event.h \
                          blackbox_c/include/entity_base.h \
                          blackbox_c/include/timestamp.h
	$(CC) $(CFLAGS) -c blackbox_c/src/event.c -o blackbox_c/build/event.o

blackbox_c/build/queue.o: blackbox_c/src/queue.c \
                          blackbox_c/include/queue.h \
                          blackbox_c/include/event.h \
                          blackbox_c/include/entity_base.h
	$(CC) $(CFLAGS) -c blackbox_c/src/queue.c -o blackbox_c/build/queue.o

blackbox_c/build/timestamp.o: blackbox_c/src/timestamp.c \
                              blackbox_c/include/timestamp.h
	$(CC) $(CFLAGS) -c blackbox_c/src/timestamp.c -o blackbox_c/build/timestamp.o

blackbox_c/build/entity.o: blackbox_c/src/entity.c \
                           blackbox_c/include/entity.h \
                           blackbox_c/include/entity_base.h \
                           blackbox_c/include/event.h \
                           blackbox_c/include/timestamp.h
	$(CC) $(CFLAGS) -c blackbox_c/src/entity.c -o blackbox_c/build/entity.o

blackbox_c/build/scenario.o: blackbox_c/src/scenario.c \
							 blackbox_c/include/scenario.h
	$(CC) $(CFLAGS) -c blackbox_c/src/scenario.c -o blackbox_c/build/scenario.o

run: $(TARGET)
	./$(TARGET)

clean:
	del /Q blackbox_c\build\*.o blackbox_c\build\blackbox.exe