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
void append_in_list(list *start, list *end, char nom[30], char prenom[30], char matricule[8], char id[30]) {
    list nouv;
    if (*start == NULL) {
        allouer(start);
        //!setting data:start
        strcpy((*start)->nom,nom);
        strcpy((*start)->prenom,prenom);
        strcpy((*start)->matricule,matricule);
        strcpy((*start)->id,id);
        //!setting data:end
        (*start)->svt = NULL;
        *end = *start;
    } else {
        allouer(&nouv);
        //!setting data:start
        strcpy((nouv)->nom,nom);
        strcpy((nouv)->prenom,prenom);
        strcpy((nouv)->matricule,matricule);
        strcpy((nouv)->id,id);
        //!setting data:end
        nouv->svt = NULL;
        (*end)->svt = nouv;
        *end = nouv;
    }
}
//? start list manipulation functions:end
void extractFieldValue(const char *jsonString, const char *fieldName, char *destination) {
    char fieldWithQuotes[30];
    snprintf(fieldWithQuotes, sizeof(fieldWithQuotes), "\"%s\"", fieldName);

    char *fieldStart = strstr(jsonString, fieldWithQuotes);

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

    fgets(buffer,1000,fPtr);
    fgets(buffer,1000,fPtr);
    extractFieldValue(buffer,"id",dest);
    printf("\n%s",dest);

    return 0;
}