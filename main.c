/******************************************************************************
Group Number: Group 24
Lab Number: Cmp 1 - Activity 05 (13:00 Monday, Online lab)
Team Members:
- Robin Hanjun Kwon (12723680)
- Michael Logozzo (12559973)
- Timothy Heyes (12545202)
- Madhav Diwan (13930437)
- Simrat Sandhu (13576083)

Compiling instructions:
1. Simply type 'make' or 'make normal' command in command line to compile 
all source files in normal mode which is default.
2. 'PM.exe' is excutable file, so type './PM.exe [argument]' in command line 
to run the program.
(For example, type './PM.exe -a' to run program in XOR encryption mode. 
More about argument, please refer to Argument section below)
3. If you want to RUN in DEBUG MODE, type 'make debug' command in command line.
(More details about debugging in 'Command' and 'Debug mode' section)
4. 'PM_DEBUG.exe' is excutable file which runs in debug mode, 
so type './PM_DEBUG.exe [argument]' in command line to run the program.

Arguments:
1. '-a' to run XOR encryption mode.
2. '-b' to run ROT13 encryption mode.

Command:
1. 'make' to compile all source files in normal mode which is default.
2. 'make normal' to compile in normal mode. 
(This command does as same as using only 'make' command as shown above)
3. 'make debug' to compile in debug mode.
4. 'make clean' to delete 'PM.exe', 'PM_DEBUG.exe', '*.txt', and '*.bin' files.

Debug mode:
- 'main.c' and 'Huffman.c' supports debug mode.
1. To run debug mode, type 'make debug' command in command line to compile 
all source files in debug mode.
2. To run normal mode, type 'make' or 'make normal' command in command line 
to compile all source files in normal mode which is default.
3. Make sure that each command build different name of the output, which are 
'PM.exe' or 'PM_DEBUG.exe'.
******************************************************************************/

/*Libaries*/
#include "Core.h"
#include "Huffman.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*Function Prototypes*/
void printMenu(void);

/*Functions*/
/******************************************************************************
main function
Author: Michael Logozzo
******************************************************************************/
int main(int argc, char* argv[])
{
    FILE *src1, *src2, *src3;

    int i;
    int choice = 0;
    password_t pwList[MAX_SIZE], temp[MAX_SIZE];
    char input[MAX_SIZE], txtFile[MAX_SIZE], 
    binFile[MAX_SIZE], treeFile[MAX_SIZE];

    /* Initialise counter pointer */
    int* count_p;
    count_p = (int*) malloc(sizeof(int));
    *count_p = 0;

    printf("\nLocal Password Manager\n");
    printf("----------------------\n");
    printf("Welcome to our local password manager\n");

    /* Specify whether debug mode is running or not 
    at the beginning of the program */
    if(DEBUG)
        printf("The program runs in [Debug mode]\n");
    else
        printf("The program runs in [Normal mode]\n");

    /*Check for encryption mode, letter user know 
    which encryption mode is running*/
    if(argc <=1)
    {
        printf("Specify an encryption mode\n");
        return 1;
    }
    if(strcmp(argv[1], "-a") == 0)
    {
        printf("Password Manager is using XOR encryption\n");
    }
    else if(strcmp(argv[1], "-b") == 0)
    {
        printf("Password Manager is using ROT13 encryption\n");
    }
    else
    {
        printf("Invalid Input\n");
        return 1;
    }

    /*Main menu loop*/
    while(choice!=5)
    {
        printMenu();
        scanf("%d", &choice);
        
        switch(choice)
        {
            case 1:
                /*Add new password to array*/
                addPassword(pwList, *count_p);
                *count_p+=1;
                break;

            case 2:
                /*View a password in the array*/
                viewPassword(pwList);
                break;

            case 3:
                /*Copies password array to a temp variable*/
                /*Temp variable gets encrypted, becomes unreadable*/
                /*Password array remains readable*/
                for(i=0; i<*count_p; i++)
                {
                    temp[i] = pwList[i];
                }
                /*User enters file name*/
                printf("Enter the name of the file to save> ");
                scanf("%s", input);
                
                /*Add file extension to input for later use*/
                strcpy(txtFile, input);
                strcat(txtFile, ".txt");
                
                /*Encrypt file*/
                encryptData(temp, *count_p, argv[1]);
                /*Save to text file*/
                saveToFile(txtFile, temp, *count_p);
                /*Compress text file to bin file*/
                if ((src1 = fopen(txtFile, "r")) == NULL)
		        {
			        printf("Error: Could not find '%s'.\n", txtFile);
			        break;
		        }
		        compress(src1, input);
#if DEBUG
                printf("[Debug mode] '%s' file has been generated.\n", txtFile);
#endif

#if DEBUG != 1
                /*Remove text file*/
                remove(txtFile);
#endif
                fclose(src1);
                break;

            case 4:
                /*User inputs file they want to load*/
                printf("Enter the name of the file to load> ");
                scanf(" %s", input);
                
                /*add file extensions to user input*/
                strcpy(txtFile, input);
                strcat(txtFile, "_decompressed.txt");

                strcpy(binFile, input);
                strcat(binFile, ".bin");

                strcpy(treeFile, input);
                strcat(treeFile, "_tree.txt");                
                
                /*Decompress bin file*/
                if ((src2 = fopen(binFile, "rb")) == NULL)
		        {
			        printf("Error: Could not find '%s'.\n", binFile);
                    break;
		        }
		
		        if ((src3 = fopen(treeFile, "r")) == NULL)
		        {
			        printf("Error: Could not find '%s'.\n", treeFile);
                    break;
		        }
		
		        decompress(src2, src3, input);
                fclose(src2);
		        fclose(src3);
                /*Load File*/
                loadFile(txtFile, pwList, count_p);
#if DEBUG
                printf("[Debug mode] '%s' file has been generated.\n", txtFile);
#endif

#if DEBUG != 1
                remove(txtFile);
#endif

#if DEBUG
                printf("[Debug mode] Encrypted name of the website: ");
                printf("%s\n", pwList[0].website);
#endif 

                encryptData(pwList, *count_p, argv[1]);

#if DEBUG
                printf("[Debug mode] Name of the website: ");
                printf("%s\n", pwList[0].website);
#endif
                break;

            case 5:
                break;
            
            default:
                printf("Invalid choice\n");
                break;                    
        }
    }
    /* Free memory and end */
    free(count_p);
    count_p = NULL;
    return 0;
}

/******************************************************************************
printMenu function shows the prompt to user
Author: Michael Logozzo
Input:
- None
Output:
- None
******************************************************************************/
void printMenu(void)
{
    printf("\nSelect a number between 1-5\n"
    "1. Add new account\n"
    "2. View specified account\n"
    "3. Save accounts to a local file\n"
    "4. Load accounts from a local file\n"
    "5. Exit\n"
    "> ");
}

