#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
// todo creating project types: start

typedef struct cellule *list;
typedef struct cellule
{
    char nom[30];
    char prenom[30];
    char matricule[8];
    char id[30];
    list svt;
} cellule;

void allouer(list *nouv)
{
    *nouv = (list)malloc(sizeof(cellule));
}

// todo creating project types: end
void append_in_list(list *start, list *end, char nom[30], char prenom[30], char matricule[8], char id[30])//! hadi nrml order
{
    list nouv;
    if (*start == NULL)
    {
        allouer(start);
        //? set data:start
        strncat((*start)->nom, nom, sizeof((*start)->nom) - strlen((*start)->nom) - 1);
        strncat((*start)->prenom, prenom, sizeof((*start)->prenom) - strlen((*start)->prenom) - 1);
        strncat((*start)->matricule, matricule, sizeof((*start)->matricule) - strlen((*start)->matricule) - 1);
        strncat((*start)->id, id, sizeof((*start)->id) - strlen((*start)->id) - 1);

        //? set data:end
        (*start)->svt = NULL;
        *end = *start;
    }
    else
    {
        allouer(&nouv);
        //? set data:start
        strncat(nouv->nom, nom, sizeof(nouv->nom) - strlen(nouv->nom) - 1);
        strncat(nouv->prenom, prenom, sizeof(nouv->prenom) - strlen(nouv->prenom) - 1);
        strncat(nouv->matricule, matricule, sizeof(nouv->matricule) - strlen(nouv->matricule) - 1);
        strncat(nouv->id, id, sizeof(nouv->id) - strlen(nouv->id) - 1);

        //? set data:end
        nouv->svt = NULL;
        (*end)->svt = nouv;
        *end = nouv;
    }
}

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

//todo project functions:start
void insertion(list *start, char nom[30], char prenom[30], char matricule[8], char id[30])
{
    int numId = atoi(id);
    list prd = NULL, svt = *start;
    list nouv;

    allouer(&nouv);
    strncat(nouv->nom, nom, sizeof(nouv->nom) - 1);
    strncat(nouv->prenom, prenom, sizeof(nouv->prenom) - 1);
    strncat(nouv->matricule, matricule, sizeof(nouv->matricule) - 1);
    strncat(nouv->id, id, sizeof(nouv->id) - 1);
    nouv->svt = NULL;

    if (*start == NULL || numId < atoi(svt->id))
    {
        nouv->svt = *start;
        *start = nouv;
        return;
    }

    while (svt != NULL && numId > atoi(svt->id))
    {
        prd = svt;
        svt = svt->svt;
    }

    nouv->svt = svt;
    prd->svt = nouv;
}

void creation(FILE *fPtr, list *tete)
{
    list start = NULL, end = NULL;
    char buffer[1000];
    char nom[44], prenom[44], matricule[44], id[44];

    fPtr = fopen("logicalFile.txt", "r");
    for (int i = 0; i < 8; i++)
    {
        fgets(buffer, 1000, fPtr); //! to skip the first 8 lines
    }
    while (buffer[4] == '{')
    {
        fgets(buffer, 1000, fPtr);
        extractFieldValue(buffer, "nom", nom);
        fgets(buffer, 1000, fPtr);
        extractFieldValue(buffer, "prenom", prenom);
        fgets(buffer, 1000, fPtr);
        extractFieldValue(buffer, "matricule", matricule);
        fgets(buffer, 1000, fPtr);
        extractFieldValue(buffer, "ID", id);
        fgets(buffer, 1000, fPtr);
        fgets(buffer, 1000, fPtr);
        insertion(&start, nom, prenom, matricule, id);
    }
    *tete = start;
}

void print_list(list start)
{
    list current = start;
    while (current != NULL)
    {
        printf("name = %s\nprenom = %s\nmatricule = %s\nid = %s\n", current->nom, current->prenom, current->matricule, current->id);
        printf("####################################################################\n");
        current = current->svt;
    }
}
void rechercheById(list start, char id[30])
{
    int numId = atoi(id);
    list current = start;

    while (current != NULL)
    {
        if (numId == atoi(current->id))
        {
            printf("ID %s found:\n", id);
            printf("name = %s\nprenom = %s\nmatricule = %s\nid = %s\n", current->nom, current->prenom, current->matricule, current->id);
            return;
        }
        current = current->svt;
    }

    printf("ID %s not found.\n", id);
}

void deleteById(list *start, char id[30])
{
    int numId = atoi(id);
    list current = *start;
    list previous = NULL;

    while (current != NULL)
    {
        if (numId == atoi(current->id))
        {
            if (previous == NULL)
            {
                // Node to be deleted is the first node
                *start = current->svt;
            }
            else
            {
                // Node to be deleted is in the middle or end
                previous->svt = current->svt;
            }

            free(current);
            printf("Node with ID %s deleted.\n", id);
            return;
        }

        previous = current;
        current = current->svt;
    }

    printf("ID %s not found for deletion.\n", id);
}
//todo project functions:end
int main()
{
    FILE *fPtr;
    list start = NULL, end = NULL;
    creation(fPtr, &start);
    rechercheById(start,"007");
    // print_list(start);
    return 0;
}
