CC=gcc
TARGET=bresenham
DEBUG=-g
LFLAGS=-std=c99 -Wall $(DEBUG)
CFLAGS=-std=c99 -Wall $(DEBUG) -c
FRAMEWORKS=-framework OpenGL -framework GLUT
DEPS=map.o

all: $(TARGET)

$(TARGET) : main.c $(DEPS)
	    $(CC) $(LFLAGS) main.c -o $(TARGET) $(FRAMEWORKS) $(DEPS)

%.o: %.cpp %.h
	    $(CC) $(CFLAGS) $<

.IGNORE .PHONY clean :
	rm $(TARGET)
	rm *.o
	rm -rf $(TARGET).dYSM
