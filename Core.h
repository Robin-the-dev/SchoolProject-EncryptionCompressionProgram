#ifndef _CORE_H_
#define _CORE_H_

/*Macros*/
#define ROT 13
#define MAX_SIZE 100
#define TRUE 0
#define FALSE 1

/*Structures*/
struct password
{
    char website[MAX_SIZE];
    char userName[MAX_SIZE];
    char password[MAX_SIZE];
};
typedef struct password password_t;

/*Function Prototypes*/
void encryptData(password_t input[], int count, char* mode);
char* xor(char input[], char xorKey);
char* rot13(char input[]);
void addPassword(password_t data_array[], int count);
void viewPassword(password_t data_array[]);
int saveToFile(char file_name[], password_t data_array[], int count);
int loadFile(char file_name[], password_t data_array[], int* count);

#endif
