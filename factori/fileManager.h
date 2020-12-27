#pragma once
/*
fileManager.h
----------------------------------------------------------------------------
Decryption or encryption of txt by key.
Header file for decrypte.c
*/

//-------------------------------------------------------------//
// --------------------------INCLUDE-------------------------- //
//-------------------------------------------------------------//

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Queue.h"
#include "Lock.h"
#include "HardCodedData.h"
#include "io.h"

//---------------------------------------------------------------//
// -------------------------DECLARATIONS------------------------ //
//---------------------------------------------------------------//

///*--------------------------------------------------------------------------------------------
//DESCRIPTION - print an output to the end of tasks file.
//PARAMETERS - primearray: array of the prime factors
//             arraylen: a pointer to len of primearray
//             n: the given number
//             output_file: tasks file handle
//RETURN - zero if succeded o.w -2
//    --------------------------------------------------------------------------------------------*/
//
//int PrintFile(int* primearray, int* arraylen, int n, HANDLE output_file);
//
///*--------------------------------------------------------------------------------------------
//DESCRIPTION - write str types to output file.
//PARAMETERS - output_file: handle to output file
//            line: the string to print
//RETURN - zero if succeded o.w -2
//    --------------------------------------------------------------------------------------------*/
//
//int StringFile(HANDLE output_file, char* line);
//
///*--------------------------------------------------------------------------------------------
//DESCRIPTION - write int types to output file.
//PARAMETERS - output_file: handle to output file
//            line: the int to print
//RETURN - zero if succeded o.w -2
//    --------------------------------------------------------------------------------------------*/
//
//
//int IntFile(HANDLE output_file, int n);
//
int CountDigits(int number);

int GetTask(char* tasks_file_name, int curr_task_num, int* number_of_digits);

int print_primes_to_file(char* tasks_file_name, int* prime_numbers, int prime_numbers_size, int n, int number_of_digits);

int StringToFileWithCheck(HANDLE tasks_file, char* string, int string_len);

HANDLE GetFile(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwCreationDisposition);


