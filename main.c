#include <gtk/gtk.h>
#include "login.h"

int main ( int argc , char* argv[])
{	
	gtk_init(&argc,&argv);
	GtkWidget *window = login();
	//gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,5);
	gtk_widget_show_all(window);
	gtk_main();
	return FALSE;
}
