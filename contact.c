#include <gtk/gtk.h>
#include <time.h>
#include "chat.h"

extern void add_to_list(GtkWidget *list, const gchar *str);
extern GtkWidget *liaolist;
extern char usrName[20];
static GtkWidget *clocker;
int chatwindownum = 0;
extern GtkWidget * ChatWindow;
extern char table[20][20];
GtkWidget *view;
char userlist[20][20];

char chatlist[20][20];
int count = 0;

enum {
    COLUMN = 0,
    NUM_COLS
};

void on_changed(GtkWidget * widget, gpointer statusbar)
{
    GtkTreeIter iter;
    GtkTreeModel *model;
    char *value;
    int flag = 1;
    int k = 0, m = 0, n = 0;
	//static GtkWidget * ChatWindow;

    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), &model, &iter)) {

        gtk_tree_model_get(model, &iter, COLUMN, &value, -1);
        gtk_statusbar_push(GTK_STATUSBAR(statusbar), gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), value), value);

		printf("%s\n",value);
		if(strcmp(value,"我的群")==0 ||strcmp(value,"我的好友")==0)
		{}
		else
		{
			if(chatwindownum == 0)
			{	
				chatwindownum = 1;
				for(k = 0; k < 20; k++)
					for(m = 0; m < 20; m++)
						chatlist[k][m] = '\0';
						
				ChatWindow = chat(value);
				strcpy(chatlist[0], value);
				count = 1;
			}
			else
			{
				
				flag = 1;
				g_print("count: %d\n", count);
				for(n = 0; n < count; n++)
				{
					if(strcmp(chatlist[n], value) == 0)
					{
						gtk_widget_show(ChatWindow);
						flag = 0;
						break;
					}
				}
				if(flag)
				{
					add_to_list(liaolist, value);
					strcpy(chatlist[count], value);	
					count ++;
				}
			}
		}

        g_free(value);
    }
}


void clock_begin()
{
    time_t now;
    struct tm *l_time;
    gchar buf[100];
    now = time((time_t *)NULL);
    l_time = localtime(&now);
    sprintf(buf,"%02d:%02d:%02d",l_time->tm_hour,l_time->tm_min,l_time->tm_sec);
    gtk_label_set_text(GTK_LABEL(clocker),buf);
}

static GtkTreeModel *create_and_fill_model(void)
{
    GtkTreeStore *treestore;
    GtkTreeIter toplevel, child;
	int i = 0;
	
	
    treestore = gtk_tree_store_new(NUM_COLS, G_TYPE_STRING);

    gtk_tree_store_append(treestore, &toplevel, NULL);
    gtk_tree_store_set(treestore, &toplevel, COLUMN, "我的群", -1);
    gtk_tree_store_append(treestore, &child, &toplevel);
    gtk_tree_store_set(treestore, &child, COLUMN, "public", -1);
	g_signal_connect(GTK_OBJECT(treestore),"clicked",G_CALLBACK(gtk_main_quit),NULL);

    gtk_tree_store_append(treestore, &toplevel, NULL);
    gtk_tree_store_set(treestore, &toplevel, COLUMN, "我的好友", -1);

/*-------------------change by whd time 2014.10.30 22:10 start------------------------------------------*/
//将userlist中的用户添加到树状列表中

	for(i = 0;i < 20;i ++)
	{
		if(strcmp(userlist[i],"")==0)
			break;
		gtk_tree_store_append(treestore, &child, &toplevel);
    	gtk_tree_store_set(treestore, &child, COLUMN, userlist[i], -1);
	}

/*-------------------change by whd time 2014.10.30 22:10 end------------------------------------------*/

    return GTK_TREE_MODEL(treestore);
}

static GtkWidget *create_view_and_model(void)
{
    GtkTreeViewColumn *col;
    GtkCellRenderer *renderer;
    //GtkWidget *view;
    GtkTreeModel *model;

    view = gtk_tree_view_new();

    col = gtk_tree_view_column_new();
    gtk_tree_view_column_set_title(col, "好友列表");
    gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_add_attribute(col, renderer, "text", COLUMN);

    model = create_and_fill_model();
    gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
    g_object_unref(model);

    //return view;
}

