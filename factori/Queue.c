
/*
Queue.c
----------------------------------------------------------------------------
Queue module with all queue related actions
*/

//-------------------------------------------------------------//
// --------------------------INCLUDE-------------------------- //
//-------------------------------------------------------------//

#include "Queue.h"

//---------------------------------------------------------------//
// -------------------------DECLARATIONS------------------------ //
//---------------------------------------------------------------//

/*--------------------------------------------------------------------------------------------
DESCRIPTION - just initializing the queue elements to NULL in preparation for the actual queue initialization with nodes

PARAMETERS -    queue - pointer to the queue type element which all the threads are using

RETURN - void
    --------------------------------------------------------------------------------------------*/
void StartQueue(QUEUE* queue);

/*--------------------------------------------------------------------------------------------
DESCRIPTION - push an element into the queue

PARAMETERS -    queue - pointer to the queue type element which all the threads are using
                priority - tghe priority to be entered into the current queue

RETURN - void
    --------------------------------------------------------------------------------------------*/
void Push(QUEUE* queue, int priority);

/*--------------------------------------------------------------------------------------------
DESCRIPTION - check if queue is empty

PARAMETERS -    queue - pointer to the queue type element which all the threads are using

RETURN - '1' if empty and '0' if not
    --------------------------------------------------------------------------------------------*/
int Empty(QUEUE* queue);

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

//---------------------------------------------------------------//
// ------------------------IMPLEMENTAIONS------------------------//
//---------------------------------------------------------------//


void StartQueue(QUEUE * queue)
{
    queue->number_of_tasks_in_queue = 0;
    queue->first_in_line = NULL;
    queue->last_in_line = NULL;
}

void Push(QUEUE * queue, int priority)
{
    NODE * temp_node;
    if (NULL == (temp_node = malloc(sizeof(NODE))))
    {
        printf("There's something wrong with pushing into the queue\n");
        return;
    }
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
        printf("the number %d priority is the task  at byte number %d\n", i + 1, temp_node->priority);
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

    // Get first file
    if (FALSE == ReadFile(priority_file, curr_char, 1, NULL, NULL))
    {
        printf("couldn't open file to be read in WINAPI\n");
        CloseHandle(priority_file);
        return NULL;
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
                    CloseHandle(priority_file);
                    return NULL;
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
                    CloseHandle(priority_file);
                    return NULL;
                }
            }
        }
        // In order to go to next line

        if (FALSE == ReadFile(priority_file, curr_char, 1, NULL, NULL))
        {
            printf("couldn't open file to be read in WINAPI\n");
            CloseHandle(priority_file);
            return NULL;
        }

        if (FALSE == ReadFile(priority_file, curr_char, 1, NULL, NULL))
        {
            printf("couldn't open file to be read in WINAPI\n");
            CloseHandle(priority_file);
            return NULL;
        }

        //Push the priority into the queue
        Push(queue, curr_priority);
        queue->number_of_tasks_in_queue++;
    }
    printf("The queue is as follows:\n");
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