CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LFLAGS = 
LIBS = -lm -lpthread
SOURCES = main.c
OBJECTS = $(subst .c,.o,$(SOURCES))
BIN = main
.PHONY: clean help

main: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o $@ 

%.o : %.c
	$(CC) $(CFLAGS) -c $< 

all : $(BIN)

clean:
	rm -f $(OBJECTS) $(BIN) *~
