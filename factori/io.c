/*
io.c
----------------------------------------------------------------------------
Handeling input & output files, any function that get a ptr to a file.
*/

//-------------------------------------------------------------//
// --------------------------INCLUDE-------------------------- //
//-------------------------------------------------------------//

#include "io.h"


//---------------------------------------------------------------//
// -------------------------DECLARATIONS------------------------ //
//---------------------------------------------------------------//

/*--------------------------------------------------------------------------------------------
DESCRIPTION - Print an error msg to the console. msg includes an error code. Close files and exit program.

PARAMETERS - err_code: int representing the type of error. List in HardCodedData.h

RETURN - void
    --------------------------------------------------------------------------------------------*/
void error_handler(int err_code);

/*--------------------------------------------------------------------------------------------
DESCRIPTION - Checking validity of argv. argv must be of specific size and obey:
                argv[2] integer;
                argv[3] integer larger than 0;
                argv[4] str -d / -e only.
PARAMETERS - argc: length of argv[].
             argv: command argument vector.

RETURN - SUCCESS_CODE if correct, error code otherwise.
    --------------------------------------------------------------------------------------------*/
int check_arguments(int argc, char* argv[]);


/*--------------------------------------------------------------------------------------------
DESCRIPTION - Parses the argv given and writes it as parametes.

PARAMETERS - a_mode: pchar to hold e\d;
             threads: holds num of required threads.
             key: holds the key to encrypte/ decrypte.
             argv: command argument vector.

RETURN - void (writes the parameters to mem)
    --------------------------------------------------------------------------------------------*/
void parse_arguments(char* a_mode, int* threads, int* key, char* argv[]);


FILE* open_file(FILE* file, char* file_name, char* file_open_type);


int close_file(FILE* file, char* file_name);


/*--------------------------------------------------------------------------------------------
DESCRIPTION - Creates av array of ints. Every index hold the num of chars in that line including '\n'.

PARAMETERS - input file ptr.

RETURN - array of ints.
    --------------------------------------------------------------------------------------------*/
int* count_row_endings(FILE* file);


/*--------------------------------------------------------------------------------------------
DESCRIPTION - Reads a line from input text. writes 0 to loop when input file is reached EOF.

PARAMETERS - file: input file ptr.
             loop: flag for EOF.

RETURN - ptr to raw line buffer.
    --------------------------------------------------------------------------------------------*/
char* read_line(FILE* file, int* loop);


/*--------------------------------------------------------------------------------------------
DESCRIPTION - Write one line to output file.

PARAMETERS - output_file: output file ptr.
             line: the line to be written.

RETURN - success code if reached.
    --------------------------------------------------------------------------------------------*/
int write_new_line_to_output(FILE* output_file, char* line);


void print_exit_msg(char* in_file_name, char* out_file_name);


//---------------------------------------------------------------//
// ------------------------IMPLEMENTAIONS------------------------//
//---------------------------------------------------------------//
void error_handler(int err_code) {

    printf("\n\n--------------------ERROR--------------------\n\n");
    printf("Error num %d, Check HardCodedData.h to get the error definition.\n", err_code);
    printf("All memory allocated has been released.\n");
    exit(err_code);
}


int check_arguments(int argc, char* argv[]) {

    // making sure there are enough args given
    if (argc != ARGS_REQUIRED) {
        printf("\nIncorrect number of arguments. %d requird,", ARGS_REQUIRED - 1);
        printf(" %d given.", argc - 1);
        error_handler(ERROR_CODE_ARGS);
    }

    // mode must be -d or -e only
    if (!strcmp(argv[4], "-d"));
    else if (!strcmp(argv[4], "-e"));
    else {
        printf("\nMode must be -d or -e. %s given", argv[4]);
        error_handler(ERROR_CODE_ARGS);
    }

    char* pkey;
    char* pthreads;
    long key, threads;

    // making sure key and threads given are integers
    errno = 0;
    key = strtol(argv[2], &pkey, 10);

    if (errno != 0 || *pkey != '\0') {
        error_handler(ERROR_CODE_ARGS);
    }

    threads = strtol(argv[3], &pthreads, 10);

    if (errno != 0 || *pthreads != '\0' || threads <= 0) {
        error_handler(ERROR_CODE_ARGS);
    }
    return SUCCESS_CODE;
}


