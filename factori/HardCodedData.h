/*
HardCodedData.h
----------------------------------------------------------------------------
Contains specific measured sizes and exit codes of the project. 
*/


//-------------------------------------------------------------//
// --------------------------DEFINES-------------------------- //
//-------------------------------------------------------------//

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


// Sizes
#define INITIAL_SIZE							10 //for line in the input file
#define ARGS_REQUIRED							5
#define THREAD_WAIT_TIME						20000
