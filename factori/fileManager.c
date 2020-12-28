
/*
threadManager.c
----------------------------------------------------------------------------
creates and dispatches threads.
*/

//-------------------------------------------------------------//
// --------------------------INCLUDE-------------------------- //
//-------------------------------------------------------------//

#include "fileManager.h"

//---------------------------------------------------------------//
// -------------------------DECLARATIONS------------------------ //
//---------------------------------------------------------------//


//---------------------------------------------------------------//
// ------------------------IMPLEMENTAIONS------------------------//
int CountDigits(int number)
{
	int counter = 0;
	while (number != 0)
	{
		number = number / 10;
		counter++;
	}
	return counter;
}

int GetTask(char* tasks_file_name, int curr_task_num, int* p_number_of_digits)
{
	int n = 0, i = 0;
	char* curr_char = NULL;
	HANDLE tasks_file = GetFile(tasks_file_name, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, OPEN_EXISTING);
	if (NULL == (curr_char = (char*)malloc(sizeof(char))))
	{
		printf("Memomry allocation failed\n");
		return STATUS_CODE_FAILURE;
	}

	// Move to desired offset in file
	DWORD dwPtr = SetFilePointer(
		tasks_file,
		curr_task_num,
		NULL,
		FILE_BEGIN);

	// Open file with check
	if (FALSE == ReadFile(tasks_file, curr_char, 1, NULL, NULL))
	{
		printf("couldn't open file to be read in WINAPI\n");
		return ERROR_CODE_FILE;
	}

	// Get the full number
	while (*curr_char != '\r')
	{
		n = ((10 * n) + (*curr_char - '0'));
		*p_number_of_digits += 1;

		if (FALSE == ReadFile(tasks_file, curr_char, 1, NULL, NULL))
		{
			printf("couldn't open file to be read in WINAPI\n");
			return ERROR_CODE_FILE;
		}
	}

	free(curr_char);
	CloseHandle(tasks_file);
	return n;
}

char* MakePrimeString(char* string_prime, char* string_buffer, int prime_numbers_size, int number_of_digits, int* prime_numbers)
{
	int i = 0, j = 0, k = 0, curr_prime_number, prime_string_length = 0, prime_number_length = 0;

	// it's enough to allocate the number of digits for each prime, multiplied by 3 for the commas and spaces. +1 for the '\0' symbol
	if (NULL == (string_prime = (char*)malloc((((number_of_digits * prime_numbers_size) * 30) + 1) * sizeof(char)))) return ERROR_CODE_ALLOCATION;

	// buffer for each string
	if (NULL == (string_buffer = (char*)malloc((20 * number_of_digits) * sizeof(char)))) return ERROR_CODE_ALLOCATION;

	for (i = 0; i < prime_numbers_size; i++)
	{
		if (i != prime_numbers_size - 1)
		{
			curr_prime_number = *(prime_numbers + i);
			prime_number_length = CountDigits(curr_prime_number);
			for (j = prime_number_length - 1; j >= 0; j--)
			{
				*(string_buffer + j) = (curr_prime_number % 10) + '0';
				curr_prime_number = curr_prime_number / 10;
			}
			*(string_buffer + prime_number_length) = '\0';

			for (k = 0; k < strlen(string_buffer); k++)
			{
				*(string_prime + prime_string_length) = *(string_buffer + k);
				prime_string_length++;
			}
			*(string_prime + prime_string_length) = ',';
			prime_string_length++;
			*(string_prime + prime_string_length) = ' ';
			prime_string_length++;
		}

		if (i == prime_numbers_size - 1)
		{
			curr_prime_number = *(prime_numbers + i);
			prime_number_length = CountDigits(curr_prime_number);
			for (j = prime_number_length - 1; j >= 0; j--)
			{
				*(string_buffer + j) = curr_prime_number % 10 + '0';
				curr_prime_number = curr_prime_number / 10;
			}
			*(string_buffer + prime_number_length) = '\0';

			for (k = 0; k < strlen(string_buffer); k++)
			{
				*(string_prime + prime_string_length) = *(string_buffer + k);
				prime_string_length++;
			}
			*(string_prime + prime_string_length) = '\r';
			prime_string_length++;
			*(string_prime + prime_string_length) = '\n';
			prime_string_length++;
		}
	}
	*(string_prime + prime_string_length) = '\0';

	return string_prime;
}

