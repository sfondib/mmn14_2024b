# Makefile

CC = gcc
CFLAGS = -Wall -ansi -pedantic

# Object files
OBJS = helpful.o pre_processor.o pre_processor_funcs.o first_run.o first_run_funcs.o second_run.o table.o

# Output executable
TARGET = assembler

# Default rule
all: $(TARGET)

# Rule to link object files and create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rules to compile each .c file into a .o file
helpful.o: helpful.c helpful.h
	$(CC) $(CFLAGS) -c helpful.c -o helpful.o

pre_processor.o: pre_processor.c pre_processor_funcs.h
	$(CC) $(CFLAGS) -c pre_processor.c -o pre_processor.o

pre_processor_funcs.o: pre_processor_funcs.c pre_processor_funcs.h
	$(CC) $(CFLAGS) -c pre_processor_funcs.c -o pre_processor_funcs.o

first_run.o: first_run.c first_run_funcs.h
	$(CC) $(CFLAGS) -c first_run.c -o first_run.o

first_run_funcs.o: first_run_funcs.c first_run_funcs.h
	$(CC) $(CFLAGS) -c first_run_funcs.c -o first_run_funcs.o

second_run.o: second_run.c second_run.h helpful.h
	$(CC) $(CFLAGS) -c second_run.c -o second_run.o

symbol_table.o: symbol_table.c symbol_table.h helpful.h
	$(CC) $(CFLAGS) -c symbol_table.c -o symbol_table.o

# Clean up object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
