#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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

char *case_str(const char *haystack, const char *needle)
{
    size_t needle_len = strlen(needle);

    while (*haystack)
    {
        if (strncasecmp(haystack, needle, needle_len) == 0)
        {
            return (char *)haystack;
        }
        haystack++;
    }

    return NULL;
}

void append_in_list(list *start, list *end, char nom[30], char prenom[30], char matricule[8], char id[30])
{
    list nouv;
    if (*start == NULL)
    {
        allouer(start);
        strncat((*start)->nom, nom, sizeof((*start)->nom) - strlen((*start)->nom) - 1);
        strncat((*start)->prenom, prenom, sizeof((*start)->prenom) - strlen((*start)->prenom) - 1);
        strncat((*start)->matricule, matricule, sizeof((*start)->matricule) - strlen((*start)->matricule) - 1);
        strncat((*start)->id, id, sizeof((*start)->id) - strlen((*start)->id) - 1);
        (*start)->svt = NULL;
        *end = *start;
    }
    else
    {
        allouer(&nouv);
        strncat(nouv->nom, nom, sizeof(nouv->nom) - strlen(nouv->nom) - 1);
        strncat(nouv->prenom, prenom, sizeof(nouv->prenom) - strlen(nouv->prenom) - 1);
        strncat(nouv->matricule, matricule, sizeof(nouv->matricule) - strlen(nouv->matricule) - 1);
        strncat(nouv->id, id, sizeof(nouv->id) - strlen(nouv->id) - 1);
        nouv->svt = NULL;
        (*end)->svt = nouv;
        *end = nouv;
    }
}

