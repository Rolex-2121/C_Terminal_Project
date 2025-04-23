#include "file_ops.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

void handle_file_ops(const gchar *command, GtkWidget *widget, GtkTextBuffer *buffer_view, char *current_dir, gboolean *handled) {
    *handled = TRUE;

    // Create file
    if (g_str_has_prefix(command, "create ")) {
        const char *filename = command + 7;
        char filepath[1024];
        snprintf(filepath, sizeof(filepath), "%s/%s", current_dir, filename);
        FILE *fp = fopen(filepath, "w");

        gchar *msg = fp ? g_strdup_printf("✅ File created: %s", filepath)
                        : g_strdup_printf("❌ Failed to create file: %s", filepath);
        if (fp) fclose(fp);
        gtk_text_buffer_set_text(buffer_view, msg, -1);
        g_free(msg);
        return;
    }

    // Delete file
    if (g_str_has_prefix(command, "delete ")) {
        const char *filename = command + 7;
        char filepath[1024];
        snprintf(filepath, sizeof(filepath), "%s/%s", current_dir, filename);

        if (!confirm_deletion(GTK_WINDOW(gtk_widget_get_toplevel(widget)), filepath)) return;

        if (remove(filepath) == 0) {
            gchar *msg = g_strdup_printf("🗑️ File deleted: %s", filepath);
            gtk_text_buffer_set_text(buffer_view, msg, -1);
        } else {
            gchar *msg = g_strdup_printf("⚠️ Failed to delete file: %s\n🔍 Possible Reasons:\n1. Permission denied\n2. File in use\n3. File does not exist\n\n💡 Try:\n• sudo permission\n• Close the program using the file", filepath);
            gtk_text_buffer_set_text(buffer_view, msg, -1);
        }
        return;
    }

    // Make directory
    if (g_str_has_prefix(command, "mkdir ")) {
        const char *folder = command + 6;
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", current_dir, folder);

        int res = mkdir(path, 0755);
        gchar *msg = res == 0 ? g_strdup_printf("📁 Directory created: %s", path)
                              : g_strdup_printf("❌ Failed to create directory: %s", path);
        gtk_text_buffer_set_text(buffer_view, msg, -1);
        g_free(msg);
        return;
    }

    // Remove directory
    if (g_str_has_prefix(command, "rmdir ")) {
        const char *folder = command + 6;
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", current_dir, folder);

        if (!confirm_deletion(GTK_WINDOW(gtk_widget_get_toplevel(widget)), path)) return;

        int res = rmdir(path);
        gchar *msg = res == 0 ? g_strdup_printf("🗑️ Directory removed: %s", path)
                              : g_strdup_printf("⚠️ Failed to remove directory: %s\n🔍 Possible Reasons:\n1. Directory not empty\n2. Permission denied\n3. Directory does not exist\n\n💡 Try:\n• Delete contents first\n• Use sudo if needed", path);
        gtk_text_buffer_set_text(buffer_view, msg, -1);
        g_free(msg);
        return;
    }

    *handled = FALSE;  // If none of the conditions matched
}
