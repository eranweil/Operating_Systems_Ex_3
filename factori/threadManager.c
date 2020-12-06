
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

/*--------------------------------------------------------------------------------------------
DESCRIPTION - Counting the elements of an array (help function).

PARAMETERS - array to count it's elements.

RETURN - number of elements.
    --------------------------------------------------------------------------------------------*/
int count_elements(int* array);


/*--------------------------------------------------------------------------------------------
DESCRIPTION - Calls a wait for single obj on the newly created thread. when done executes free_thread_and_data

PARAMETERS - hThread: handle to the thread
             pData: pointer to the data struct of that thread

RETURN - void
    --------------------------------------------------------------------------------------------*/
void wait_for_thread_execution_and_free(HANDLE hThread, PTHREADDATA pData);


/*--------------------------------------------------------------------------------------------
DESCRIPTION - Closes the handle of the thread and HeadFree's it's data.

PARAMETERS - hThread: handle to the thread
             pData: pointer to the data struct of that thread

RETURN - void
    --------------------------------------------------------------------------------------------*/
void free_thread_and_data(HANDLE hthread, PTHREADDATA pdata);


/*--------------------------------------------------------------------------------------------
DESCRIPTION - Function every new thread is called to. reads, encrypte\decrypte and writes every line assigned to that thread.

PARAMETERS - lpParam: holds the data structure of pData for that thread

RETURN - signal exit code.
    --------------------------------------------------------------------------------------------*/
DWORD WINAPI thread_main(LPVOID lpParam);


/*--------------------------------------------------------------------------------------------
DESCRIPTION - Counts the rows and assigns parameters for every thread to be created. then creates threads one by one.

PARAMETERS - in_file: input text file
             out_file: output target file
             key: For thr Caesar system coding
             threads_required: Number of threads to assign. If there are mor threads than rows, the last ones will be created 
                               but do nothing.
             rows_endings: Hold the length of every linr in the text
             mode: e for encryption, d for decryption

RETURN - status code
    --------------------------------------------------------------------------------------------*/
int dispatch_threads(FILE* in_file, FILE* out_file, int key, int threads_required, int* rows_endings, char mode);

//---------------------------------------------------------------//
// ------------------------IMPLEMENTAIONS------------------------//
//---------------------------------------------------------------//
void free_thread_and_data(HANDLE hthread, PTHREADDATA pdata) {
    // closing handle and free pData struct
    CloseHandle(hthread);
    if (pdata != NULL)
    {
        HeapFree(GetProcessHeap(), 0, pdata);
        pdata = NULL;    // Ensure address is not reused.
    }
}


void wait_for_thread_execution_and_free(HANDLE hThread, PTHREADDATA pData) {
    // wait for the thread code
    DWORD dwWaitResult = WaitForSingleObject(
        hThread,    
        THREAD_WAIT_TIME); 
    free_thread_and_data(hThread, pData);
    if (0 != dwWaitResult) {
        // prints the error of the thread by the dwWaitResult code
        error_handler(dwWaitResult);
    }
}


DWORD WINAPI thread_main(LPVOID lpParam)
{
    PTHREADDATA thread_params = (PTHREADDATA)lpParam;
    int loop = 1, cur_row =0;
    char* line = NULL;

    // run as long as assigned lines are left
    while (cur_row != (thread_params->rows))
    {
        // read the line
        if (NULL == (line = read_line(thread_params->in_file, &loop))) {
            free(line);
            error_handler(ERROR_CODE_ALLOCATION);
        }
        else {
            // decrypte or encrypte the line
            if (NULL == (line = decrypte_line(line, thread_params->key, thread_params->mode))) {
                free(line);
                error_handler(ERROR_CODE_ALLOCATION);
            }
            else {
                //write the line
                if (0 != write_new_line_to_output(thread_params->out_file, line)) error_handler(ERROR_CODE_WRITE);
                cur_row++;
            }
        }
    }
    return SUCCESS_CODE;
}


int count_elements(int* array) {
    int i = 0;
    while (*(array + i) != '\0') {
        i++;
    }
    return i;
}


int dispatch_threads(FILE* in_file, FILE* out_file, int key, int threads_required, int* rows_endings, char mode) {
    float rows_remaining = (float)count_elements(rows_endings);
    int i = 0, threads = threads_required, thread_start, thread_end, busy=0;

    PTHREADDATA pData;
    DWORD   dwThreadId;
    HANDLE  hThread;

    // continue dispatching threads untill 0 threads are needed
    while (threads) {

        // algorithm to calaculate how many rows each thread is assigned
        int thread_rows = ceil(rows_remaining / threads);

        // first thread
        if (i == 0) {
            thread_start = 0;
            if ((fseek(in_file, (sizeof(char) * thread_start), SEEK_SET)) != 0) error_handler(ERROR_CODE_FILE);
        }
        else
        {
            thread_start = *(rows_endings + i - 1) + 1;
        }

        // i holds the number of line proccessed
        i += thread_rows;
        thread_end = *(rows_endings + i - 1);
        
        // allocate the struct of data to be passed to the thread
        pData = (PTHREADDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(THREADDATA));

        if (NULL == pData)
        {
            HeapFree(pData, 0, pData);
            error_handler(ERROR_CODE_FILE);
        }

        // assigning the data
            pData->key = key;
            pData->start = thread_start;
            pData->end = thread_end;
            pData->mode = mode;
            pData->rows = thread_rows;
            pData->in_file = in_file;
            pData->out_file = out_file;

        // safety lock
        while (busy) { ; }

        hThread = CreateThread(
            NULL,                   // default security attributes
            0,                      // use default stack size  
            thread_main,            // thread function name
            pData,                  // argument to thread function 
            0,                      // use default creation flags 
            &dwThreadId);           // returns the thread identifier 

        if (NULL == hThread) {
            free_thread_and_data(hThread, pData);
            error_handler(ERROR_CODE_THREAD);
        }
        // locking
        busy = 1;

        // escorting the thread
        wait_for_thread_execution_and_free(hThread, pData);

        // unlocking
        busy = 0;

        rows_remaining -= thread_rows;
        threads--;
    }
    return SUCCESS_CODE;
}