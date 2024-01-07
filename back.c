#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
// todo creating project types: start

typedef struct cellule *list;
typedef struct cellule
{
    char nom[30], prenom[30], matricule[8], id[30];
    list svt;
} cellule;

void allouer(list *nouv)
{
    *nouv = (list)malloc(sizeof(cellule));
}

// todo creating project types: end
// todo creating project functions:start
//? start list manipulation functions:start
void extractFieldValue(const char *jsonString, const char *fieldName, char *destination)
{
    char fieldWithQuotes[30];
    snprintf(fieldWithQuotes, sizeof(fieldWithQuotes), "\"%s\"", fieldName);

    char *fieldStart = strcasestr(jsonString, fieldWithQuotes);

    if (fieldStart != NULL)
    {
        fieldStart += strlen(fieldWithQuotes) + 3;

        char *fieldEnd = strchr(fieldStart, '"');

        if (fieldEnd != NULL)
        {
            size_t fieldLength = fieldEnd - fieldStart;
            strncpy(destination, fieldStart, fieldLength);
            destination[fieldLength] = '\0';
        }
    }
}

// todo creating project functions:end

int main()
{
    FILE *fPtr;
    list start = NULL, end = NULL;
    fPtr = fopen("logicalFile.txt", "r");
    char buffer[1000];
    char nom[44], prenom[44], matricule[44], id[44];
    for (int i = 0; i < 8; i++)
    {
        fgets(buffer, 1000, fPtr); //! to skip the first 8 lines
    }
    while (buffer == '{')
    {
        fgets(buffer, 1000, fPtr);
        extractFieldValue(buffer, "nom", nom);
        fgets(buffer, 1000, fPtr);
        extractFieldValue(buffer, "prenom", prenom);
        fgets(buffer, 1000, fPtr);
        extractFieldValue(buffer, "matricule", matricule);
        fgets(buffer, 1000, fPtr);
        extractFieldValue(buffer, "id", id);
    }


    return 0;
}