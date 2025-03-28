#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tinyexpr.h"  // Include TinyExpr for expression evaluation

// Global variable to store the current expression
char expression[256] = "";

// Function to update the display
void update_display(GtkEntry *entry) {
    gtk_entry_set_text(GTK_ENTRY(entry), expression);
}

// Function to evaluate the arithmetic expression
void evaluate_expression(GtkEntry *entry) {
    if (strlen(expression) == 0) {
        return;  // Avoid evaluating an empty expression
    }

    double result = te_interp(expression, 0);  // Evaluate the mathematical expression

    // Check if the result is valid (TinyExpr returns NaN for invalid expressions)
    if (result != result) {  // NaN check
        strcpy(expression, "Error");  // Display "Error" for invalid expressions
    } else {
        snprintf(expression, sizeof(expression), "%g", result);  // Convert result to string
    }

    update_display(entry);  // Update the display with the result
}

// Function to handle button clicks
void on_button_clicked(GtkWidget *widget, gpointer data) {
    if (!widget || !data) return;  // Prevent NULL pointer crash

    const char *text = gtk_button_get_label(GTK_BUTTON(widget));  // Get button label
    GtkEntry *entry = GTK_ENTRY(data);

    if (!text) return;  // Ensure button label exists

    if (strcmp(text, "C") == 0) {
        strcpy(expression, "");  // Clear expression
    } else if (strcmp(text, "⌫") == 0) {
        if (strlen(expression) > 0) {
            expression[strlen(expression) - 1] = '\0';  // Remove the last character
        }
    } else if (strcmp(text, "=") == 0) {
        evaluate_expression(entry);  // Calculate result
        return;
    } else {
        if (strlen(expression) + strlen(text) < sizeof(expression)) {
            strcat(expression, text);
        }
    }

    update_display(entry);
}

// Function to create and display the calculator
void create_calculator() {
    GtkWidget *window, *grid, *entry;
    gtk_init(NULL, NULL);

    // Create the main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GTK+ Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 250, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a grid layout for buttons
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Create the display entry field
    entry = gtk_entry_new();
    gtk_entry_set_alignment(GTK_ENTRY(entry), 1);  // Right-align text
    gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE);  // Make entry field read-only
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 0, 4, 1);

    // Buttons for calculator
    const char *buttons[] = {
        "7", "8", "9", "/",
        "4", "5", "6", "*",
        "1", "2", "3", "-",
        "0", "C", "⌫", "=",
        "+"
    };

    // Create buttons and add them to the grid
    int i;
    for (i = 0; i < 16; i++) {
        GtkWidget *button = gtk_button_new_with_label(buttons[i]);
        g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), entry);
        gtk_grid_attach(GTK_GRID(grid), button, i % 4, (i / 4) + 1, 1, 1);
    }

    // Show all widgets
    gtk_widget_show_all(window);
    gtk_main();
}

// Main function
int main(int argc, char *argv[]) {
    create_calculator();
    return 0;
}
