LIBS = `pkg-config --libs --cflags libusb-1.0`
CC = gcc

SOURCE = fcd.c hid-libusb.c main.c
EXEC = fcdfreq

all:
	$(CC) $(LIBS) $(SOURCE) -o $(EXEC)
 
