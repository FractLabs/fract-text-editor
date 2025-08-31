#include <gtk-3.0/gtk/gtk.h>
#include <string>
#include <fstream>
#include "icon.h"

GtkWidget *window;
GtkWidget *label;
GtkWidget *textview;
GtkWidget *btn_close;
GtkWidget *btn_save;
GtkTextBuffer *textbuffer;
std::string currentPath;

void set_window_title(const std::string &title) {
    gtk_window_set_title(GTK_WINDOW(window), title.c_str());
}

void show_message(const char* msg, GtkMessageType type) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_MODAL, type, GTK_BUTTONS_OK, "%s", msg);

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void on_new_file(GtkWidget*, gpointer) {
    gtk_widget_hide(label);
    gtk_text_buffer_set_text(textbuffer, "", -1);
    gtk_widget_show(textview);
    gtk_widget_show(btn_close);
    gtk_widget_show(btn_save);

    currentPath.clear();
    set_window_title("Fract Text Editor - New File");
}

void on_open_file(GtkWidget*, gpointer) {
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        std::ifstream file(filename, std::ios::in);

        if (!file) {
            show_message("Failed to open file!", GTK_MESSAGE_ERROR);
            g_free(filename);
            gtk_widget_destroy(dialog);
            return;
        }

        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        gtk_text_buffer_set_text(textbuffer, content.c_str(), -1);
        gtk_widget_show(textview);
        gtk_widget_hide(label);
        gtk_widget_show(btn_close);
        gtk_widget_show(btn_save);

        currentPath = filename;
        set_window_title("Fract Text Editor - " + currentPath);

        g_free(filename);
    }
    gtk_widget_destroy(dialog);
}

void on_save_file(GtkWidget*, gpointer) {
    if (!gtk_widget_get_visible(textview)) {
        show_message("No file is open to save!", GTK_MESSAGE_WARNING);
        return;
    }

    if (currentPath.empty()) {
        GtkWidget *dialog = gtk_file_chooser_dialog_new("Save File", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_SAVE, "_Cancel", GTK_RESPONSE_CANCEL, "_Save", GTK_RESPONSE_ACCEPT, NULL);
        gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);

        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
            char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
            currentPath = filename;
            g_free(filename);
        }
        gtk_widget_destroy(dialog);

        if (currentPath.empty()) return;
    }

    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(textbuffer, &start, &end);
    char *text = gtk_text_buffer_get_text(textbuffer, &start, &end, FALSE);

    std::ofstream file(currentPath, std::ios::out | std::ios::trunc);
    if (!file) {
        show_message("Failed to save file!", GTK_MESSAGE_ERROR);
        g_free(text);
        return;
    }

    file << text;
    file.close();
    g_free(text);

    show_message("File saved successfully!", GTK_MESSAGE_INFO);
    set_window_title("Fract Text Editor - " + currentPath);
}

void on_close_file(GtkWidget*, gpointer) {
    if (gtk_widget_get_visible(textview)) {
        gtk_widget_hide(textview);
        gtk_widget_show(label);
        set_window_title("Fract Text Editor");
        currentPath.clear();

        gtk_widget_hide(btn_close);
        gtk_widget_hide(btn_save);
    }
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Fract Text Editor");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 350);
    gtk_window_set_resizable(GTK_WINDOW(window), false);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 5);

    GtkWidget *btn_new = gtk_button_new_with_label("New");
    GtkWidget *btn_open = gtk_button_new_with_label("Open");
    btn_close = gtk_button_new_with_label("Close");
    btn_save = gtk_button_new_with_label("Save");

    gtk_box_pack_start(GTK_BOX(hbox), btn_new, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), btn_open, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), btn_close, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), btn_save, TRUE, TRUE, 0);

    label = gtk_label_new("Welcome to Fract Text Editor!");
    gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 5);

    textview = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textview), GTK_WRAP_WORD_CHAR);
    textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));

    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scroll), textview);
    gtk_widget_set_size_request(scroll, 490, 295);
    gtk_box_pack_start(GTK_BOX(vbox), scroll, TRUE, TRUE, 5);

    g_signal_connect(btn_new, "clicked", G_CALLBACK(on_new_file), NULL);
    g_signal_connect(btn_open, "clicked", G_CALLBACK(on_open_file), NULL);
    g_signal_connect(btn_save, "clicked", G_CALLBACK(on_save_file), NULL);
    g_signal_connect(btn_close, "clicked", G_CALLBACK(on_close_file), NULL);

    // keybinds
    GtkAccelGroup *accel = gtk_accel_group_new();
    gtk_window_add_accel_group(GTK_WINDOW(window), accel);

    gtk_widget_add_accelerator(btn_new, "clicked", accel, GDK_KEY_n, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(btn_open, "clicked", accel, GDK_KEY_o, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(btn_save, "clicked", accel, GDK_KEY_s, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(btn_close, "clicked", accel, GDK_KEY_x, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    // icon

    GError *error = nullptr;
    GdkPixbufLoader *loader = gdk_pixbuf_loader_new();
    if (!gdk_pixbuf_loader_write(loader, icon_png, (gsize)icon_png_len, &error)) {
        g_printerr("Error while trying to load icon: %s\n", error->message);
        g_clear_error(&error);
    } else {
        gdk_pixbuf_loader_close(loader, &error);
        GdkPixbuf *pixbuf = gdk_pixbuf_loader_get_pixbuf(loader);
        if (pixbuf) {
            gtk_window_set_icon(GTK_WINDOW(window), pixbuf);
        }
    }
    g_object_unref(loader);

    // css

    GtkSettings *default_settings = gtk_settings_get_default();
    g_object_set(default_settings, "gtk-application-prefer-dark-theme", TRUE, NULL);

    const char *dark_css =
    "window, entry, scrolledwindow {"
    "   background-color: #2b2b2b;"
    "   color: #e6e6e6;"
    "}"
    "button {"
    "   background-color: #3a3a3aff;"
    "   color: #161616ff;"
    "   border-radius: 6px;"
    "   padding: 6px 12px;"
    "}"
    "button:hover {"
    "   background-color: #505050;"
    "}"
    "textview, textview text {"
    "   background-color: #252525;"
    "   border-radius: 5px;"
    ""
    "}"
    "scrollbar slider {"
    "   background-color: #666666;"
    "   border-radius: 4px;"
    "}"
    "scrollbar trough {"
    "   background-color: #2b2b2b;"
    "}";

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, dark_css, -1, NULL);

    GtkStyleContext *context;
    GdkScreen *screen = gdk_screen_get_default();
    gtk_style_context_add_provider_for_screen(
        screen,
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );
    g_object_unref(provider);

    gtk_widget_show_all(window);
    gtk_main();

    gtk_widget_hide(textview);
    gtk_widget_hide(btn_close);
    gtk_widget_hide(btn_save);

    return 0;
}