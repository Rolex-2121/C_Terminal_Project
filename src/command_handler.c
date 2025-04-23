#include "command_handler.h"
#include "file_ops.h"

extern GtkWidget *output_view;
extern char current_dir[1024];

void run_command(GtkWidget *widget, gpointer input_entry) {

    
    const gchar *command = gtk_entry_get_text(GTK_ENTRY(input_entry));
    if (g_strcmp0(command, "") == 0) return;

    GtkTextBuffer *buffer_view = gtk_text_view_get_buffer(GTK_TEXT_VIEW(output_view));
    gboolean handled = FALSE;

    handle_file_ops(command, widget, buffer_view, current_dir, &handled);
    if (handled) return;

    // Handle cd command
    if (g_str_has_prefix(command, "cd ")) {
        const char *raw_path = command + 3;
        char resolved[1024];

        if (raw_path[0] == '~')
            snprintf(resolved, sizeof(resolved), "%s%s", getenv("HOME"), raw_path + 1);
        else
            strncpy(resolved, raw_path, sizeof(resolved));

        if (chdir(resolved) == 0) {
            getcwd(current_dir, sizeof(current_dir));
            gchar *msg = g_strdup_printf("Changed directory to: %s", current_dir);
            gtk_text_buffer_set_text(buffer_view, msg, -1);
            g_free(msg);
        } else {
            gchar *msg = g_strdup_printf("Failed to change directory to: %s", resolved);
            gtk_text_buffer_set_text(buffer_view, msg, -1);
            g_free(msg);
        }
        return;
    }

    // Default command execution
    gchar *full_command = g_strdup_printf("cd \"%s\" && %s 2>&1", current_dir, command);
    FILE *fp = popen(full_command, "r");
    g_free(full_command);
    if (!fp) {
        gtk_text_buffer_set_text(buffer_view, "Failed to run command.", -1);
        return;
    }

    char buffer[2048];
    GString *result = g_string_new("");
    while (fgets(buffer, sizeof(buffer), fp)) g_string_append(result, buffer);
    pclose(fp);
    gtk_text_buffer_set_text(buffer_view, result->str, -1);
    g_string_free(result, TRUE);
}
