/*
main.c
----------------------------------------------------------------------------
AUTHORS - Itamar Eyal 302309539 and Eran Weil 204051353

PROJECT - Factori

DESCRIPTION - This program can find the prime number building blocks for a give number

              In cryptography, a Caesar cipher, also known as Caesar's cipher, the shift cipher,
              Caesar's code or Caesar shift, is one of the simplest and most widely known encryption techniques.
              It is a type of substitution cipher in which each letter in the plaintext is replaced by a letter
              some fixed number of positions down the alphabet. For example, with a left shift of 3, D would be
              replaced by A, E would become B, and so on. The method is named after Julius Caesar, who used it in
              his private correspondence. (from wikipedia: https://en.wikipedia.org/wiki/Caesar_cipher).

    consists of 3 modules:
        io - handles files and cmd line args
        threadManager - creates and operates threads.
        decrypte - utilizies the Caesar coding system to encrypte or decrypte text.

*/

//-------------------------------------------------------------//
// ----------------------LIBRARY INCLUDES--------------------- //
//-------------------------------------------------------------//

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


//-------------------------------------------------------------//
// ----------------------PROJECT INCLUDES--------------------- //
//-------------------------------------------------------------//

#include "HardCodedData.h"
#include "io.h"
#include "decrypte.h"
#include "threadManager.h"
#include "Queue.h"


int* break_into_primes(int n, int* prime_numbers, int* prime_numbers_size)
{
    int prime_numbers_index = *prime_numbers_size;
    int j, i = 3;

    while ((n % 2) == 0)
    {
        if (NULL == (prime_numbers = (int*)realloc(prime_numbers, prime_numbers_index * sizeof(int)))) return prime_numbers;
        *(prime_numbers + prime_numbers_index - 1) = 2;
        n = n / 2;
        printf("Number %d is %d\n", prime_numbers_index, 2);
        prime_numbers_index++;
    }

    while (i <= sqrt(n))
    {
        while ((n % i) == 0)
        {
            if (NULL == (prime_numbers = (int*)realloc(prime_numbers, prime_numbers_index * sizeof(int)))) return prime_numbers;
            *(prime_numbers + prime_numbers_index - 1) = i;
            n = n / i;
            printf("Number %d is %d\n", prime_numbers_index, i);
            prime_numbers_index++;
            //if (((n % i) == 0) && ((n / i) == 1)) prime_numbers_index--;
        }
        i = i + 2;
    }

    if (n > 2)
    {
        if (NULL == (prime_numbers = (int*)realloc(prime_numbers, prime_numbers_index * sizeof(int)))) return prime_numbers;
        *(prime_numbers + prime_numbers_index - 1) = n;
        printf("Number %d is %d\n", prime_numbers_index, n);
    }
    else prime_numbers_index--;

    //for (j = 0; j < prime_numbers_index; j++)
    //{
    //    printf("%d\n", *(prime_numbers + j));
    //}
    *prime_numbers_size = prime_numbers_index;
    return prime_numbers;
}

void print_primes_to_file(FILE* file, int* prime_numbers, int prime_numbers_size, int offset, int n)
{
    int i = 1;
//    fseek(file, (sizeof(char) * offset), SEEK_SET);
    fprintf(file, "The prime factors of %d are: %d", n, *(prime_numbers));
    while (i < prime_numbers_size)
    {
        fprintf(file, ", %d", *(prime_numbers + i));
        i++;
    }
    fprintf(file, "\n");
}

int* create_numbers_array(FILE* file, int* numbers_array, int number_of_tasks, char* tasks_file_name)
{

    int n, i = 0;
    char curr_digit = '\0';

    if (NULL == (file = open_file(file, tasks_file_name, "r"))) return ERROR_CODE_FILE;

    while ((curr_digit = fgetc(file)) != EOF)
    {
        n = 0;
        while (curr_digit != '\n')
        {
            n = ((10 * n) + (curr_digit - '0'));
            curr_digit = fgetc(file);
        }

        *(numbers_array + i) = n;
        i++;
        printf("%d\n", n); //for debug

        //curr_digit = fgetc(file);
        //curr_digit = fgetc(file);
    }

    if (0 != close_file(file, tasks_file_name)) return ERROR_CODE_FILE;
    return numbers_array;
}

