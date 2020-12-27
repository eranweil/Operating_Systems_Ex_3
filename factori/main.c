/*
main.c
----------------------------------------------------------------------------
AUTHORS - Itamar Eyal 302309539 and Eran Weil 204051353

PROJECT - Factori

DESCRIPTION - This program can find the prime number building blocks for a give number

              In cryptography, a Caesar cipher, also known as Caesar's cipher, the shift cipher,
              Caesar's code or Caesar shift, is one of the simplest and most widely known encryption techniques.
              It is a type of substitution cipher in which each letter in the plaintext is replaced by a letter
              some fixed number of positions down the alphabet. For example, with a left shift of 3, D would be
              replaced by A, E would become B, and so on. The method is named after Julius Caesar, who used it in
              his private correspondence. (from wikipedia: https://en.wikipedia.org/wiki/Caesar_cipher).

    consists of 3 modules:
        io - handles files and cmd line args
        threadManager - creates and operates threads.
        decrypte - utilizies the Caesar coding system to encrypte or decrypte text.

*/

//-------------------------------------------------------------//
// ----------------------LIBRARY INCLUDES--------------------- //
//-------------------------------------------------------------//

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


//-------------------------------------------------------------//
// ----------------------PROJECT INCLUDES--------------------- //
//-------------------------------------------------------------//

#include "HardCodedData.h"
#include "io.h"
#include "threadManager.h"
#include "fileManager.h"
#include "Queue.h"
#include "Lock.h"



int main(int argc, char** argv[])
{
    HANDLE priority_file = GetFile(argv[2], GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING);

    QUEUE queue;
    QUEUE* p_queue = &queue;

    LOCK lock;
    LOCK* p_lock = &lock;
    HANDLE* p_threads = NULL;
    HANDLE mutex_file, sephamore_file, turnstile, queue_lock, start_line_sephamore;
    int reader_counter = 0;
    int* p_reader_counter = &reader_counter;

    LPCTSTR sephamore_start_line = L"SephamoreStart";

    int number_of_threads = atoi(argv[4]);
    int number_of_tasks = atoi(argv[3]);

    int* p_number_of_tasks = &number_of_tasks;
    int* numbers_array = NULL;

    // Memory allocations for input args
    if (NULL == (numbers_array = (int*)malloc(number_of_tasks * sizeof(int)))) return ERROR_CODE_ALLOCATION;
    if (NULL == (p_threads = (HANDLE*)malloc(number_of_threads * sizeof(HANDLE)))) return ERROR_CODE_ALLOCATION;

    // Create queue
    StartQueue(p_queue);
    if (NULL == (p_queue = InitializeQueue(priority_file, p_queue, number_of_tasks, argv[2]))) return ERROR_CODE_ALLOCATION;
    PrintQueue(p_queue);
    printf("queue created\n");

    // Create the lock
    if (NULL == (p_lock = InitializeLock(p_lock, number_of_threads, p_reader_counter, &mutex_file, &sephamore_file, &turnstile, &queue_lock))) return ERROR_CODE_ALLOCATION;
    printf("lock created\n");

    // Create Sephamore to hold back all of the threads
    start_line_sephamore = CreateSemaphore(NULL, 0, number_of_threads, sephamore_start_line);
    printf("start line semaphore created\n");

    // Create the treads and give them tasks
    dispatch_threads(p_threads, p_lock, p_queue, number_of_threads, p_number_of_tasks, &start_line_sephamore, argv[1]);
    printf("all threads finished\n");

    // Now is the time to check for any misses and closes and frees
    CloseHandle(priority_file);
    CloseHandle(start_line_sephamore);
    free(numbers_array);
    free(p_threads);
    DestroyQueue(p_queue);
    DestroyLock(p_lock);

    return SUCCESS_CODE;
}