void extractFieldValue(const char *jsonString, const char *fieldName, char *destination)
{
    char fieldWithQuotes[30];
    snprintf(fieldWithQuotes, sizeof(fieldWithQuotes), "\"%s\"", fieldName);

    char *fieldStart = case_str(jsonString, fieldWithQuotes);

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
        fgets(buffer, 1000, fPtr);
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
                //deleted first node
                *start = current->svt;
            }
            else
            {
                //deleted middle or end
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

list start = NULL; 
GtkTextBuffer *buffer; 
GtkEntry *entry;
GtkWindow *window; 
GtkWindow *insert_window; 
GtkEntry *name_entry; 
GtkEntry *prenom_entry; 
GtkEntry *matricule_entry;
GtkEntry *id_entry; 


// Function bach nekhdem creation button 
void on_create_clicked(GtkButton *button, gpointer user_data)
{
    FILE *fPtr;
    list start = NULL;

    fPtr = fopen("logicalFile.txt", "r");
    if (fPtr == NULL)
    {
        g_print("Error opening the file.\n");
        return;
    }

    creation(fPtr, &start);

    fclose(fPtr);

    update_text_view();
}

// Function ta3 insertion button
void on_insert_clicked(GtkButton *button, gpointer user_data)
{
    gtk_widget_show_all(GTK_WIDGET(insert_window)); 
}

// Function  delete 
void on_delete_clicked(GtkButton *button, gpointer user_data)
{
    char id[30]; 
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *label; 
    GtkWidget *id_entry; 
    gint response; 

    // Create a dialog ok wla cancel
    dialog = gtk_dialog_new_with_buttons("Delete by ID", window, GTK_DIALOG_MODAL, "OK", GTK_RESPONSE_OK, "Cancel", GTK_RESPONSE_CANCEL, NULL);
    
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    
    label = gtk_label_new("Enter the ID of the user to delete:");

    // ye9ra l enteer
    id_entry = gtk_entry_new();
    
    
    gtk_container_add(GTK_CONTAINER(content_area), label);
    gtk_container_add(GTK_CONTAINER(content_area), id_entry);

    // Show the dialog
    gtk_widget_show_all(dialog);
    response = gtk_dialog_run(GTK_DIALOG(dialog));

    // If OK
    if (response == GTK_RESPONSE_OK)
    {
        
        strcpy(id, gtk_entry_get_text(GTK_ENTRY(id_entry)));

        // Call the deleteById function
        deleteById(&start, id);

        update_text_view();
    }
    gtk_widget_destroy(dialog);
}

// Function search 
void on_search_clicked(GtkButton *button, gpointer user_data)
{
    char id[30]; 
    char message[100]; 
    GtkWidget *dialog; 

    
    strcpy(id, gtk_entry_get_text(entry));
    FILE *stream = freopen("NUL", "a", stdout);
    setvbuf(stream, NULL, _IONBF, 0);
    rechercheById(start, id);
    freopen("CON", "w", stdout);
    fgets(message, 100, stream);

    dialog = gtk_message_dialog_new(window, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, message);

    gtk_widget_show_all(dialog);

    gtk_dialog_run(GTK_DIALOG(dialog));

    gtk_widget_destroy(dialog);
}

// Function wach yesra mour ma nkliki 3la insertion
void on_insert_button_clicked(GtkButton *button, gpointer user_data)
{
    char nom[30], prenom[30], matricule[8], id[30]; // The user information to insert
    
    strcpy(nom, gtk_entry_get_text(name_entry));
    strcpy(prenom, gtk_entry_get_text(prenom_entry));
    strcpy(matricule, gtk_entry_get_text(matricule_entry));
    strcpy(id, gtk_entry_get_text(id_entry));
    
    insertion(&start, nom, prenom, matricule, id);
    
    update_text_view();
    
    gtk_widget_hide(GTK_WIDGET(insert_window));
}

// Function cancel button 
void on_cancel_button_clicked(GtkButton *button, gpointer user_data)
{
    gtk_widget_hide(GTK_WIDGET(insert_window));
}


// todo project functions:end
//! the back : end

int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);
    // todo start window:
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);
    

    GtkWidget *fixed = gtk_fixed_new();

    
    // ya omar hada wach dert

    
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    
    gtk_box_set_homogeneous(GTK_BOX(hbox), TRUE);

    //create buttons

    //creat
    GtkWidget *create_button = gtk_button_new_with_label("Create");
    
    g_signal_connect(create_button, "clicked", G_CALLBACK(on_create_clicked), NULL);
    
    gtk_box_pack_start(GTK_BOX(hbox), create_button, TRUE, TRUE, 0);

    //insert 
    GtkWidget *insert_button = gtk_button_new_with_label("Insert");

    g_signal_connect(insert_button, "clicked", G_CALLBACK(on_insert_clicked), NULL);

    gtk_box_pack_start(GTK_BOX(hbox), insert_button, TRUE, TRUE, 0);

    //delete
    GtkWidget *delete_button = gtk_button_new_with_label("Delete");
    
    g_signal_connect(delete_button, "clicked", G_CALLBACK(on_delete_clicked), NULL);
    
    gtk_box_pack_start(GTK_BOX(hbox), delete_button, TRUE, TRUE, 0);

    //search
    GtkWidget *search_button = gtk_button_new_with_label("Search");
    
    g_signal_connect(search_button, "clicked", G_CALLBACK(on_search_clicked), NULL);

    gtk_box_pack_start(GTK_BOX(hbox), search_button, TRUE, TRUE, 0);

    gtk_fixed_put(GTK_FIXED(fixed), hbox, 0, 0);

    
    FILE *fPtr;
    list start = NULL, end = NULL; 
    creation(fPtr, &start);
    list p = start;
    int cpt = 0;
    while (p != NULL)
    {
        cpt++;
        p = p->svt;
    }
    p = start;

    
    for (int i = 0; i < cpt; i++)
    {
        GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        
        char pointerAddress[20];
        snprintf(pointerAddress, sizeof(pointerAddress), "%p", (void *)p);

        
        GtkWidget *pLabel = gtk_label_new(pointerAddress);
        gtk_box_pack_end(GTK_BOX(box), pLabel, TRUE, TRUE, 0);

        snprintf(pointerAddress, sizeof(pointerAddress), "%p", (void *)p->svt);
        gtk_box_pack_start(GTK_BOX(box), gtk_button_new_with_label(p->nom), TRUE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(box), gtk_button_new_with_label(p->prenom), TRUE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(box), gtk_button_new_with_label(p->matricule), TRUE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(box), gtk_button_new_with_label(p->id), TRUE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(box), gtk_button_new_with_label(pointerAddress), TRUE, TRUE, 0);

        p = p->svt;

        gtk_fixed_put(GTK_FIXED(fixed), box, i * 115 + 5, 200);
    }
    

    gtk_container_add(GTK_CONTAINER(window), fixed); 
    
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);
    gtk_main();
    

    return 0;
}