/*-------------------change by whd time 2014.10.30 22:10 start------------------------------------------*/
//刷新树状列表回调函数
void update_view(GtkWidget *widget,gpointer data)
{
	int i = 0;
	GtkTreeStore *store;
  	GtkTreeModel *model,*modeltmp;
  	GtkTreeIter  iter;
  	bzero(userlist, sizeof(userlist));
	bzero(table, sizeof(table));
	g_print("userNAME: %s\n", usrName);
	requre_update(usrName);
	sleep(1);
	for(i = 0; i < 20; i++)
	{
		if(strcmp(table[i], "") != 0)
		{
			strcpy(userlist[i], table[i]);
			g_print("userlist: %s\n", userlist[i]);
		}
		else
			break;
	}
	store = GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW (view)));
  	model = gtk_tree_view_get_model (GTK_TREE_VIEW (view));

  	if (gtk_tree_model_get_iter_first(model, &iter) == FALSE) 
  	{}
	else
  		gtk_tree_store_clear(store);

	modeltmp = create_and_fill_model();
    gtk_tree_view_set_model(GTK_TREE_VIEW(view), modeltmp);
	g_object_unref(model);
	g_object_unref(modeltmp);
}

/*-------------------change by whd time 2014.10.30 22:10 end------------------------------------------*/
/*========================================================*/
static void trayView(GtkMenuItem * item, gpointer window)
{
    gtk_widget_show(GTK_WIDGET(window));
    gtk_window_deiconify(GTK_WINDOW(window));
}

static void trayExit(GtkMenuItem * item, gpointer user_data)
{
    printf("exit");
    gtk_main_quit();
}

static void trayIconActivated(GObject * trayIcon, gpointer window)
{
    gtk_widget_show(GTK_WIDGET(window));
    gtk_window_deiconify(GTK_WINDOW(window));
}

static void trayIconPopup(GtkStatusIcon * status_icon, guint button, guint32 activate_time, gpointer popUpMenu)
{
    gtk_menu_popup(GTK_MENU(popUpMenu), NULL, NULL, gtk_status_icon_position_menu, status_icon, button, activate_time);
}


static gboolean delete_event(GtkWidget * window, GdkEvent * event, gpointer data)
{
    return FALSE;
}

static gboolean window_state_event(GtkWidget * widget, GdkEventWindowState * event, gpointer trayIcon)
{
    if (event->changed_mask == GDK_WINDOW_STATE_ICONIFIED && (event->new_window_state == GDK_WINDOW_STATE_ICONIFIED || event->new_window_state == (GDK_WINDOW_STATE_ICONIFIED | GDK_WINDOW_STATE_MAXIMIZED))) {
        gtk_widget_hide(GTK_WIDGET(widget));
        gtk_status_icon_set_visible(GTK_STATUS_ICON(trayIcon), TRUE);
    } else if (event->changed_mask == GDK_WINDOW_STATE_WITHDRAWN && (event->new_window_state == GDK_WINDOW_STATE_ICONIFIED || event->new_window_state == (GDK_WINDOW_STATE_ICONIFIED | GDK_WINDOW_STATE_MAXIMIZED))) {
        gtk_status_icon_set_visible(GTK_STATUS_ICON(trayIcon), FALSE);
    }
    return TRUE;
}

/*======================end==================================*/

