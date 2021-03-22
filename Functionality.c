#include "Core.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/******************************************************************************
View username and password in given data array.
Author: Timothy Heyes
Input:
- password_t array
Output:
- None
******************************************************************************/
void viewPassword(password_t data_array[]){
    char website[MAX_SIZE];
    int i, check = FALSE;

    /*select password in array given input param*/
    printf("\nEnter the website you wish to view the password for: ");
    scanf(" %s", website);
#ifdef DEBUG
    printf("DEBUG: website value is: %s\n", website);
#endif 


    for (i = 0; i < MAX_SIZE; i++) {
        if (strcmp(data_array[i].website, website) == 0) {
            /*print password struct members to the user if we find a matching website*/
            printf("Website: %s\n", data_array[i].website);
            printf("Username: %s\n", data_array[i].userName);
            printf("Password: %s\n", data_array[i].password);
            check = TRUE;
            break;
        }
    }
    
    /*if we dont find any saved passwords for the given website, print error.*/
    if (check != TRUE) {
        printf("No saved passwords for that website.\n");
    }
    
}

/******************************************************************************
Save data array of passwords to file.
Author: Simrat Sandhu
Input:
- File name
- Data Array 
Output:
- 0 for success, 1 for fail
******************************************************************************/
int saveToFile(char file_name[], password_t data_array[], int count)
{
    FILE* fp = NULL;
    int i;
    
    /*Opens file and checks if it worked*/
    if ((fp = fopen(file_name, "w")) == NULL)
    {
        printf("Error: Could not open file.\n");
        return 1;
    }
    
    /*If file opened successfully write passwords from array into file*/
    for (i = 0; i < count; i++)
    {
        fprintf(fp, "%s\n", data_array[i].website);
        fprintf(fp, "%s\n", data_array[i].userName);
        fprintf(fp, "%s\n", data_array[i].password);
    }
    fclose(fp);
    return 0;
}

/******************************************************************************
Load data array of passwords from file.
Author: Simrat Sandhu
Input:
- File Name
- Data Array
Output:
- 0 for success, 1 for fail
******************************************************************************/
int loadFile(char file_name[], password_t data_array[], int* count)
{
    int i, temp;
    FILE* fp;

    /*Opens file and checks if it worked*/
    if ((fp = fopen(file_name, "r")) == NULL)
    {
        printf("Error: Could not open file.\n");
        return 1;
    }

    /* Check if file is empty */
    fseek(fp, 0, SEEK_END);
    if(ftell(fp)==0)
    {
        *count=0;
        fclose(fp);
        return 0;
    }
    fseek(fp, 0, SEEK_SET);

    *count = 0;
    /*If file opened successfully read passwords from file into array*/
    for(i=0; i<MAX_SIZE; i++)
    {
        /*Save data from file into array*/
        fgets(data_array[i].website, MAX_SIZE, fp);
        fgets(data_array[i].userName, MAX_SIZE, fp);
        fgets(data_array[i].password, MAX_SIZE, fp);

        /*Remove newline character from strings*/
        data_array[i].website[strlen(data_array[i].website)-1] = '\0';
        data_array[i].userName[strlen(data_array[i].userName)-1] = '\0';
        data_array[i].password[strlen(data_array[i].password)-1] = '\0';

        *count+=1;

        /* Check if end of file */
        temp=fgetc(fp);
        if(temp==EOF)
        {
            fclose(fp);
            return 0;
        }
        else
        {
            ungetc(temp,fp);
        }
    }
    fclose(fp);
    return 0;
}

/******************************************************************************
Add new username and password to the data array
Author: Madhav Diwan
Input:
- Position in password_t array
Output:
- None
******************************************************************************/
void addPassword(password_t data_array[], int count){

    if(count<MAX_SIZE){
        /*collect website name*/
        printf("\nEnter the website you wish to set the password for> ");
        scanf(" %s", (data_array+count)->website);

        /*collect username*/
        printf("Set Username> ");
        scanf(" %s", (data_array+count)->userName);

        /*collect password*/
        printf("Set Password> ");
        scanf(" %s", (data_array+count)->password);
    }
}
