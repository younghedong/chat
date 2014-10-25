#include <gtk/gtk.h>


GtkWidget *informationdialog(gint errornum)
{
	GtkWidget *dialog;
	GtkMessageType type;
	gchar *message;
	
	switch(errornum)
	{
		case 0:
			message = "注册成功";
			type = GTK_MESSAGE_INFO;
			break;
		case -1:
			message = "初始化错误";
			type = GTK_MESSAGE_ERROR;
			break;
		case -2:
			message = "用户名已存在";
			type = GTK_MESSAGE_WARNING;
			break;
		case -3:
			message = "连接数据库失败";
			type = GTK_MESSAGE_ERROR;
			break;
		case -4:
			//message = "账户不存在";
		case -5:
			//message = "密码错误";
			message = "用户名或密码错误";
			type = GTK_MESSAGE_WARNING;
			break; 
		case -6:
			message = "密码长度少于六位字符";
			type = type = GTK_MESSAGE_WARNING;
	}
	dialog = gtk_message_dialog_new(NULL,
		GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
		type,
		GTK_BUTTONS_OK,
		message);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	return dialog;
}
