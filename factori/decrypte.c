/*
decrypte.c
----------------------------------------------------------------------------
Decryption or encryption of txt by key.
*/


//-------------------------------------------------------------//
// --------------------------INCLUDE-------------------------- //
//-------------------------------------------------------------//

#include "decrypte.h"


//---------------------------------------------------------------//
// -------------------------DECLARATIONS------------------------ //
//---------------------------------------------------------------//

/*--------------------------------------------------------------------------------------------
DESCRIPTION - Decryptes or encryptes a line according to the Caesar system.
              In cryptography, a Caesar cipher, also known as Caesar's cipher, the shift cipher,
              Caesar's code or Caesar shift, is one of the simplest and most widely known encryption techniques.
              It is a type of substitution cipher in which each letter in the plaintext is replaced by a letter
              some fixed number of positions down the alphabet. For example, with a left shift of 3, D would be
              replaced by A, E would become B, and so on. The method is named after Julius Caesar, who used it in
              his private correspondence.

PARAMETERS - line: ptr of the line to encrypte / decrypte
             key: integer for the calculation of the Caesar system
             mode: e for encryption, d for decryption

RETURN - ptr to the line processed
    --------------------------------------------------------------------------------------------*/
char* decrypte_line(char* line, long key, char mode);


//---------------------------------------------------------------//
// ------------------------IMPLEMENTAIONS------------------------//
//---------------------------------------------------------------//
char* decrypte_line(char* line, long key, char mode) {

    int i, mode_sign;
    char ch;
    size_t len = strlen(line);

    // determine mode of operation
    if (mode == 'd') mode_sign = 1;
    else { mode_sign = -1; }
    key *= mode_sign;

    // run on every char in the line
    for (i = 0; i < len; i++) {
        ch = *(line + i);
        // integer 0-9
        if (isdigit(ch)) {
            if ((ch - '0' - key) < 0) ch += 10; // underflow
           ch = '0' + (ch - '0' - key) % 10; 
        }
        if (isalpha(ch)) {
            // letter a-z
            if (islower(ch)) {
                if ((ch - 'a' - key) < 0) ch += 26; // underflow
                ch = 'a' + (ch - 'a' - key) % 26;
            }
            // letter A-Z
            else {
                if ((ch - 'A' - key) < 0) ch += 26; // underflow
                ch = 'A' + (ch - 'A' - key) % 26;
            }
        }
        *(line + i) = ch;
    }
    return line;
}

