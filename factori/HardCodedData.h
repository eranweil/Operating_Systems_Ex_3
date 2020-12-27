/*
HardCodedData.h
----------------------------------------------------------------------------
Contains specific measured sizes and exit codes of the project. 
*/


//-------------------------------------------------------------//
// --------------------------DEFINES-------------------------- //
//-------------------------------------------------------------//
#pragma once
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Exit codes
#define SUCCESS_CODE							0 
#define STATUS_CODE_FAILURE					   -1 
#define ERROR_CODE_FILE						   -2
#define ERROR_CODE_WRITE					   -3
#define ERROR_CODE_ARGS						   -4
#define ERROR_CODE_ALLOCATION				   -5
#define ERROR_CODE_THREAD					   -6
#define ERROR_CODE_WSO_WAIT_ABANDONED		   -7
#define ERROR_CODE_WSO_WAIT_TIMEOUT			   -8
#define ERROR_CODE_WSO_WAIT_FAILED			   -9


// Bool
#define TRUE									1 
#define FALSE								    0

// Sizes
#define INITIAL_SIZE							10 //for line in the input file
#define ARGS_REQUIRED							4
#define THREAD_WAIT_TIME						30000

// Structures

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

struct lock
{
    int number_of_threads;
    int* p_reader_counter;
    HANDLE* p_mutex_file;
    HANDLE* p_sephamore_file;
    HANDLE* p_turnstile;
    HANDLE* p_queue_lock;
};
typedef struct lock LOCK;

struct Thread
{
    LOCK* p_lock;
    QUEUE* p_queue;
    int* p_number_of_tasks;
    HANDLE* start_line_sephamore;
    char* tasks_file_name;
    int thread_number;
};
typedef struct Thread THREAD;


