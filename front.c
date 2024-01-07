#include <gtk/gtk.h>
//! the back : start
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
void append_in_list(list *start, list *end, char nom[30], char prenom[30], char matricule[8], char id[30]) //! hadi nrml order
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

// todo project functions:start
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
// todo project functions:end
//! the back : end

int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);
//todo start window:
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    // gtk_window_maximize(GTK_WINDOW(window));
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);
//todo end window:

    GtkWidget *fixed = gtk_fixed_new();//! the fixed contaier;
    // gtk_orientable_set_orientation(GTK_ORIENTABLE(fixed), GTK_ORIENTATION_HORIZONTAL); // todo fixed container:on vertical.
// todo start construction list:
    FILE *fPtr;
    list start = NULL, end = NULL;//? the start is only for cloning do not work using it;
    creation(fPtr, &start);
    list p = start;
    int cpt = 0;
    while (p != NULL)
    {
        cpt++;
        p = p->svt;
    }
    p = start;
// todo end construction list:

//! create buttons & add them to the fixed(vertical) :start
for (int i = 0; i < cpt; i++)
{
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
//? Convert pointer address to string
    char pointerAddress[20];
    snprintf(pointerAddress, sizeof(pointerAddress), "%p", (void *)p);

//? Create a label with the pointer address
    GtkWidget *pLabel = gtk_label_new(pointerAddress);
    gtk_box_pack_end(GTK_BOX(box), pLabel, TRUE, TRUE, 0);
    
    snprintf(pointerAddress, sizeof(pointerAddress), "%p", (void *)p->svt);
    gtk_box_pack_start(GTK_BOX(box), gtk_button_new_with_label(p->nom), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), gtk_button_new_with_label(p->prenom), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), gtk_button_new_with_label(p->matricule), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), gtk_button_new_with_label(p->id), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), gtk_button_new_with_label(pointerAddress), TRUE, TRUE, 0);


    p = p->svt;

    gtk_fixed_put(GTK_FIXED(fixed), box, i * 115+5, 200);
}
//! create buttons & add them to the fixed(vertical) :end

    gtk_container_add(GTK_CONTAINER(window), fixed);//! append the fixed container lifih koulesh to the window (main container)

//! some functions out of our context right now:start
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);
    gtk_main();
//! some functions out of our context right now:end

    return 0;
}
