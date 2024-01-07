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
void append_in_list(list *start, list *end, char nom[30], char prenom[30], char matricule[8], char id[30])
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
        append_in_list(&start, &end, nom, prenom, matricule, id);
    }
    *tete = start;
}

void insertion(list *start, char nom[30], char prenom[30], char matricule[8], char id[30])
{
    int numId = atoi(id);
    list prd = *start, svt = (*start)->svt;
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
    }
    else if (prd != NULL && svt == NULL)
    {
        if (numId < atoi(prd->id))
        {
            allouer(&nouv);
            //? set data:start
            strncat(nouv->nom, nom, sizeof(nouv->nom) - strlen(nouv->nom) - 1);
            strncat(nouv->prenom, prenom, sizeof(nouv->prenom) - strlen(nouv->prenom) - 1);
            strncat(nouv->matricule, matricule, sizeof(nouv->matricule) - strlen(nouv->matricule) - 1);
            strncat(nouv->id, id, sizeof(nouv->id) - strlen(nouv->id) - 1);
            //? set data:end
            nouv->svt = prd;
            prd->svt = NULL;
            prd = nouv;
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
            prd->svt = nouv;
            nouv->svt = NULL;
        }
    }
    else
    {
        while (prd != NULL)
        {
            if (numId <= atoi(svt->id) && numId > atoi(prd->id))
            {
                allouer(&nouv);
                //? set data:start
                strncat(nouv->nom, nom, sizeof(nouv->nom) - strlen(nouv->nom) - 1);
                strncat(nouv->prenom, prenom, sizeof(nouv->prenom) - strlen(nouv->prenom) - 1);
                strncat(nouv->matricule, matricule, sizeof(nouv->matricule) - strlen(nouv->matricule) - 1);
                strncat(nouv->id, id, sizeof(nouv->id) - strlen(nouv->id) - 1);
                //? set data:end
                nouv->svt=svt;
                prd->svt=nouv;
            }
            prd=prd->svt;
            svt=svt->svt;
        }
    }
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

int main()
{
    FILE *fPtr;
    list start = NULL, end = NULL;
    creation(fPtr, &start);
    print_list(start);
    return 0;
}