void parse_arguments(char* a_mode, int* threads, int* key, char *argv[]) {

    *key = atoi(argv[2]);
    *threads = atoi(argv[3]);
    if (!strcmp(argv[4], "-d")) *a_mode = 'd';
    else {  *a_mode = 'e';}

}


FILE* open_file(FILE* file, char* file_name, char* file_open_type)
{
    errno_t retval;

    retval = fopen_s(&file, file_name, file_open_type);
    if (0 != retval)
    {
        printf("Failed to open file: %s\n", file_name);
        error_handler(ERROR_CODE_FILE);
    }
    return file;
}


int close_file(FILE* file, char* file_name)
{
    errno_t retval;

    retval = fclose(file);
    if (0 != retval)
    {
        printf("Failed to close file: %s\n", file_name);
        error_handler(ERROR_CODE_FILE);
    }
    return SUCCESS_CODE;
}


int* count_row_endings(FILE* file) {
    
    int index = 0, capacity = 1, char_counting = 0, empty_flag=1;
    int *row_ending_array = NULL, *temp = NULL;
    char c;

    // allocate memory for the array that holds all locations of row endings
    if (NULL == (row_ending_array = (int*)malloc(capacity * sizeof(int))))
        error_handler(ERROR_CODE_ALLOCATION);

    if (row_ending_array) {

        // read by char
        for (c = fgetc(file); c != EOF; c = fgetc(file))
        {
            char_counting++;
            if (index == capacity - 1)
            {
                // needs to allocate more memory
                temp = (int*)realloc(row_ending_array, (capacity++) * sizeof(int));
                if (temp != NULL) {
                    row_ending_array = temp;
                }
                //allocation failed
                else {
                    free(row_ending_array);
                    error_handler(ERROR_CODE_ALLOCATION);
                }
            }

            if (c == '\n') {
                *(row_ending_array + index) = char_counting;
                index++;
            }
            empty_flag = 0;
        }
        // the input file is empty
        if (empty_flag) {
            printf("\nInput file is empty. Output will be empty too.\n");
        }

        *(row_ending_array + index) = char_counting;
        index++;
        
        *(row_ending_array + index) = '\0';
    }
    return row_ending_array;
}


char* read_line(FILE* file, int* loop)
{
    int index = 0, line_empty =1;
    double capacity = INITIAL_SIZE;
    char* buffer, *temp;
    char c;

    //allocate memory for the line to be read
    if (NULL == (buffer = (char*)malloc(capacity))) error_handler(ERROR_CODE_ALLOCATION);

    if (buffer) {
        // read by char
        for (c = fgetc(file); c != '\n'; c = fgetc(file))
        {
            //allocate more memory
            if (index == capacity - 1)
            {
                temp = (char*)realloc(buffer, capacity * 2);
                if (temp != NULL) {
                    buffer = temp;
                    capacity *= 2;
                }
                else {
                    free(buffer);
                    error_handler(ERROR_CODE_ALLOCATION);
                }

            }
            // end of file reached. change flag
            if (c == EOF) {
                *loop = 0;
                break;
            }
            *(buffer + (index++)) = c;
        }
        if (*loop) {
            *(buffer + (index++)) = '\n';

        }
        *(buffer + (index++)) = '\0';
    }
    return buffer;
}


int write_new_line_to_output(FILE* output_file, char* line) {

    fprintf(output_file, "%s", line);
    return SUCCESS_CODE;
}


void print_exit_msg(char *in_file_name, char* out_file_name) {
    printf("\nSuccess!");
    if (out_file_name == "decrypted.txt") {
        printf("\n%s decrypted to", in_file_name);
        printf(" %s.\n", out_file_name);
    }
    else {
        printf("\n%s encrypted to", in_file_name);
        printf(" %s.\n", out_file_name);
    }
}       