#include <gtk/gtk.h>
GtkWidget *chat()
{
    GtkWidget *window;
    GtkWidget *hbox,*vboxleft,*vboxright,*hboxtool,*hboxmessage;
    GtkWidget *view,*textmain,*image,*viewinfo;
    GtkWidget *labelhistory,*entry,*button;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),"chat");
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window),10);
    g_signal_connect(GTK_OBJECT(window),"destroy",
            G_CALLBACK(gtk_main_quit),NULL);

    hbox = gtk_hbox_new(FALSE,0);
    gkt_container_add(GTK_CONTAINER(window),hbox);
    vboxleft = gtk_vbox_new(FALSE,0);
    vboxright = gtk_vbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(hbox),vboxleft,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(hbox),vboxright,FALSE,FALSE,5);

    view = gtk_scrolled_window_new(NULL,NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(view),
            GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
    textmain = gtk_text_view_new();
    gtk_box_pack_start(GTK_BOX(vboxleft),view,TRUE,TRUE,5);
    gtk_container_add(GTK_CONTAINER(view),textmain);

    hboxtool = gtk_hbox_new(FALSE,0);
    labelhistory = gtk_label_new("history");
    gtk_box_pack_end(GTK_BOX(hboxtool),labelhistory,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(vboxleft),hboxtool,FALSE,FALSE,5);

    hboxmessage = gtk_hbox_new(FALSE,0);
    entry = gtk_entry_new();
    button = gtk_button_new_with_label("send");
    gtk_box_pack_start(GTK_BOX(hboxmessage),entry,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(hboxmessage),button,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(vboxleft),hboxmessage,FALSE,FALSE,5);

    image = gtk_image_new_from_file("");
    gtk_box_pack_start(GTK_BOX(vboxright),image,FALSE,FALSE,5);

    viewinfo = gtk_text_view_new();
    gtk_box_pack_start(GTK_BOX(vboxright),viewinfo,FALSE,FALSE,5);

    return window;
}
