CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lreadline
TARGET = safsh

all: $(TARGET)

$(TARGET): safsh.c
	$(CC) $(CFLAGS) safsh.c -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
