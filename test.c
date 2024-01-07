#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Project types
typedef struct cellule *list;
typedef struct cellule
{
    char nom[30], prenom[30], matricule[8], id[30];
    list svt;
} cellule;

// Function to allocate memory for a new node
void allouer(list *nouv)
{
    *nouv = (list)malloc(sizeof(cellule));
    if (*nouv == NULL)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
}

// Function to append a new node to the list
void append_in_list(list *start, list *end, const char *nom, const char *prenom, const char *matricule, const char *id)
{
    list nouv;
    allouer(&nouv);

    // Set data
    strncat(nouv->nom, nom, sizeof(nouv->nom) - strlen(nouv->nom) - 1);
    strncat(nouv->prenom, prenom, sizeof(nouv->prenom) - strlen(nouv->prenom) - 1);
    strncat(nouv->matricule, matricule, sizeof(nouv->matricule) - strlen(nouv->matricule) - 1);
    strncat(nouv->id, id, sizeof(nouv->id) - strlen(nouv->id) - 1);

    // Set pointers
    nouv->svt = NULL;
    if (*start == NULL)
    {
        *start = nouv;
        *end = *start;
    }
    else
    {
        (*end)->svt = nouv;
        *end = nouv;
    }
}

// Function to extract field value from JSON string
void extractFieldValue(const char *jsonString, const char *fieldName, char *destination)
{
    char fieldWithQuotes[40]; // Increased buffer size
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

int main()
{
    // Declaration
    FILE *fPtr;
    list start = NULL, end = NULL;
    fPtr = fopen("logicalFile.txt", "r");

    if (fPtr == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    char buffer[1000];
    char nom[30], prenom[30], matricule[8], id[30]; // Reduced buffer size
    // Skip the first 8 lines
    for (int i = 0; i < 8; i++)
    {
        if (fgets(buffer, sizeof(buffer), fPtr) == NULL)
        {
            perror("Error reading file");
            fclose(fPtr);
            return 1;
        }
    }

    // Read and append data to the list
    while (fgets(buffer, sizeof(buffer), fPtr) != NULL && strstr(buffer, "{") != NULL)
    {
        fgets(buffer, sizeof(buffer), fPtr);
        extractFieldValue(buffer, "nom", nom);
        fgets(buffer, sizeof(buffer), fPtr);
        extractFieldValue(buffer, "prenom", prenom);
        fgets(buffer, sizeof(buffer), fPtr);
        extractFieldValue(buffer, "matricule", matricule);
        fgets(buffer, sizeof(buffer), fPtr);
        extractFieldValue(buffer, "ID", id);
        append_in_list(&start, &end, nom, prenom, matricule, id);
        fgets(buffer, sizeof(buffer), fPtr); // Skip a line
        fgets(buffer, sizeof(buffer), fPtr); // Skip the closing bracket
    }

    // Print the list
    list current = start;
    while (current != NULL)
    {
        printf("name = %s prenom = %s matricule = %s id = %s\n", current->nom, current->prenom, current->matricule, current->id);
        current = current->svt;
    }

    // Cleanup
    fclose(fPtr);

    // Free allocated memory
    current = start;
    while (current != NULL)
    {
        list temp = current;
        current = current->svt;
        free(temp);
    }

    printf("\n the end");

    return 0;
}
