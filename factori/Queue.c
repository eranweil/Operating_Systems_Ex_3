
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

QUEUE * InitializeQueue(HANDLE priority_file, QUEUE * queue, int number_of_tasks, char* priority_file_name)
{
    
    int curr_priority, i = 0;
    char* curr_char = NULL;
    if (NULL == (curr_char = (char*)malloc(sizeof(char))))
    {
        printf("Memomry allocation failed\n");
        return queue;
    }

    // Make sure file read works
    if (FALSE == ReadFile(priority_file, curr_char, 1, NULL, NULL))
    {
        printf("couldn't open file to be read in WINAPI\n");
        return ERROR_CODE_FILE;
    }

    //Stop at end of file
    for (i = 0; i < number_of_tasks; i++)
    {
        curr_priority = 0;
        //Stop at end of data in line
        if (i != number_of_tasks - 1)
        {
            while (*curr_char != '\r')
            {
                curr_priority = ((10 * curr_priority) + (*curr_char - '0'));
                if (FALSE == ReadFile(priority_file, curr_char, 1, NULL, NULL))
                {
                    printf("couldn't open file to be read in WINAPI\n");
                    return ERROR_CODE_FILE;
                }
            }
        }
        if (i == number_of_tasks - 1)
        {
            while ((*curr_char != EOF) && (*curr_char != '\r'))
            {
                curr_priority = ((10 * curr_priority) + (*curr_char - '0'));
                if (FALSE == ReadFile(priority_file, curr_char, 1, NULL, NULL))
                {
                    printf("couldn't open file to be read in WINAPI\n");
                    return ERROR_CODE_FILE;
                }
            }
        }
        // In order to go to next line

        if (FALSE == ReadFile(priority_file, curr_char, 1, NULL, NULL))
        {
            printf("couldn't open file to be read in WINAPI\n");
            return ERROR_CODE_FILE;
        }

        if (FALSE == ReadFile(priority_file, curr_char, 1, NULL, NULL))
        {
            printf("couldn't open file to be read in WINAPI\n");
            return ERROR_CODE_FILE;
        }

        //Push the priority into the queue
        Push(queue, curr_priority);
        queue->number_of_tasks_in_queue++;
        printf("%d\n", curr_priority); //for debug
    }
    PrintQueue(queue);
    free(curr_char);
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