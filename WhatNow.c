#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Structure for a fixed-length record
typedef struct cellule *list;
typedef struct cellule
{
    char nom[30];
    char prenom[30];
    char matricule[8];
    char id[30];
    list svt;
} cellule;

list start = NULL;

// Function to allocate memory for a new node
void allouer(list *nouv)
{
    *nouv = (list)malloc(sizeof(cellule));
}

// Function to insert a record in LOF
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

// Function to delete a record by ID
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
            return;
        }

        previous = current;
        current = current->svt;
    }
}

// Function to search for a record by ID
void rechercheById(char id[30])
{
    int numId = atoi(id);
    list current = start;

    while (current != NULL)
    {
        if (numId == atoi(current->id))
        {
            // Print or use the record as needed
            printf("ID %s found:\n", id);
            printf("name = %s\nprenom = %s\nmatricule = %s\nid = %s\n", current->nom, current->prenom, current->matricule, current->id);
            return;
        }
        current = current->svt;
    }

    printf("ID %s not found.\n", id);
}

// Function to print the entire list
void print_list()
{
    list current = start;
    while (current != NULL)
    {
        // Print or use the record as needed
        printf("name = %s\nprenom = %s\nmatricule = %s\nid = %s\n", current->nom, current->prenom, current->matricule, current->id);
        printf("####################################################################\n");
        current = current->svt;
    }
}

// Function to save the entire list to a file
void save()
{
    FILE *fPtr = fopen("logicalFile.txt", "w");
    list p = start;
    char buffer[1000];
    fputs("{\n    \"Header\": {\n    \"FileType\": \"User Information\",\n    \"Format\": \"JSON\",\n    \"GeneratedDate\": \"2024-01-06\"\n  },\n  \"Users\": [\n", fPtr);
    while (p != NULL)
    {
        fputs("    {\n", fPtr);
        strcpy(buffer, "      \"nom\": \"");
        strcat(buffer, p->nom);
        strcat(buffer, "\",\n");
        fputs(buffer, fPtr);
        strcpy(buffer, "      \"prenom\": \"");
        strcat(buffer, p->prenom);
        strcat(buffer, "\",\n");
        fputs(buffer, fPtr);
        strcpy(buffer, "      \"matricule\": \"");
        strcat(buffer, p->matricule);
        strcat(buffer, "\",\n");
        fputs(buffer, fPtr);
        strcpy(buffer, "      \"ID\": \"");
        strcat(buffer, p->id);
        strcat(buffer, "\"\n");
        fputs(buffer, fPtr);
        fputs("    },\n", fPtr);
        p = p->svt;
    }
    fputs("  ]\n}", fPtr);
    fclose(fPtr);
}

// GTK signal handlers
void on_button_create_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *dialog = GTK_WIDGET(gtk_builder_get_object(builder, "information_modal"));
    gtk_widget_show_all(dialog);
}

void on_button_confirm_create_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *length_entry = GTK_WIDGET(gtk_builder_get_object(builder, "length_entry"));
    const gchar *length_text = gtk_entry_get_text(GTK_ENTRY(length_entry));
    int length = atoi(length_text);

    // Perform necessary operations with the length value
    // This could include updating the record structure or performing other logic

    // Close the dialog
    GtkWidget *dialog = GTK_WIDGET(gtk_builder_get_object(builder, "information_modal"));
    gtk_widget_hide(dialog);
}

void on_button_add_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *dialog = GTK_WIDGET(gtk_builder_get_object(builder, "add_item_modal"));
    gtk_widget_show_all(dialog);
}

void on_button_confirm_clicked(GtkButton *button, gpointer user_data)
{
    // Retrieve values from entry fields
    GtkWidget *first_name_entry = GTK_WIDGET(gtk_builder_get_object(builder, "first_name_entry"));
    GtkWidget *second_name_entry = GTK_WIDGET(gtk_builder_get_object(builder, "second_name_entry"));
    GtkWidget *id_entry = GTK_WIDGET(gtk_builder_get_object(builder, "id_entry"));

    const gchar *first_name = gtk_entry_get_text(GTK_ENTRY(first_name_entry));
    const gchar *second_name = gtk_entry_get_text(GTK_ENTRY(second_name_entry));
    const gchar *id = gtk_entry_get_text(GTK_ENTRY(id_entry));

    // Perform necessary operations with the retrieved values
    insertion(&start, (char *)first_name, (char *)second_name, "", (char *)id);

    // Close the dialog
    GtkWidget *dialog = GTK_WIDGET(gtk_builder_get_object(builder, "add_item_modal"));
    gtk_widget_hide(dialog);
}

void on_button_delete_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *dialog = GTK_WIDGET(gtk_builder_get_object(builder, "delete_modal"));
    gtk_widget_show_all(dialog);
}

void on_button_confirm_delete_clicked(GtkButton *button, gpointer user_data)
{
    // Retrieve value from entry field
    GtkWidget *id_delete_entry = GTK_WIDGET(gtk_builder_get_object(builder, "id_delete_entry"));
    const gchar *id = gtk_entry_get_text(GTK_ENTRY(id_delete_entry));

    // Perform necessary operations with the retrieved value
    deleteById(&start, (char *)id);

    // Close the dialog
    GtkWidget *dialog = GTK_WIDGET(gtk_builder_get_object(builder, "delete_modal"));
    gtk_widget_hide(dialog);
}

void on_button_show_content_clicked(GtkButton *button, gpointer user_data)
{
    // Perform necessary operations to show content
    print_list();
}

void on_button_quit_clicked(GtkButton *button, gpointer user_data)
{
    // Save the data before quitting
    save();

    // Quit the GTK main loop
    gtk_main_quit();
}

// Main GTK function
int main(int argc, char *argv[])
{
    GtkWidget *main_window;
    GtkBuilder *builder;

    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create a new GtkBuilder instance
    builder = gtk_builder_new();

    // Load the UI definition from the Glade file
    if (gtk_builder_add_from_file(builder, design.glade, NULL) == 0)
    {
        g_error("Error loading file");
        return 1;
    }

    // Connect signal handlers
    gtk_builder_connect_signals(builder, NULL);

    // Get the main window
    main_window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));

    // Show the main window
    gtk_widget_show_all(main_window);

    // Start the GTK main loop
    gtk_main();

    // Free the GtkBuilder instance
    g_object_unref(builder);

    return 0;
}
