#ifndef FILE_OPS_H
#define FILE_OPS_H

#include <gtk/gtk.h>

void handle_file_ops(const gchar *command, GtkWidget *widget, GtkTextBuffer *buffer_view, char *current_dir, gboolean *handled);

#endif
