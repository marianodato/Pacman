CC = gcc 
CFLAGS  = -g -Wall
TARGET = Pacman

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c -lncurses

clean:
	$(RM) $(TARGET)