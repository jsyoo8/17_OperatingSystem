.SUFFIXES : .c .o

OBJECTS = main.o read.o write.o
CC = gcc
CFLAGS = -g
TARGET = file

$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)

%o : %c
	$(CC) -c $^

clean :
	rm -f $(TARGET) $(OBJECTS)
