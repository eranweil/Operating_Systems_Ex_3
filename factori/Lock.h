
// Queue Header file


//-------------------------------------------------------------//
// ----------------------LIBRARY INCLUDES--------------------- //
//-------------------------------------------------------------//

#pragma once
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

//-------------------------------------------------------------//
// ------------------FUNCTION DECLARATION--------------------- //
//-------------------------------------------------------------//


LOCK* InitializeLock(LOCK* lock, int number_of_threads, int* p_reader_counter, HANDLE* p_mutex_file, HANDLE* p_sephamore_file, HANDLE* p_turnstile, HANDLE* p_queue_lock);

void read_lock(LOCK* lock);

void read_release(LOCK* lock);

void write_lock(LOCK* lock);

void write_release(LOCK* lock);

void queue_lock(LOCK* lock);

BOOL queue_release(LOCK* lock);

LOCK* DestroyLock(LOCK* lock);