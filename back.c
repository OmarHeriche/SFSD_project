#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
//todo creating project types: start

typedef struct cellule *list;
typedef struct cellule{
    char nom[30],prenom[30],matricule[8],id[30];
    list svt;
}cellule;

void allouer (list *nouv){
    *nouv=(list)malloc(sizeof(cellule));
}

//todo creating project types: end
//todo creating project functions:start
//? start list manipulation functions:start 
void extractFieldValue(const char *jsonString, const char *fieldName, char *destination) {
    char fieldWithQuotes[30];
    snprintf(fieldWithQuotes, sizeof(fieldWithQuotes), "\"%s\"", fieldName);

    char *fieldStart = strcasestr(jsonString, fieldWithQuotes);

    if (fieldStart != NULL) {
        fieldStart += strlen(fieldWithQuotes) + 3; 

        char *fieldEnd = strchr(fieldStart, '"'); 

        if (fieldEnd != NULL) {
            size_t fieldLength = fieldEnd - fieldStart;
            strncpy(destination, fieldStart, fieldLength);
            destination[fieldLength] = '\0'; 
        }
    }
}

//todo creating project functions:end

int main() {
    FILE *fPtr;
    fPtr= fopen("logicalFile.txt","r");
    char buffer[1000];
    for (int i = 0; i < 9; i++)
    {
        fgets(buffer,1000,fPtr);//! to skip the first 9 lines 
    }
    char dest[44];
    extractFieldValue(buffer,"nom",dest);
    printf("%s",dest);

    fgets(buffer,1000,fPtr);
    extractFieldValue(buffer,"prenom",dest);
    printf("\n%s",dest);

    // fgets(buffer,1000,fPtr);
    fgets(buffer,1000,fPtr);
    extractFieldValue(buffer,"matricule",dest);
    printf("\n%s",dest);

    fgets(buffer,1000,fPtr);
    extractFieldValue(buffer,"id",dest);
    printf("\n%s",dest);

    return 0;
}