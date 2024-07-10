/* Basic compilation command: */
CC = gcc # GCC Compiler
CFLAGS = -ansi -Wall -pedantic

/* Main: */
assembler.o: assembler.c
	$(CC) -c assembler.c &(CFLAGS) -o assembler

/* Code Helper Functions: */  #########for example#########
code_helper.o: code_helper.c code_helper.h
	$(CC) -c code_helper.c $(CFLAGS) -o code_helper

/* First Run: */
first_run.o: first_run.c first_run.h
	$(CC) -c first_run.c $(CFLAGS) -o first_run

/* Second Run: */
second_run.o: second_run.c second_run.h
	$(CC) -c second_run.c $(CFLAGS) -o second_run