int main(int argc, char** argv[]) 
{
    FILE* tasks_file = NULL;
    FILE* priority_file = NULL;
    //char* tasks_file_name = "Tasks.txt";

    QUEUE queue;
    QUEUE * p_queue = &queue;
    StartQueue(p_queue);

    int number_of_tasks = atoi(argv[3]);
    int* numbers_array = NULL;
    if (NULL == (numbers_array = (int*)malloc(number_of_tasks * sizeof(int)))) return ERROR_CODE_ALLOCATION;
    
    int prime_numbers_size = 1;
    int* p_prime_numbers_size = &prime_numbers_size;
    int* prime_numbers;

    char curr_digit = '\0';
    int i = 0;
    int n;

    // Create queue
    if (NULL == (p_queue = InitializeQueue(priority_file, p_queue, number_of_tasks, argv[2]))) return ERROR_CODE_ALLOCATION;

    // Create array with all of the numbers
    if (NULL == (numbers_array = create_numbers_array(tasks_file, numbers_array, number_of_tasks, argv[1]))) return ERROR_CODE_ALLOCATION;
 
    // Open file
    if (NULL == (tasks_file = open_file(tasks_file, argv[1], "a"))) return ERROR_CODE_FILE;

    //create queue


    //while (i < number_of_tasks)
    //{
    //    create_the_correct_number_of_threads;

    //}

    while (i < number_of_tasks)
    {
        n = *(numbers_array + i);
        prime_numbers = NULL;
        prime_numbers_size = 1;
        if (NULL == (prime_numbers = break_into_primes(n, prime_numbers, p_prime_numbers_size))) return ERROR_CODE_ALLOCATION;
        print_primes_to_file(tasks_file, prime_numbers, prime_numbers_size, 5, *(numbers_array + i));
        i++;
    }

    //PrintQueue(p_queue);
    //DestroyQueue(p_queue);
    //PrintQueue(p_queue);


    //free(numbers_array);
    //free(prime_numbers);

    if (0 != close_file(tasks_file, argv[1])) return ERROR_CODE_FILE;

    return SUCCESS_CODE;
}
    //// check that the arguments are suitable. if not, exit program.
    //check_arguments(argc, argv);

    //// Parse arguments for mode of operation, num of threades and key.
    //char mode = '\0';
    //long key, threads_required;
    //parse_arguments(&mode, &threads_required, &key, argv);

    //int* row_ending = NULL;

    //// File parameters
    //FILE* p_input_file = NULL;
    //char* in_file_name = argv[1];
    //char* in_file_open_type = "r";

    //FILE* p_output_file = NULL;
    //char* out_file_name = "decrypted.txt";
    //if (mode == 'e') out_file_name = "encrypted.txt";
    //char* out_file_open_type = "w";

    //// Open file with check
    //if (NULL == (p_input_file = open_file(p_input_file, in_file_name, in_file_open_type))) error_handler(ERROR_CODE_FILE);

    //// Open file with check
    //if (NULL == (p_output_file = open_file(p_output_file, out_file_name, out_file_open_type))) error_handler(ERROR_CODE_FILE);

    //// Array holding locations of all line ending in the input file.
    //row_ending = count_row_endings(p_input_file);

    //// thread manager function
    //dispatch_threads(p_input_file, p_output_file, key, threads_required, row_ending, mode);

    //// Close file with check
    //if (close_file(p_input_file, in_file_name) == STATUS_CODE_FAILURE) error_handler(ERROR_CODE_FILE);

    //// Close file with check
    //if (close_file(p_output_file, out_file_name) == STATUS_CODE_FAILURE) error_handler(ERROR_CODE_FILE);

    //print_exit_msg(in_file_name, out_file_name);

    //return SUCCESS_CODE;
