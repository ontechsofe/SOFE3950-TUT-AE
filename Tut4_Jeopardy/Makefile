CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LFLAGS = 
LIBS = -lm
SOURCES = main.c
OBJECTS = $(subst .c,.o,$(SOURCES))
BIN = jeopardy
.PHONY: clean help

jeopardy: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o $@ 

%.o : %.c
	$(CC) $(CFLAGS) -c $< 

all : $(BIN)

clean:
	rm -f $(OBJECTS) $(BIN) *~

help:
	@echo "Valid targets:"
	@echo "  clean:  removes .o and binary files"
