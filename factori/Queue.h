
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
#include "decrypte.h"
#include "threadManager.h"

//-------------------------------------------------------------//
// ------------------FUNCTION DECLARATION--------------------- //
//-------------------------------------------------------------//


struct node
{
    int priority;
    struct NODE* ahead_in_line;
    struct NODE* behind_in_line;
};
typedef struct node NODE;

struct Queue
{
    int number_of_tasks_in_queue;
    NODE* first_in_line;
    NODE* last_in_line;
};
typedef struct Queue QUEUE;



void StartQueue(QUEUE * queue);

void Push(QUEUE * queue, int priority);

int Empty(QUEUE * queue);

NODE* Top(QUEUE* queue);

NODE* Pop(QUEUE* queue);

void PrintQueue(QUEUE* queue);  //for debug

QUEUE * InitializeQueue(FILE* priority_file, QUEUE * queue, int number_of_tasks, char* priority_file_name);

QUEUE* DestroyQueue(QUEUE* queue);