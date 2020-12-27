
// Lock Module

//-------------------------------------------------------------//
// ----------------------LIBRARY INCLUDES--------------------- //
//-------------------------------------------------------------//

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <stdbool.h>


//-------------------------------------------------------------//
// ----------------------PROJECT INCLUDES--------------------- //
//-------------------------------------------------------------//

#include "HardCodedData.h"
#include "io.h"
#include "threadManager.h"
#include "Queue.h"
#include "Lock.h"

//-------------------------------------------------------------//
// -------------------------CONSTANTS------------------------- //
//-------------------------------------------------------------//

//#define FILETIME_UNITS_PER_MILLISECOND 10000 
//#define TIMEOUT_IN_MILLISECONDS 5000
//#define BRUTAL_TERMINATION_CODE 0x55


//-------------------------------------------------------------//
// ------------------FUNCTION DECLARATION--------------------- //
//-------------------------------------------------------------//

LOCK* InitializeLock(LOCK* lock, int number_of_threads, int* p_reader_counter, HANDLE* p_mutex_file, HANDLE* p_sephamore_file, HANDLE* p_turnstile, HANDLE* p_queue_lock)
{
    *p_mutex_file = CreateMutex(NULL, FALSE, NULL);
    if (p_mutex_file == NULL) return lock;
    *p_sephamore_file = CreateSemaphore(NULL, 1, (LONG)number_of_threads, NULL);
    if (p_sephamore_file == NULL) return lock;
    *p_turnstile = CreateMutex(NULL, FALSE, NULL);
    if (p_turnstile == NULL) return lock;
    *p_queue_lock = CreateMutex(NULL, FALSE, NULL);
    if (p_queue_lock == NULL) return lock;

    lock->number_of_threads = number_of_threads;
    lock->p_reader_counter = p_reader_counter;
    lock->p_mutex_file = p_mutex_file;
    lock->p_sephamore_file = p_sephamore_file;
    lock->p_turnstile = p_turnstile;
    lock->p_queue_lock = p_queue_lock;

    return lock;
}

void read_lock(LOCK* lock)
{
    DWORD wait_res;
    BOOL release_res;

    //Turnstile
    if (WAIT_OBJECT_0 != (wait_res = WaitForSingleObject(*(lock->p_turnstile), lock->number_of_threads * THREAD_WAIT_TIME))) return lock;
    if (FALSE == (release_res = ReleaseMutex(*(lock->p_turnstile)))) return lock;

    //Mutex
    if (WAIT_OBJECT_0 != (wait_res = WaitForSingleObject(*(lock->p_mutex_file), lock->number_of_threads * THREAD_WAIT_TIME))) return lock;
    *lock->p_reader_counter += 1;
    if (*lock->p_reader_counter == 1)
    {   if (WAIT_OBJECT_0 != (wait_res = WaitForSingleObject(*lock->p_sephamore_file, lock->number_of_threads * THREAD_WAIT_TIME))) return lock;    }
    if (FALSE == (release_res = ReleaseMutex(*(lock->p_mutex_file)))) return lock;
}

void read_release(LOCK* lock)
{
    DWORD wait_res;
    BOOL release_res;

    //Mutex
    if (WAIT_OBJECT_0 != (wait_res = WaitForSingleObject(*(lock->p_mutex_file), lock->number_of_threads * THREAD_WAIT_TIME))) return lock;
    *lock->p_reader_counter -= 1;
    if (0 == *lock->p_reader_counter)
    {   if (FALSE == (release_res = ReleaseSemaphore(*(lock->p_sephamore_file), 1, NULL))) return lock;  }
    if (FALSE == (release_res = ReleaseMutex(*(lock->p_mutex_file)))) return lock;
}

void write_lock(LOCK* lock)
{
    DWORD wait_res;
    BOOL release_res;

    //Turnstile
    if (WAIT_OBJECT_0 != (wait_res = WaitForSingleObject(*(lock->p_turnstile), lock->number_of_threads * THREAD_WAIT_TIME))) return lock;
    
    //Semaphore
    if (WAIT_OBJECT_0 != (wait_res = WaitForSingleObject(*(lock->p_sephamore_file), lock->number_of_threads * THREAD_WAIT_TIME))) return lock;

}

void write_release(LOCK* lock)
{
    BOOL release_res;
    if (FALSE == (release_res = ReleaseMutex(*(lock->p_turnstile)))) return lock;
    if (FALSE == (release_res = ReleaseSemaphore(*(lock->p_sephamore_file), 1, NULL))) return lock;
}

void queue_lock(LOCK* lock)
{
    DWORD wait_res;
    if (WAIT_OBJECT_0 != (wait_res = WaitForSingleObject(*(lock->p_queue_lock), lock->number_of_threads * THREAD_WAIT_TIME))) return lock;
}

BOOL queue_release(LOCK* lock)
{
    BOOL release_res;
    if (FALSE != (release_res = ReleaseMutex(*(lock->p_queue_lock)))) return lock;
}

LOCK* DestroyLock(LOCK* lock)
{
    CloseHandle(*lock->p_mutex_file);
    CloseHandle(*lock->p_sephamore_file);
    CloseHandle(*lock->p_turnstile);
    CloseHandle(*lock->p_queue_lock);
    return (lock = NULL);
}



