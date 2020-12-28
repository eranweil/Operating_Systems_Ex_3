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
#include "Queue.h"
#include "Lock.h"
#include "fileManager.h"
#include "HardCodedData.h"
#include "io.h"


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
int* break_into_primes(int n, int* p_prime_numbers, int* p_prime_numbers_size);

/*--------------------------------------------------------------------------------------------
DESCRIPTION - Calls a wait for single obj on the newly created thread. when done executes free_thread_and_data

PARAMETERS - hThread: handle to the thread
             pData: pointer to the data struct of that thread

RETURN - void
    --------------------------------------------------------------------------------------------*/
void wait_for_threads_execution_and_free(HANDLE* p_threads, int number_of_threads);

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
int dispatch_threads(HANDLE* p_threads, LOCK* p_lock, QUEUE* p_queue, int number_of_threads, int* p_number_of_tasks, HANDLE* start_line_sephamore, char* tasks_file_name);

/*--------------------------------------------------------------------------------------------
DESCRIPTION - Function every new thread is called to. reads, encrypte\decrypte and writes every line assigned to that thread.

PARAMETERS - lpParam: holds the data structure of pData for that thread

RETURN - signal exit code.
    --------------------------------------------------------------------------------------------*/
DWORD WINAPI thread_main(LPVOID lpParam);