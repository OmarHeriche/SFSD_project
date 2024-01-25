#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Define the structure for a fixed-length record
typedef struct cellule *list;
typedef struct cellule
{
    char nom[30];
    char prenom[30];
    char matricule[8];
    char id[30];
    list svt;
} cellule;

// Forward declarations of GTK signal handlers
void on_button_add_clicked(GtkButton *button, gpointer user_data);
void on_button_delete_clicked(GtkButton *button, gpointer user_data);
void on_button_search_clicked(GtkButton *button, gpointer user_data);
void on_button_show_clicked(GtkButton *button, gpointer user_data);
void on_button_save_clicked(GtkButton *button, gpointer user_data);
void on_button_quit_clicked(GtkButton *button, gpointer user_data);

// Global variables
list start = NULL; // Start of the list

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

// Main GTK function
int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *label_nom, *label_prenom, *label_matricule, *label_id;
    GtkWidget *entry_nom, *entry_prenom, *entry_matricule, *entry_id;
    GtkWidget *button_add, *button_delete, *button_search, *button_show, *button_save, *button_quit;

    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create a new top-level window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "LOF GTK Example");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // Create a new grid
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Create labels and entry fields
    label_nom = gtk_label_new("Nom:");
    entry_nom = gtk_entry_new();
    label_prenom = gtk_label_new("Prenom:");
    entry_prenom = gtk_entry_new();
    label_matricule = gtk_label_new("Matricule:");
    entry_matricule = gtk_entry_new();
    label_id = gtk_label_new("ID:");
    entry_id = gtk_entry_new();

    // Create buttons
    button_add = gtk_button_new_with_label("Add");
    button_delete = gtk_button_new_with_label("Delete");
    button_search = gtk_button_new_with_label("Search");
    button_show = gtk_button_new_with_label("Show");
    button_save = gtk_button_new_with_label("Save");
    button_quit = gtk_button_new_with_label("Quit");

    // Set button click signal handlers
    g_signal_connect(button_add, "clicked", G_CALLBACK(on_button_add_clicked), NULL);
    g_signal_connect(button_delete, "clicked", G_CALLBACK(on_button_delete_clicked), NULL);
    g_signal_connect(button_search, "clicked", G_CALLBACK(on_button_search_clicked), NULL);
    g_signal_connect(button_show, "clicked", G_CALLBACK(on_button_show_clicked), NULL);
    g_signal_connect(button_save, "clicked", G_CALLBACK(on_button_save_clicked), NULL);
    g_signal_connect(button_quit, "clicked", G_CALLBACK(on_button_quit_clicked), NULL);

    // Attach widgets to the grid
    gtk_grid_attach(GTK_GRID(grid), label_nom, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_nom, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_prenom, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_prenom, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_matricule, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_matricule, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_id, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_id, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_add, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_delete, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_search, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_show, 1, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_save, 0, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_quit, 1, 6, 1, 1);

    // Show all widgets
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    return 0;
}

// GTK signal handlers
void on_button_add_clicked(GtkButton *button, gpointer user_data)
{
    const gchar *nom = gtk_entry_get_text(GTK_ENTRY(entry_nom));
    const gchar *prenom = gtk_entry_get_text(GTK_ENTRY(entry_prenom));
    const gchar *matricule = gtk_entry_get_text(GTK_ENTRY(entry_matricule));
    const gchar *id = gtk_entry_get_text(GTK_ENTRY(entry_id));

    insertion(&start, (char *)nom, (char *)prenom, (char *)matricule, (char *)id);
    g_print("Record added.\n");
}

void on_button_delete_clicked(GtkButton *button, gpointer user_data)
{
    const gchar *id = gtk_entry_get_text(GTK_ENTRY(entry_id));

    deleteById(&start, (char *)id);
    g_print("Record deleted.\n");
}

void on_button_search_clicked(GtkButton *button, gpointer user_data)
{
    const gchar *id = gtk_entry_get_text(GTK_ENTRY(entry_id));

    rechercheById((char *)id);
}

void on_button_show_clicked(GtkButton *button, gpointer user_data)
{
    print_list();
}

void on_button_save_clicked(GtkButton *button, gpointer user_data)
{
    save();
    g_print("Records saved.\n");
}

void on_button_quit_clicked(GtkButton *button, gpointer user_data)
{
    gtk_main_quit();
}
