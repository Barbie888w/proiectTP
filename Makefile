CC = gcc
CFLAGS = -Wall -Wextra -std=c11 `pkg-config --cflags gtk+-3.0`
LIBS = `pkg-config --libs gtk+-3.0`
TARGET = joc

all: $(TARGET)

$(TARGET): main.c
	$(CC) main.c -o $(TARGET) $(CFLAGS) $(LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean