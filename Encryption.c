/******************************************************************************
File: Encryption.c
Author: Michael Logozzo
Date: Friday, 16th of October
Reference:
  - https://hackernoon.com/reasons-why-xor-is-important-in-cryptography-6tcn32yx
  - https://en.wikipedia.org/wiki/ROT13 
******************************************************************************/
#include "Core.h"
#include <stdio.h>
#include <string.h>

/******************************************************************************
Encrypts the data from the input password_t struct 
using specified encryption technique.
Author: Michael Logozzo
Input:
- Unencrypted password_t struct, encryption mode
Output:
- Encrypted password_t struct
******************************************************************************/
void encryptData(password_t input[], int count, char* mode)
{
    char xorKey;
    int i;
    
    /*Check for encryption mode via command line parsing*/
    /*a corresponds to XOR encryption*/
    if(strcmp(mode, "-a") == 0)
    {
        /*User must input a random key*/
        /*This ensures only the intended user can unencrypt*/
        printf("Please specify you encryption key> ");
        scanf(" %c", &xorKey);

        for(i=0; i<count; i++)
        {
            /*Encrypt each section of input stuct, write to output struct*/
            strcpy(input[i].website, xor(input[i].website, xorKey));
            strcpy(input[i].userName, xor(input[i].userName, xorKey));
            strcpy(input[i].password, xor(input[i].password, xorKey));
        }
    }
    /*b corresponds to ROT13 encryption*/
    else if(strcmp(mode, "-b") == 0)
    {
        for(i=0; i<count; i++)
        {
            /*Encrypt each section of input stuct, write to output struct*/
            strcpy(input[i].website, rot13(input[i].website));
            strcpy(input[i].userName, rot13(input[i].userName));
            strcpy(input[i].password, rot13(input[i].password));
        }       
    }
}

/******************************************************************************
Encrypts input string using XOR encryption technique.
Author: Michael Logozzo
Input:
- Unencrypted string, XOR key
Output:
- Encrypted string
******************************************************************************/
char* xor(char input[], char xorKey)
{
    int i, len;
    
    /*Find length of input string*/
    len = strlen(input);
    /*Bitwise XOR operation for every char in string*/
    for(i=0; i<len; i++)
    {
        input[i] = input[i] ^ xorKey;
    }
    /*Ensure last char is NULL*/
    input[len] = '\0';

    return input;
}

/******************************************************************************
Encrypts the input string using ROT13 encryption technique.
Author: Michael Logozzo
Input:
- Unencrypted string
Output:
- Encrypted string
******************************************************************************/
char* rot13(char input[])
{
    int i, len;
    
    /*Find length of input string*/
    len = strlen(input);
    /*Cycle through each char*/
    for(i=0; i<len; i++)
    {
        /*Check if upper case*/
        if(input[i]>='A' && input[i]<='Z')
        {
            /*Shift letter by 13 places*/
            if(input[i]+ROT<='Z')
            {
                input[i] = input[i] + ROT;
            }
            else
            {
                input[i] = input[i] - ROT;
            }
            
        }
        /*Check if lower case*/
        else if(input[i]>='a' && input[i]<='z')
        {
            /*Shift letter by 13 places*/
            if(input[i]+ROT<='z')
            {
                input[i] = input[i] + ROT;
            }
            else
            {
                input[i] = input[i] - ROT;
            }
            
        }
    }
    /*Ensure last char is NULL*/
    input[len] = '\0';

    return input;
}

