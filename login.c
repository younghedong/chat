#include <gtk/gtk.h>
static GtkWidget* entry1;
static GtkWidget* entry2;


void on_button_ok_clicked (GtkWidget* button,gpointer data)
{
	const gchar *username = gtk_entry_get_text(GTK_ENTRY(entry1));
	const gchar *password = gtk_entry_get_text(GTK_ENTRY(entry2));
	if(selectpsd((char *)username, (char *)password))
	{
		g_print("login ok\n");
	}
	else
	{
		g_print("error\n");
	}
}

void on_button_signup_clicked(GtkWidget *button, gpointer data)
{
	const gchar *username = gtk_entry_get_text(GTK_ENTRY(entry1));
	const gchar *password = gtk_entry_get_text(GTK_ENTRY(entry2));
	g_print("username: %s\npasswd: %s\n", username, password);
	if(insertUser((char *)username, (char *)password) == 0)
	{
		g_print("error\n");
	}
	else
	{
		g_print("ok\n");
	}
}

GtkWidget *login()
{
	GtkWidget* window;
	GtkWidget* box;
	GtkWidget* box1;
	GtkWidget* box2;
	GtkWidget* box3;
	GtkWidget* label1;
	GtkWidget* label2;
	GtkWidget* buttonok;
	GtkWidget* buttonsignup;
	GtkWidget* sep;
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window),"destroy",
	G_CALLBACK(gtk_main_quit),NULL);
	gtk_window_set_title(GTK_WINDOW(window),"登录窗口");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window),20);
	box = gtk_vbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(window),box);
	box1 = gtk_hbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(box),box1,FALSE,FALSE,5);
	box2 = gtk_hbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(box),box2,FALSE,FALSE,5);
	sep = gtk_hseparator_new();
	gtk_box_pack_start(GTK_BOX(box),sep,FALSE,FALSE,5);
	label1 = gtk_label_new("用户名:");
	entry1 = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(box1),label1,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(box1),entry1,FALSE,FALSE,5);
	label2 = gtk_label_new("  密   码:");
	entry2 = gtk_entry_new();
	gtk_entry_set_visibility(GTK_ENTRY(entry2),FALSE);
	gtk_box_pack_start(GTK_BOX(box2),label2,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(box2),entry2,FALSE,FALSE,5);
	buttonok = gtk_button_new_with_label("确 认");
	buttonsignup = gtk_button_new_with_label("注 册");
	box3 = gtk_hbox_new(FALSE, 0);
	gtk_widget_set_size_request(buttonok, 65, 35);
	gtk_widget_set_size_request(buttonsignup, 65, 35);
	gtk_box_pack_start(GTK_BOX(box),box3,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(box3),buttonsignup,FALSE,FALSE,65);
	gtk_box_pack_start(GTK_BOX(box3),buttonok,FALSE,FALSE,2);
	g_signal_connect(G_OBJECT(buttonok),"clicked",
	G_CALLBACK(on_button_ok_clicked),NULL);
	g_signal_connect(G_OBJECT(buttonsignup),"clicked",
	G_CALLBACK(on_button_signup_clicked),NULL);
	return window;
}
