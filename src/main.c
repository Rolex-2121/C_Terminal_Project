#include <gtk/gtk.h>
#include "command_handler.h"

GtkWidget *output_view;
char current_dir[1024];

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Load custom CSS theme
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "style.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
            GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    strcpy(current_dir, getenv("HOME"));

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Mini Terminal");
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget *left_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *right_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), main_box);
    gtk_box_pack_start(GTK_BOX(main_box), left_panel, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(main_box), right_panel, TRUE, TRUE, 5);

    GtkWidget *command_list = gtk_label_new("Available Commands:\n\n"
                                            "  ls\n"
                                            "  pwd\n"
                                            "  whoami\n"
                                            "  date\n"
                                            "  uname -a\n"
                                            "  df -h\n"
                                            "  free -m\n"
                                            "  cd ~/<path>\n"
                                            "  create <fileName>\n"
                                            "  delete <fileName>\n"
                                            "  mkdir <FolderName>\n"
                                            "  rmdir <FolderName>\n");
    gtk_box_pack_start(GTK_BOX(left_panel), command_list, TRUE, TRUE, 5);

    GtkWidget *input = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(input), "Enter a command...");
    gtk_box_pack_start(GTK_BOX(right_panel), input, FALSE, FALSE, 0);
    

    GtkWidget *run_button = gtk_button_new_with_label("Run");
    gtk_box_pack_start(GTK_BOX(right_panel), run_button, FALSE, FALSE, 0);
    g_signal_connect(run_button, "clicked", G_CALLBACK(run_command), input);

    output_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(output_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(output_view), GTK_WRAP_WORD_CHAR);
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window), output_view);
    gtk_box_pack_start(GTK_BOX(right_panel), scrolled_window, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
