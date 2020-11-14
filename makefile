# basic GTK+ app makefile MD Harrington 30th April 2018
# Change to suite your target name file or target exe name
TARGET = InstrumentMeter
# your source file you want to compile is changed here
SOURCES = Meter1.c
OBJS	= ${SOURCES:.c=.o}
CFLAGS	= -Wall -Wextra -v $(shell pkg-config --cflags gtk+-3.0)
LDFLAGS	=$(shell pkg-config --libs gtk+-3.0) -lm -rdynamic
CC      = gcc -c
LINKER	= gcc -o


$(TARGET) :obj
	@${LINKER} ${TARGET} ${OBJS} ${LDFLAGS} 
	@echo "linking complete"

obj	:
	@${CC} ${CFLAGS} ${SOURCES} 
	@echo "Compilation complete!"

clean:
	rm -f $(OBJS)
	rm -f $(PACKAGE)


# end of file

