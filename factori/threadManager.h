#pragma once
/*
threadManager.h
----------------------------------------------------------------------------
Decryption or encryption of txt by key.
Header file for decrypte.c
*/

//-------------------------------------------------------------//
// --------------------------INCLUDE-------------------------- //
//-------------------------------------------------------------//

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "HardCodedData.h"
#include "io.h"
#include "decrypte.h"


//---------------------------------------------------------------//
// -------------------------DECLARATIONS------------------------ //
//---------------------------------------------------------------//

/*--------------------------------------------------------------------------------------------
DESCRIPTION - Struct holding parameters to pass on to a new created thread.

PARAMETERS - key: for Caesar encryption
             start: start point in file
             end: end point in file
             rows: number of row to read & write
             in_file: input file ptr
             out_file: output file ptr
    --------------------------------------------------------------------------------------------*/
typedef struct ThreadData {
    int key, start, end, rows;
    char mode;
    FILE* in_file;
    FILE* out_file;
} THREADDATA, * PTHREADDATA;

/*--------------------------------------------------------------------------------------------
DESCRIPTION - Counting the elements of an array (help function).

PARAMETERS - array to count it's elements.

RETURN - number of elements.
    --------------------------------------------------------------------------------------------*/
int count_elements(int* array);


/*--------------------------------------------------------------------------------------------
DESCRIPTION - Calls a wait for single obj on the newly created thread. when done executes free_thread_and_data

PARAMETERS - hThread: handle to the thread
             pData: pointer to the data struct of that thread

RETURN - void
    --------------------------------------------------------------------------------------------*/
void wait_for_thread_execution_and_free(HANDLE hThread, PTHREADDATA pData);


/*--------------------------------------------------------------------------------------------
DESCRIPTION - Closes the handle of the thread and HeadFree's it's data.

PARAMETERS - hThread: handle to the thread
             pData: pointer to the data struct of that thread

RETURN - void
    --------------------------------------------------------------------------------------------*/
void free_thread_and_data(HANDLE hthread, PTHREADDATA pdata);


/*--------------------------------------------------------------------------------------------
DESCRIPTION - Function every new thread is called to. reads, encrypte\decrypte and writes every line assigned to that thread.

PARAMETERS - lpParam: holds the data structure of pData for that thread

RETURN - signal exit code.
    --------------------------------------------------------------------------------------------*/
DWORD WINAPI thread_main(LPVOID lpParam);


/*--------------------------------------------------------------------------------------------
DESCRIPTION - Counts the rows and assigns parameters for every thread to be created. then creates threads one by one.

PARAMETERS - in_file: input text file
             out_file: output target file
             key: For thr Caesar system coding
             threads_required: Number of threads to assign. If there are mor threads than rows, the last ones will be created
                               but do nothing.
             rows_endings: Hold the length of every linr in the text
             mode: e for encryption, d for decryption

RETURN - status code
    --------------------------------------------------------------------------------------------*/
int dispatch_threads(FILE* in_file, FILE* out_file, int key, int threads_required, int* rows_endings, char mode);

