
// Queue Module


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
#include "decrypte.h"
#include "threadManager.h"
#include "Queue.h"

//-------------------------------------------------------------//
// -------------------------CONSTANTS------------------------- //
//-------------------------------------------------------------//

//#define FILETIME_UNITS_PER_MILLISECOND 10000 
//#define TIMEOUT_IN_MILLISECONDS 5000
//#define BRUTAL_TERMINATION_CODE 0x55


//-------------------------------------------------------------//
// ------------------FUNCTION DECLARATION--------------------- //
//-------------------------------------------------------------//


void StartQueue(QUEUE * queue)
{
    queue->number_of_tasks_in_queue = 0;
    queue->first_in_line = NULL;
    queue->last_in_line = NULL;
}

void Push(QUEUE * queue, int priority)
{
    NODE * temp_node;
    if (NULL == (temp_node = malloc(sizeof(NODE)))) return;
    temp_node->priority = priority;
    temp_node->behind_in_line = NULL;
    temp_node->ahead_in_line = NULL;

    if (Empty(queue))
    {
        queue->first_in_line = temp_node;
        queue->last_in_line = temp_node;
    }
    else
    {
        temp_node->ahead_in_line = queue->last_in_line;
        queue->last_in_line->behind_in_line = temp_node;
        queue->last_in_line = temp_node;
    }
}

int Empty(QUEUE * queue)
{
    return (queue->first_in_line == NULL);
}

NODE* Top(QUEUE* queue)
{
    return queue->first_in_line;
}

NODE* Pop(QUEUE* queue)
{
    NODE* temp_node;
    temp_node = queue->first_in_line;
    queue->first_in_line = queue->first_in_line->behind_in_line;
    queue->number_of_tasks_in_queue--;
    return temp_node;
}

void PrintQueue(QUEUE* queue)  //for debug
{
    int i = 0;
    NODE* temp_node = queue->first_in_line;

    printf("The number of tasks in the queue originally are: %d\n", queue->number_of_tasks_in_queue);

    for (i = 0; i < queue->number_of_tasks_in_queue; i++)
    {
        printf("the %d priority is task #%d\n", i + 1, temp_node->priority);
        temp_node = temp_node->behind_in_line;
    }
}

QUEUE * InitializeQueue(FILE* priority_file, QUEUE * queue, int number_of_tasks, char* priority_file_name)
{
    
    int curr_priority, i = 0;
    char curr_digit = '\0';

    if (NULL == (priority_file = open_file(priority_file, priority_file_name, "r"))) return queue;

    while ((curr_digit = fgetc(priority_file)) != EOF)
    {
        curr_priority = 0;
        while (curr_digit != '\n')
        {
            curr_priority = ((10 * curr_priority) + (curr_digit - '0'));
            curr_digit = fgetc(priority_file);
        }
        
        Push(queue, curr_priority);
        queue->number_of_tasks_in_queue++;
        i++;

        printf("%d\n", curr_priority); //for debug

        //curr_digit = fgetc(file);
        //curr_digit = fgetc(file);
    }

    if (0 != close_file(priority_file, priority_file_name)) return queue;
    return queue;
}

QUEUE* DestroyQueue(QUEUE* queue)
{
    NODE* temp_node;
    while (queue->number_of_tasks_in_queue > 0)
    {
        temp_node = queue->first_in_line;
        queue->first_in_line = queue->first_in_line->behind_in_line;
        free(temp_node);
        queue->number_of_tasks_in_queue--;
    }
    temp_node = NULL;
    return temp_node;
}