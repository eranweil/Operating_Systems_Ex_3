#pragma once

/*
io.h
----------------------------------------------------------------------------
Handeling input & output files, any function that get a ptr to a file.
Header for io.c
*/


//-------------------------------------------------------------//
// --------------------------INCLUDE-------------------------- //
//-------------------------------------------------------------//

#include <stdio.h>
#include <windows.h>
#include "HardCodedData.h"
#include "threadManager.h"
#include "decrypte.h"


//---------------------------------------------------------------//
// -------------------------DECLARATIONS------------------------ //
//---------------------------------------------------------------//

    /*--------------------------------------------------------------------------------------------
DESCRIPTION - Print an error msg to the console. msg includes an error code. Close files and exit program.

PARAMETERS - err_code: int representing the type of error. List in HardCodedData.h

RETURN - void
    --------------------------------------------------------------------------------------------*/
void error_handler(int err_code);

/*--------------------------------------------------------------------------------------------
DESCRIPTION - Checking validity of argv. argv must be of specific size and obey:
                argv[2] integer;
                argv[3] integer larger than 0;
                argv[4] str -d / -e only.
PARAMETERS - argc: length of argv[].
             argv: command argument vector.

RETURN - SUCCESS_CODE if correct, error code otherwise.
    --------------------------------------------------------------------------------------------*/
int check_arguments(int argc, char* argv[]);


/*--------------------------------------------------------------------------------------------
DESCRIPTION - Parses the argv given and writes it as parametes.

PARAMETERS - a_mode: pchar to hold e\d;
             threads: holds num of required threads.
             key: holds the key to encrypte/ decrypte.
             argv: command argument vector.

RETURN - void (writes the parameters to mem)
    --------------------------------------------------------------------------------------------*/
void parse_arguments(char* a_mode, int* threads, int* key, char* argv[]);


FILE* open_file(FILE* file, char* file_name, char* file_open_type);


int close_file(FILE* file, char* file_name);


/*--------------------------------------------------------------------------------------------
DESCRIPTION - Creates av array of ints. Every index hold the num of chars in that line including '\n'.

PARAMETERS - input file ptr.

RETURN - array of ints.
    --------------------------------------------------------------------------------------------*/
int* count_row_endings(FILE* file);


/*--------------------------------------------------------------------------------------------
DESCRIPTION - Reads a line from input text. writes 0 to loop when input file is reached EOF.

PARAMETERS - file: input file ptr.
             loop: flag for EOF.

RETURN - ptr to raw line buffer.
    --------------------------------------------------------------------------------------------*/
char* read_line(FILE* file, int* loop);


/*--------------------------------------------------------------------------------------------
DESCRIPTION - Write one line to output file.

PARAMETERS - output_file: output file ptr.
             line: the line to be written.

RETURN - success code if reached.
    --------------------------------------------------------------------------------------------*/
int write_new_line_to_output(FILE* output_file, char* line);


void print_exit_msg(char* in_file_name, char* out_file_name);