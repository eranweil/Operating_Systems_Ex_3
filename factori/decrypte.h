#pragma once
/*
decrypte.h
----------------------------------------------------------------------------
Decryption or encryption of txt by key.
Header file for decrypte.c
*/


//-------------------------------------------------------------//
// --------------------------INCLUDE-------------------------- //
//-------------------------------------------------------------//

#include "HardCodedData.h"
#include <stdio.h>
#include <windows.h>
#include <stdio.h>


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
