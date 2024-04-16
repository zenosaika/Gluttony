#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <gtk/gtk.h> // ignore this error

#define MAX_LEN 1024
#define PORT 5656

char *TITLE = "Gluttony";
int WIDTH = 600;
int HEIGHT = 600;

static void print_hello(GtkWidget *widget, gpointer data)
{
    g_print("Hello World\n");
}

static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *button;

    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "theme.css");
    gtk_style_context_add_provider_for_display(gdk_display_get_default(),
                                              GTK_STYLE_PROVIDER(cssProvider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), TITLE);
    gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGHT);
    gtk_widget_set_name(window, "main");

    // button = gtk_button_new_with_label("Hello World");
    // g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);
    // gtk_window_set_child(GTK_WINDOW(window), button);
    // gtk_widget_set_name(button, "main");

    gtk_window_present(GTK_WINDOW(window));
}

void *connect_server(void *args)
{
    int sd, rc, i, valread;
    char buffer[MAX_LEN] = {0};
    struct sockaddr_in servAddr;

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr);

    /* create socket */
    sd = socket(AF_INET, SOCK_STREAM, 0);

    /* connect to server */
    rc = connect(sd, (struct sockaddr *)&servAddr, sizeof(servAddr));

    /* send response */
    char response[5] = "hi";
    send(sd, response, strlen(response), 0);

    /* read message */
    memset(buffer, 0x0, MAX_LEN); // reset
    valread = read(sd, buffer, MAX_LEN - 1);
    printf("Received: %s\n", buffer);
}

int main(int argc, char **argv)
{
    // connect to server
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, connect_server, NULL);

    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    pthread_exit(NULL);
    return status;
}