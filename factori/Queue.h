#pragma once
/*
Queue.h
----------------------------------------------------------------------------
Queue module with all queue related actions header file
*/

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
#include "threadManager.h"

//-------------------------------------------------------------//
// ------------------FUNCTION DECLARATION--------------------- //
//-------------------------------------------------------------//

/*--------------------------------------------------------------------------------------------
DESCRIPTION - just initializing the queue elements to NULL in preparation for the actual queue initialization with nodes

PARAMETERS -    queue - pointer to the queue type element which all the threads are using

RETURN - void
    --------------------------------------------------------------------------------------------*/
void StartQueue(QUEUE * queue);

/*--------------------------------------------------------------------------------------------
DESCRIPTION - push an element into the queue

PARAMETERS -    queue - pointer to the queue type element which all the threads are using
                priority - tghe priority to be entered into the current queue

RETURN - void
    --------------------------------------------------------------------------------------------*/
void Push(QUEUE * queue, int priority);

/*--------------------------------------------------------------------------------------------
DESCRIPTION - check if queue is empty

PARAMETERS -    queue - pointer to the queue type element which all the threads are using

RETURN - '1' if empty and '0' if not
    --------------------------------------------------------------------------------------------*/
int Empty(QUEUE * queue);

/*--------------------------------------------------------------------------------------------
DESCRIPTION - return the top element without removing from queue

PARAMETERS -    queue - pointer to the queue type element which all the threads are using

RETURN - a pointer to a node element
    --------------------------------------------------------------------------------------------*/
NODE* Top(QUEUE* queue);

/*--------------------------------------------------------------------------------------------
DESCRIPTION - return the top element with removing from queue

PARAMETERS -    queue - pointer to the queue type element which all the threads are using

RETURN - a pointer to a node element
    --------------------------------------------------------------------------------------------*/
NODE* Pop(QUEUE* queue);

/*--------------------------------------------------------------------------------------------
DESCRIPTION - a debug function to print the priorities in the queue

PARAMETERS -    queue - pointer to the queue type element which all the threads are using

RETURN - void
    --------------------------------------------------------------------------------------------*/
void PrintQueue(QUEUE* queue);  //for debug

/*--------------------------------------------------------------------------------------------
DESCRIPTION - Initializing the queue

PARAMETERS -    priority_file - a HANDLE to the priority file
                queue - pointer to the queue type element which was created outside the function
                number_of_tasks - number of tasks to be pushed into the queue
                priority_file_name - The name of the priority file for the WINAPI read function


RETURN - a pointer to the initialized queue (or NULL if failed)
    --------------------------------------------------------------------------------------------*/
QUEUE* InitializeQueue(HANDLE priority_file, QUEUE* queue, int number_of_tasks, char* priority_file_name);

/*--------------------------------------------------------------------------------------------
DESCRIPTION - releasing all of the memory allocated for the queue

PARAMETERS -    queue - pointer to the queue type element which all the threads are using

RETURN - a NULL pointer to a node element
    --------------------------------------------------------------------------------------------*/
QUEUE* DestroyQueue(QUEUE* queue);