int print_primes_to_file(char* tasks_file_name, int* prime_numbers, int prime_numbers_size, int n, int number_of_digits)
{
	char* string_1 = "The prime factors of ";
	char* string_2 = NULL;
	char* string_3 = " are: ";
	char* string_4 = ", ";
	char* string_end_of_line = "\r\n";
	char* string_prime = NULL;
	char* string_buffer = NULL;
	size_t size_number_of_digits = number_of_digits + 1;
	rsize_t size_in_bytes = 0;

	//Build the number string
	if (NULL == (string_2 = (char*)malloc((number_of_digits + 1) * sizeof(char)))) return ERROR_CODE_ALLOCATION;
	_itoa_s(n, string_2, size_number_of_digits, 10);

	// Build the prime string
	string_prime = MakePrimeString(string_prime, string_buffer, prime_numbers_size, number_of_digits, prime_numbers);

	int string_len_1 = strlen(string_1);
	int string_len_2 = strlen(string_2);
	int string_len_3 = strlen(string_3);
	int string_len_prime = strlen(string_prime);

	HANDLE tasks_file = GetFile(tasks_file_name, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, OPEN_EXISTING);

	// write string 1
	if (STATUS_CODE_FAILURE == StringToFileWithCheck(tasks_file, string_1, string_len_1)) return STATUS_CODE_FAILURE;

	// write string 2
	if (STATUS_CODE_FAILURE == StringToFileWithCheck(tasks_file, string_2, string_len_2))
	{
		free(string_2);
		free(string_buffer);
		free(string_prime);
		return STATUS_CODE_FAILURE;
	};

	// write string 3
	if (STATUS_CODE_FAILURE == StringToFileWithCheck(tasks_file, string_3, string_len_3)) return STATUS_CODE_FAILURE;

	// write prime numbers
	if (STATUS_CODE_FAILURE == StringToFileWithCheck(tasks_file, string_prime, string_len_prime))
	{
		free(string_2);
		free(string_buffer);
		free(string_prime);
		return STATUS_CODE_FAILURE;
	}

	//Success
	CloseHandle(tasks_file);
	free(string_2);
	free(string_buffer);
	free(string_prime);
	return SUCCESS_CODE;
}

int StringToFileWithCheck(HANDLE tasks_file, char* string, int string_len)
{
	DWORD dwPtr = SetFilePointer(tasks_file, 0, NULL, FILE_END);

	if (FALSE == WriteFile(tasks_file, string, string_len, NULL, NULL))
	{
		printf("couldn't open file to be write in WINAPI\n");
		CloseHandle(tasks_file);
		return STATUS_CODE_FAILURE;
	}
	return SUCCESS_CODE;
}

HANDLE GetFile(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwCreationDisposition) {
	HANDLE file_handle;

	if (NULL == lpFileName || 0 == dwDesiredAccess || 0 == dwShareMode)
	{
		printf("Received null pointer");
		return NULL;
	}

	file_handle = CreateFileA(
		lpFileName,				//The name of the file or device to be created or opened.
		dwDesiredAccess,        //The requested access to the file or device, which can be summarized 
								//as read, write, both or neither zero).
								//The most commonly used values are 
								//GENERIC_READ, GENERIC_WRITE, or both(GENERIC_READ | GENERIC_WRITE).
		dwShareMode,            //The requested sharing mode of the file or device, which can be read, 
								//write, both, delete, all of these, or none 
								//FILE_SHARE_READ 1 OR FILE_SHARE_WRITE 2
		NULL,
		dwCreationDisposition,  //Should be CREATE_ALWAYS to overwrite or OPEN_EXISTING TO READ
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (NULL == file_handle)
	{
		printf("Couldn't create file\n");
		return NULL;
	}

	return file_handle;
}