GtkWidget *contact(gchar *name)
{
	GtkWidget *window;
    GtkTreeSelection *selection;
    GtkWidget *statusbar,*label;
    GtkWidget *vbox,*hbox,*vbox2;
    GtkWidget *sep1,*sep2,*button;
    GtkWidget *image,*frame;
    const GdkPixbuf *src_pixbuf;
    GdkPixbuf *dest_pixbuf;

    //gtk_init(&argc,&argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),name);
    gtk_window_set_position(GTK_WINDOW(window),
            GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window),4);
    gtk_widget_set_size_request(window,220,380);
    g_signal_connect(GTK_OBJECT(window),"destroy",
            G_CALLBACK(gtk_main_quit),NULL);

    vbox = gtk_vbox_new(FALSE,0);
    gtk_container_add(GTK_CONTAINER(window),vbox);
    hbox = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,5);

    frame = gtk_frame_new("");
	gtk_box_pack_start(GTK_BOX(hbox),frame,FALSE,FALSE,5);
	gtk_widget_set_size_request(frame,100,100);
	//image = gtk_image_new_from_file("dongtu.gif");
    src_pixbuf = gdk_pixbuf_new_from_file("dongtu.gif",NULL);
    dest_pixbuf = gdk_pixbuf_scale_simple(src_pixbuf,100,100,
            GDK_INTERP_HYPER);
    image = gtk_image_new_from_pixbuf(dest_pixbuf);
    gtk_container_add(GTK_CONTAINER(frame),image);
	sep1 = gtk_vseparator_new();
	gtk_box_pack_start(GTK_BOX(hbox),sep1,FALSE,FALSE,0);

	vbox2 = gtk_vbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(hbox),vbox2,FALSE,FALSE,12);
    label = gtk_label_new("时间：");
    gtk_box_pack_start(GTK_BOX(vbox2),label,FALSE,FALSE,10);
    clocker = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox2),clocker,FALSE,FALSE,5);
    gtk_timeout_add(1000,(GtkFunction)clock_begin,NULL);
    sep2 = gtk_hseparator_new();
	gtk_box_pack_start(GTK_BOX(vbox),sep2,FALSE,FALSE,0);
	
	button = gtk_button_new_with_label("刷新列表");
	gtk_widget_set_size_request(button,210,32);
	gtk_box_pack_start(GTK_BOX(vbox),button,FALSE,FALSE,5);

/*-------------------change by whd time 2014.10.30 22:10 start------------------------------------------*/
	g_signal_connect(GTK_OBJECT(button),"clicked",G_CALLBACK(update_view),name);	

	//view = create_view_and_model();
	bzero(userlist,sizeof(userlist));
	create_view_and_model();
/*-------------------change by whd time 2014.10.30 22:10 end------------------------------------------*/

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));

    gtk_box_pack_start(GTK_BOX(vbox), view, TRUE, TRUE, 1);

    statusbar = gtk_statusbar_new();
    gtk_box_pack_start(GTK_BOX(vbox), statusbar, FALSE, TRUE, 1);
	//点击列表事件
    g_signal_connect(selection, "changed", G_CALLBACK(on_changed), statusbar);

    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

        /*========================================================*/
    /* 加载托盘图标*/
    GtkStatusIcon *trayIcon = gtk_status_icon_new_from_file("./ico/qq1.ico");

    //为托盘图标设置弹出式菜单
    GtkWidget *menu, *menuItemView, *menuItemExit;
    menu = gtk_menu_new();
    menuItemView = gtk_menu_item_new_with_label("View");
    menuItemExit = gtk_menu_item_new_with_label("Exit");
    g_signal_connect(G_OBJECT(menuItemView), "activate", G_CALLBACK(trayView), window);
    g_signal_connect(G_OBJECT(menuItemExit), "activate", G_CALLBACK(trayExit), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItemView);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItemExit);
    gtk_widget_show_all(menu);

    // 设置工具提示
    gtk_status_icon_set_tooltip(trayIcon, "MsgWatcherGTK");

    //连接鼠标事件句柄connect handlers for mouse events
    g_signal_connect(GTK_STATUS_ICON(trayIcon), "activate", GTK_SIGNAL_FUNC(trayIconActivated), window);
    g_signal_connect(GTK_STATUS_ICON(trayIcon), "popup-menu", GTK_SIGNAL_FUNC(trayIconPopup), menu);
    gtk_status_icon_set_visible(trayIcon, FALSE);// 设置图标可视化

    //g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(destroy), NULL);
    g_signal_connect(G_OBJECT(window), "delete_event", G_CALLBACK(delete_event), trayIcon);
    g_signal_connect(G_OBJECT(window), "window-state-event", G_CALLBACK(window_state_event), trayIcon);
    //gtk_container_add(GTK_CONTAINER(window), menuBar);
    //gtk_widget_show_all(window);
/*========================================================*/

    gtk_widget_show_all(window);

    //gtk_main();

    return window;
}
