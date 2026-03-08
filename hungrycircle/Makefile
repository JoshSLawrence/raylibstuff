CC = gcc
CFLAGS = -I./include
LDFLAGS = -L./lib
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
TARGET = hungrycricle
SRC = src/main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o build/$(TARGET) $(LDFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)
