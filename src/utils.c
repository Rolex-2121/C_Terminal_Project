#include "utils.h"

gboolean confirm_deletion(GtkWindow *parent, const char *target) {
    GtkWidget *dialog = gtk_message_dialog_new(
        parent,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_WARNING,
        GTK_BUTTONS_YES_NO,
        "Are you sure you want to delete '%s'?",
        target
    );
    gtk_window_set_title(GTK_WINDOW(dialog), "Confirm Deletion");
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return (response == GTK_RESPONSE_YES);
}
