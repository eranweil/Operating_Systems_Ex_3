/*
main.c
----------------------------------------------------------------------------
AUTHORS - Itamar Eyal 302309539 and Eran Weil 204051353

PROJECT - Factori

DESCRIPTION - This program can find the prime number building blocks for all of the numbers given in a task file by the user


    consists of 4 main modules:
        fileManager - handles files and cmd line args
        threadManager - creates and handles threads
        Lock - A module for all of the lock specific actions, comprising mostly of sync object actions
        Queue - A module for all of the queue specific actions, comprising mostly of linked list actions
*/

//-------------------------------------------------------------//
// ----------------------LIBRARY INCLUDES--------------------- //
//-------------------------------------------------------------//

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>

//-------------------------------------------------------------//
// ----------------------PROJECT INCLUDES--------------------- //
//-------------------------------------------------------------//

#include "HardCodedData.h"
#include "threadManager.h"
#include "fileManager.h"
#include "Queue.h"
#include "Lock.h"



int main(int argc, char** argv[])
{
    QUEUE queue;
    QUEUE* p_queue = &queue;

    LOCK lock;
    LOCK* p_lock = &lock;

    HANDLE priority_file = GetFile(argv[2], GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING);
    HANDLE mutex_file, sephamore_file, turnstile, queue_lock, start_line_sephamore;
    HANDLE* p_threads = NULL;
    
    int i = 0, reader_counter = 0;
    int* p_reader_counter = &reader_counter;

    LPCTSTR sephamore_start_line = L"SephamoreStart";

    int number_of_threads = atoi(argv[4]);
    int number_of_tasks = atoi(argv[3]);

    int* p_number_of_tasks = &number_of_tasks;
    int* numbers_array = NULL;

    // Check args given
    if (STATUS_CODE_FAILURE == check_arguments(argc, argv)) return ERROR_CODE_ARGS;

    // Memory allocations for input args
    if (NULL == (numbers_array = (int*)malloc(number_of_tasks * sizeof(int)))) return ERROR_CODE_ALLOCATION;
    if (NULL == (p_threads = (HANDLE*)malloc(number_of_threads * sizeof(HANDLE)))) return ERROR_CODE_ALLOCATION;

    // Create queue
    StartQueue(p_queue);
    if (NULL == (p_queue = InitializeQueue(priority_file, p_queue, number_of_tasks, argv[2])))
    {
        printf("There was trouble initializing the queue");
        return ERROR_CODE_ALLOCATION;
    }
    printf("The queue was created\n");

    // Create the lock
    if (NULL == (p_lock = InitializeLock(p_lock, number_of_threads, p_reader_counter, &mutex_file, &sephamore_file, &turnstile, &queue_lock))) 
    {
        printf("There was trouble initializing the queue");
        return ERROR_CODE_ALLOCATION;
    }
    printf("The lock was created\n");

    // Create Sephamore to hold back all of the threads
    if (NULL == (start_line_sephamore = CreateSemaphore(NULL, 0, number_of_threads, sephamore_start_line)))
    {
        printf("Start line semaphore was not created\n");
        return ERROR_CODE_HANDLE;
    }
    printf("start line semaphore created\n");

    // Create the treads and give them tasks
    if (STATUS_CODE_FAILURE == (dispatch_threads(p_threads, p_lock, p_queue, number_of_threads, p_number_of_tasks, &start_line_sephamore, argv[1])))
    {
        printf("There was a problem dispatching threads\n");
        return ERROR_CODE_THREAD;
    }
    printf("all threads finished\n");

    // Get rid of all left over memory and handles
    CloseHandle(priority_file);
    CloseHandle(start_line_sephamore);
    free(numbers_array);
    free(p_threads);
    DestroyQueue(p_queue);
    DestroyLock(p_lock);

    return SUCCESS_CODE;
}