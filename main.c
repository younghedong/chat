#include <gtk/gtk.h>
#include "login.h"
#include "insertdata.h"
#include "selectdata.h"
#include "informationdialog.h"
#include "contact.h"
#include "client.h"

int main ( int argc , char* argv[])
{	
	gtk_init(&argc,&argv);
	client();
	GtkWidget *window = login();
	//GtkWidget *window = contact("haha");
	//gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,5);
	gtk_widget_show_all(window);
	gtk_main();
	return FALSE;
}
