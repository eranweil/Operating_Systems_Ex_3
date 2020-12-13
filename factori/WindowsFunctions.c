//
////WindowsFunctions
//
////-------------------------------------------------------------//
//// ----------------------LIBRARY INCLUDES--------------------- //
////-------------------------------------------------------------//
//
//#include <Windows.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>
//#include <limits.h>
//#include <stdbool.h>
//
//
////-------------------------------------------------------------//
//// ----------------------PROJECT INCLUDES--------------------- //
////-------------------------------------------------------------//
//
//#include "HardCodedData.h"
//
//
////-------------------------------------------------------------//
//// -------------------------CONSTANTS------------------------- //
////-------------------------------------------------------------//
//
//#define FILETIME_UNITS_PER_MILLISECOND 10000 
//#define TIMEOUT_IN_MILLISECONDS 5000
//#define BRUTAL_TERMINATION_CODE 0x55
//
//
////-------------------------------------------------------------//
//// ------------------FUNCTION DECLARATION--------------------- //
////-------------------------------------------------------------//
//
///**
//* CreateProcessSimple uses the win32 API to create a process that runs the
//* command in 'CommandLine'. it uses the win32 API function CreateProcess()
//* using default values for most parameters.
//*
//* Accepts:
//* --------
//* CommandLine - a windows generic string containing the command that the new
//*               process performs. ( See CreateProcess( documentation for more ).
//* ProcessInfoPtr - an output parameter, used to return a PROCESS_INFORMATION
//*					structure containing data about the process that was created.
//*					( See CreateProcess() documentation for more ).
//*
//* Returns:
//* --------
//* the output of CreateProcess().
//*/
//BOOL CreateProcessSimple(LPTSTR CommandLine, PROCESS_INFORMATION* ProcessInfoPtr);
//static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
//	LPDWORD p_thread_id);
//
///*
//* IO thread. This thread is in charge of the user interface.
//* Notes:
//*   The thread calls the Echo function in a loop forever.
//*   Not all threads are an eternal loop. This is just for
//*   for the sake of this example.
//*/
//static DWORD WINAPI IOThread(LPVOID lpParam);
//
///*
//* Communication thread. This thread simulates a time consuming task.
//* Notes:
//*   This thread calls SendDiagnosticsToMicrosoft in a loop forever.
//*   Not all threads are an eternal loop. This is just for
//*   for the sake of this example.
//*/
//static DWORD WINAPI CommunicationThread(LPVOID lpParam);
//
///*
//* Prints back the string that has been inputed on the command line.
//*/
//void CreateProcessSimpleMain(void);
//
//
//
////-------------------------------------------------------------//
//// -------------------CREATION fUNCTIONS---------------------- //
////-------------------------------------------------------------//
//
//void CreateProcessSimpleMain(void)
//{
//	PROCESS_INFORMATION procinfo;
//	DWORD				waitcode;
//	DWORD				exitcode;
//	BOOL				retVal;
//	TCHAR				command[] = ("notepad.exe ../log.txt"); /* TCHAR is a win32
//													generic char which may be either a simple (ANSI) char or a unicode char,
//													depending on behind-the-scenes operating system definitions. Type LPTSTR
//													is a string of TCHARs. Type LPCTSTR is a const string of TCHARs. */
//
//													/*  Start the child process. */
//	retVal = CreateProcessSimple(command, &procinfo);
//
//
//	if (retVal == 0)
//	{
//		printf("Process Creation Failed!\n");
//		return;
//	}
//
//
//	waitcode = WaitForSingleObject(
//		procinfo.hProcess,
//		TIMEOUT_IN_MILLISECONDS); /* Waiting 5 secs for the process to end */
//
//	printf("WaitForSingleObject output: ");
//	switch (waitcode)
//	{
//	case WAIT_TIMEOUT:
//		printf("WAIT_TIMEOUT\n"); break;
//	case WAIT_OBJECT_0:
//		printf("WAIT_OBJECT_0\n"); break;
//	default:
//		printf("0x%x\n", waitcode);
//	}
//
//	if (waitcode == WAIT_TIMEOUT) /* Process is still alive */
//	{
//		printf("Process was not terminated before timeout!\n"
//			"Terminating brutally!\n");
//		TerminateProcess(
//			procinfo.hProcess,
//			BRUTAL_TERMINATION_CODE); /* Terminating process with an exit code of 55h */
//		Sleep(10); /* Waiting a few milliseconds for the process to terminate,
//					note the above command may also fail, so another WaitForSingleObject is required.
//					We skip this for brevity */
//	}
//
//	GetExitCodeProcess(procinfo.hProcess, &exitcode);
//
//	printf("The exit code for the process is 0x%x\n", exitcode);
//
//	/* Note: process is still being tracked by OS until we release handles */
//	CloseHandle(procinfo.hProcess); /* Closing the handle to the process */
//	CloseHandle(procinfo.hThread); /* Closing the handle to the main thread of the process */
//}
//
//BOOL CreateProcessSimple(LPTSTR CommandLine, PROCESS_INFORMATION* ProcessInfoPtr)
//{
//	STARTUPINFO	startinfo = { sizeof(STARTUPINFO), NULL, 0 }; /* <ISP> here we */
//															  /* initialize a "Neutral" STARTUPINFO variable. Supplying this to */
//															  /* CreateProcess() means we have no special interest in this parameter. */
//															  /* This is equivalent to what we are doing by supplying NULL to most other */
//															  /* parameters of CreateProcess(). */
//
//	return CreateProcess(
//		NULL, /*  No module name (use command line). */
//		CommandLine,			/*  Command line. */
//		NULL,					/*  Process handle not inheritable. */
//		NULL,					/*  Thread handle not inheritable. */
//		FALSE,					/*  Set handle inheritance to FALSE. */
//		NORMAL_PRIORITY_CLASS,	/*  creation/priority flags. */
//		NULL,					/*  Use parent's environment block. */
//		NULL,					/*  Use parent's starting directory. */
//		&startinfo,				/*  Pointer to STARTUPINFO structure. */
//		ProcessInfoPtr			/*  Pointer to PROCESS_INFORMATION structure. */
//	);
//}
//
//static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine, LPDWORD p_thread_id)
//{
//	/* Should check for NULL pointers. Skipped for the sake of simplicity. */
//
//	return CreateThread(
//		NULL,            /*  default security attributes */
//		0,               /*  use default stack size */
//		p_start_routine, /*  thread function */
//		NULL,            /*  argument to thread function */
//		0,               /*  use default creation flags */
//		p_thread_id);    /*  returns the thread identifier */
//}
//
//static DWORD WINAPI IOThread(LPVOID lpParam)
//{
//	while (1)
//	{
//		Echo();
//	}
//}
//
//static DWORD WINAPI CommunicationThread(LPVOID lpParam)
//{
//	while (1)
//	{
//		SendDiagnosticsToMicrosoft();
//	}
//}
//
//
//
//
////// Function Definitions --------------------------------------------------------
////
////int main()
////{
////	HANDLE p_thread_handles[NUM_THREADS];
////	DWORD p_thread_ids[NUM_THREADS];
////
////	// Create two threads, each thread performs on task.
////	p_thread_handles[0] = CreateThreadSimple(IOThread, &p_thread_ids[0]);
////	p_thread_handles[1] = CreateThreadSimple(CommunicationThread, &p_thread_ids[1]);
////
////	// The main is also a thread, so there are in fact 3 threads in this example.
////	// If the main thread returns, then all other threads are terminated by the OS.
////	// To prevent this, we never return from main.
////	// This is not the proper way to do things. Don't copy from this example!
////	// The next example is just this example written properly.
////	while (1)
////	{
////		// pass
////	}
////}
//
//
