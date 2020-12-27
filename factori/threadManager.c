
/*
threadManager.c
----------------------------------------------------------------------------
creates and dispatches threads.
*/

//-------------------------------------------------------------//
// --------------------------INCLUDE-------------------------- //
//-------------------------------------------------------------//

#include "threadManager.h"

//---------------------------------------------------------------//
// -------------------------DECLARATIONS------------------------ //
//---------------------------------------------------------------//


//---------------------------------------------------------------//
// ------------------------IMPLEMENTAIONS------------------------//
//---------------------------------------------------------------//

int* break_into_primes(int n, int* p_prime_numbers,int* secondary_prime_number_array, int* p_prime_numbers_size)
{
    int prime_numbers_index = *p_prime_numbers_size, prime_number = n;
    int i = 3;
    if (NULL == (p_prime_numbers = (int*)malloc(p_prime_numbers, prime_numbers_index * sizeof(int)))) return p_prime_numbers;

    while ((prime_number % 2) == 0)
    {
        if (NULL == (secondary_prime_number_array = (int*)realloc(p_prime_numbers, prime_numbers_index * sizeof(int)))) return p_prime_numbers;
        *(p_prime_numbers + prime_numbers_index - 1) = 2;
        prime_number = prime_number / 2;
        printf("Number %d of prime %d is %d\n", prime_numbers_index, n, 2);
        p_prime_numbers = secondary_prime_number_array;
        prime_numbers_index++;
    }

    while (i <= sqrt(prime_number))
    {
        while ((prime_number % i) == 0)
        {
            if (NULL == (secondary_prime_number_array = (int*)realloc(p_prime_numbers, prime_numbers_index * sizeof(int)))) return p_prime_numbers;
            *(p_prime_numbers + prime_numbers_index - 1) = i;
            prime_number = prime_number / i;
            printf("Number %d of prime %d is %d\n", prime_numbers_index, n, i);
            prime_numbers_index++;
            p_prime_numbers = secondary_prime_number_array;
        }
        i = i + 2;
    }

    if (prime_number > 2)
    {
        if (NULL == (secondary_prime_number_array = (int*)realloc(p_prime_numbers, prime_numbers_index * sizeof(int)))) return p_prime_numbers;
        *(p_prime_numbers + prime_numbers_index - 1) = prime_number;
        printf("Number %d of prime %d is %d\n", prime_numbers_index, n, prime_number);
        p_prime_numbers = secondary_prime_number_array;
    }
    else prime_numbers_index--;
    *p_prime_numbers_size = prime_numbers_index;
    return p_prime_numbers;
}

void wait_for_threads_execution_and_free(HANDLE* p_threads, int number_of_threads)
{
    int i = 0;
    DWORD dwEvent;

    dwEvent = WaitForMultipleObjects(
        number_of_threads,                                          // number of objects in array
        p_threads,                                                  // array of objects
        TRUE,                                                       // wait for all object
        10 * number_of_threads * THREAD_WAIT_TIME);                 // long wait period for all threads
    
    // Close event handles
    for (i = 0; i < number_of_threads; i++)
        CloseHandle(*(p_threads + i));
    printf("freed Thread number: %d\n", i + 1);

}

int dispatch_threads(HANDLE* p_threads, LOCK* p_lock, QUEUE* p_queue, int number_of_threads, int* p_number_of_tasks, HANDLE* p_start_line_sephamore, char* tasks_file_name)
{
    int i = 0, j = 0;

    THREAD Data;
    THREAD* pData = &Data;
    DWORD dwThreadId;


    pData->p_lock = p_lock;
    pData->p_queue = p_queue;
    pData->p_number_of_tasks = p_number_of_tasks;
    pData->start_line_sephamore = p_start_line_sephamore;
    pData->tasks_file_name = tasks_file_name;

    // continue dispatching threads until 0 threads are needed
    for (i = 0; i < number_of_threads; i++)
    {
        pData->thread_number = i + 1;

        // create thread
        *(p_threads + i) = CreateThread(
            NULL,                                           // default security attributes
            0,                                              // use default stack size  
            thread_main,                                    // thread function name
            pData,                                          // argument to thread function 
            0,                                              // use default creation flags 
            &dwThreadId);                                   // returns the thread identifier 

        if (NULL == *(p_threads + i))
        {
            CloseHandle(p_threads);
            error_handler(ERROR_CODE_THREAD);
        }
        printf("dispatched Thread number: %d\n", i + 1);

    }

    // Release start line Sephamore
    ReleaseSemaphore(*p_start_line_sephamore, number_of_threads, NULL);
    printf("start line semaphore released\n");

    // escorting the thread
    wait_for_threads_execution_and_free(p_threads, number_of_threads);
    printf("freed all threads\n");

    return SUCCESS_CODE;
}

DWORD WINAPI thread_main(LPVOID lpParam)
{
    THREAD* thread_params = (THREAD*)lpParam;
    int curr_task_num = 0, n = 0, number_of_digits = 0, prime_numbers_size = 1, thread_num = thread_params->thread_number;
    int* p_number_of_digits = &number_of_digits, p_prime_numbers, secondary_prime_number_array, p_prime_numbers_size = &prime_numbers_size;
    char* line = NULL;

    // Wait at the starting line for all threads to be created
    WaitForSingleObject(*(thread_params->start_line_sephamore), thread_params->p_lock->number_of_threads * THREAD_WAIT_TIME);

    // Run as long as there are more tasks left
    while (*(thread_params->p_number_of_tasks))
    {
        p_prime_numbers = NULL;
        secondary_prime_number_array = NULL;

        // Get the next prioritized task
        queue_lock(thread_params->p_lock);
        curr_task_num = Pop(thread_params->p_queue)->priority;
        *(thread_params->p_number_of_tasks) -= 1;
        queue_release(thread_params->p_lock);
        printf("Thread %d finished with the queue\n", thread_num);

        // Get the task
        read_lock(thread_params->p_lock);
        n = GetTask(thread_params->tasks_file_name, curr_task_num, p_number_of_digits);
        read_release(thread_params->p_lock);
        printf("Thread %d finished reading task number: %d\n", thread_num, *(thread_params->p_number_of_tasks));

        // Calculate the prime numbers
        if (NULL == (p_prime_numbers = break_into_primes(n, p_prime_numbers, secondary_prime_number_array, p_prime_numbers_size))) return p_prime_numbers;
        printf("Thread %d broke into primes\n", thread_num);

        // Write to the file
        write_lock(thread_params->p_lock);
        print_primes_to_file(thread_params->tasks_file_name, p_prime_numbers, prime_numbers_size, n, number_of_digits);
        p_prime_numbers = NULL;
        prime_numbers_size = 1;
        write_release(thread_params->p_lock);
        *p_number_of_digits = 0;
        printf("Thread %d finished writing task number: %d\n", thread_num, *(thread_params->p_number_of_tasks));
        free(p_prime_numbers);
        free(secondary_prime_number_array);
    }

    return SUCCESS_CODE;
}