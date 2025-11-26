TARGET = bonded

SRC = $(wildcard scr/*.c)

CFLAGS = -Wall
LDFLAGS = -lraylib -lm

all: $(TARGET)

$(TARGET): $(SRC)
	gcc $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
