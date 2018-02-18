#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/*
 * Used for boolean valued functnions
 */
typedef enum{ false, true } boolean;

/*
 * InputString's content is populated by the arguments given to the program
 * position is used to get the next Token eveery time
 * len is the lenght of the content to avoid multiple use of strlen in strings
 * InputString's content's memory should NOT be manipulated
 */
struct InputString{
    char* content;
    int position;
    int len;
};
typedef struct InputString InputString;

/*
 * Token's ePos and sPos indicate the postion of a token in the give InputString struct
 */
struct Token{
    int sPos;
    int ePos;
    InputString* iString;
};
typedef struct Token Token;

boolean HandleInputs(int argc, char** argv, InputString *iString);

boolean IsAlphabetic(char c);

int CountTokens(InputString *iString);

Token GetNextToken(InputString *iString);

char** InputToArray(InputString *iString, int tokenCount);

void InsertionSort(char** tokenArray, int tokenCount);

void OutputArray(char** tokenArray, int tokenCount);

void SkipNonAlphabetic(InputString *iString);

void FreeData(char** tokenArray, int tokenCount);

int StringCompare(char* str1, char* str2);

int main(int argc, char** argv){
    InputString iString;
    if(HandleInputs(argc, argv, &iString)== false)
        return 1;
    int tokenCount = CountTokens(&iString);
    if(tokenCount != 0)
    {
        char** tokenArray = InputToArray(&iString, tokenCount);
        InsertionSort(tokenArray, tokenCount);
        OutputArray(tokenArray, tokenCount);
        FreeData(tokenArray, tokenCount);
    }
    return 0;
}

/*
 * This function only accepts alphabetic strings
 * If the stopping character is the null character on one string,
 * that string will be decided as lower
 * returns:
 *          0 if str1 = str2
 *          0< if str1 < str2
 *          0> if str1 > str2
 */
int StringCompare(char* str1, char* str2){
    int i;
    i = 0;
    char c1 = str1[i] , c2 = str2[i];
    while((c1 != '\0' || c2 != '\0') && c1 == c2)
    {
        i++;
        c1 = str1[i];
        c2 = str2[i];
    }
    if(c1 == '\0' || c2 == '\0') 
        return (int)c1-c2;
    if((c1 >= 'A' && c1 <= 'Z') && (c2 >= 'a' && c2 <= 'z')) 
        return (int)c1-c2;
    if((c1 >= 'a' && c1 <= 'z') && (c2 >= 'A' && c2 >= 'Z'))
        return (int)c2-c1;
    else 
    return (int)c2-c1;
}

/*
 * Outpusts the array list by outputing each element one by one
 */
void OutputArray(char** TokenArray, int tokenCount){
    int i = 0;
    for(; i<tokenCount ; i++)
        printf(ANSI_COLOR_GREEN "%s\n" ANSI_COLOR_RESET, TokenArray[i]);
    return;
}

/*
 * Free's the tokenArray by freeing every element and then freeing the tokenArray
 */
void FreeData(char** tokenArray, int tokenCount){
    int i = 0;
    for(; i < tokenCount; i++)
        free(tokenArray[i]);
    free(tokenArray);
    return;
}

/*
 * Performs insertion sort on tokenArray using StringCompare()
 */
void InsertionSort(char** tokenArray, int tokenCount){
    int i = 0;
    char* tmp;
    for(; i < tokenCount; i++)
    {
        int j = i;
        for(; j < tokenCount; j++)
        {
            if(StringCompare(tokenArray[i], tokenArray[j]) < 0)
            {
                tmp = tokenArray[i];
                tokenArray[i] = tokenArray[j];
                tokenArray[j] = tmp;
            }
        }
    }
    return;
}

/*
 * InputToArray takes and InputString and allocates memory for every token in InputString
 * The number of tokens in the iString should be determined before usage of this function
 * via CountToken()
 * Also, the string array should be freed after use
 */
char** InputToArray(InputString* iString, int tokenCount){
    char** tokenArray = (char **)malloc(tokenCount * sizeof(char*));
    if(tokenArray == NULL)
        printf(ANSI_COLOR_RED "Error: allocating tokens array unsucessful.\n" ANSI_COLOR_RESET);
    Token newTok;
    newTok.sPos = -1;
    newTok.ePos = 0;
    int i = 0;
    while(i!=tokenCount)
    {
        newTok = GetNextToken(iString);
        tokenArray[i] = (char *)malloc((newTok.ePos-newTok.sPos+1) * sizeof(char));
        if(tokenArray[i] == NULL)
            puts(ANSI_COLOR_RED "Error: allocating tokens unsucessful.\n" ANSI_COLOR_RESET);
        tokenArray[i][newTok.ePos-newTok.sPos] = '\0';
        memcpy(tokenArray[i], &(iString->content[newTok.sPos]), (newTok.ePos-newTok.sPos) * sizeof(char));
        i++;
    }
    iString->position = 0;
    return tokenArray;
}

/*
 * GetNextToken uses the position variable in InputString given and finds the position of the next token
 * then returns it via a Token struct
 * no allocation is made here
 */
Token GetNextToken(InputString *iString){
    Token token;
    token.iString = iString;
    int sPos, ePos;
    SkipNonAlphabetic(iString);
    sPos = iString->position;
    while(IsAlphabetic(iString->content[iString->position]) && iString->position != iString->len){
        iString->position++;
    }
    ePos = iString->position;
    token.sPos = sPos;
    token.ePos = ePos;

    return token;
}

/*
 *SkipNonAlphabetic increments the position variable in InputString until it hits an alphabetic character
 */
void SkipNonAlphabetic(InputString *iString){
    while(!IsAlphabetic(iString->content[iString->position]) && iString->position!= iString->len){
        iString->position++;
    }
    return;
}

/*
 * CountTokens counts the number of tokens in InputString.content by calling GetNextToken() until it reaches the end
 */
int CountTokens(InputString *iString){
    int count = 0;
    Token newTok;
    newTok.sPos = -1;
    newTok.ePos = 0;
    while(true)
    {
        newTok = GetNextToken(iString);
        if(newTok.sPos == newTok.ePos)
            break;
        count++;
    }
    iString->position = 0;
    return count;
}

/*
 * HandleInputs makes sure the format of the inputs are given in correct foramt
 * There should be only one input
 * returns false if the program must halt
 * else returns true
 */
boolean HandleInputs(int argc, char** argv, InputString *iString){
    if(argc == 2)
    {
        if(strcmp("-h",argv[1]) == 0)
        {
            printf(ANSI_COLOR_YELLOW "Input a single string to sort in word by word basis\n" ANSI_COLOR_RESET);
            return false;
        }
        iString->content = argv[1];
        iString->position = 0;
        iString->len = strlen(argv[1]);
        return true;
    }
    else
        printf(ANSI_COLOR_RED "Error: This program accepts a single string"
                                " contained between two \"s\n" ANSI_COLOR_RESET);
    return false;
}

/*
 * Checks if the given character is alphabetic
 */
boolean IsAlphabetic(char c){
    if((c>='a' && c<='z')||(c>='A' && c<='Z'))
        return true;
    return false;
}