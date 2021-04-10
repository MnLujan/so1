# Directorios
OUTPUT_DIR=bin

# Compilacion
OUTPUTFILE=bash
CC=gcc
CFLAGS=-Wall -pedantic

all : compile

compile : baash.c
	mkdir -p $(OUTPUT_DIR)
	$(CC) $(CFLAGS)  -o $(OUTPUT_DIR)/$(OUTPUTFILE).o -c baash.c
	$(CC) $(CFLAGS) -o $(OUTPUT_DIR)/$(OUTPUTFILE) $(OUTPUT_DIR)/$(OUTPUTFILE).o

.PHONY : clean
clean :
	rm  -Rf $(OUTPUT_DIR)
