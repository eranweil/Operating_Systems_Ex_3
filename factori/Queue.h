
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


void StartQueue(QUEUE * queue);

void Push(QUEUE * queue, int priority);

int Empty(QUEUE * queue);

NODE* Top(QUEUE* queue);

NODE* Pop(QUEUE* queue);

void PrintQueue(QUEUE* queue);  //for debug

QUEUE* InitializeQueue(HANDLE priority_file, QUEUE* queue, int number_of_tasks, char* priority_file_name);

QUEUE* DestroyQueue(QUEUE* queue);