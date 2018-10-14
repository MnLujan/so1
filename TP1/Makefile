# Directorios
OUTPUT_DIR=bin

# Compilacion
OUTPUTFILE=ksamp
CC=gcc
CFLAGS=-Wall -pedantic

all : ksamp

ksamp : main.c LAB1.h
	mkdir -p $(OUTPUT_DIR)
	$(CC) $(CFLAGS)  -o $(OUTPUT_DIR)/$(OUTPUTFILE).o -c main.c
	$(CC) $(CFLAGS) -o $(OUTPUT_DIR)/$(OUTPUTFILE) $(OUTPUT_DIR)/$(OUTPUTFILE).o

.PHONY : clean
clean :
	rm  -Rf $(OUTPUT_DIR)
