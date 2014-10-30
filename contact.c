#include <gtk/gtk.h>
#include <time.h>
#include "chat.h"

static GtkWidget *clocker;
static GtkWidget * chatwindow[50];
static int chatwindownum = 0;
enum {
    COLUMN = 0,
    NUM_COLS
};

void on_changed(GtkWidget * widget, gpointer statusbar)
{
    GtkTreeIter iter;
    GtkTreeModel *model;
    char *value;


    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), &model, &iter)) {

        gtk_tree_model_get(model, &iter, COLUMN, &value, -1);
        gtk_statusbar_push(GTK_STATUSBAR(statusbar), gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), value), value);

		printf("%s\n",value);
		if(strcmp(value,"我的群")==0 ||strcmp(value,"我的好友")==0)
		{}
		else
		{
			//pthread_t id2;
			//pthread_create(&id2,0,chat,value);
			//gdk_threads_enter();
			//if(fork() == 0)
				chat(value);
			//gdk_threads_leave();
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

    treestore = gtk_tree_store_new(NUM_COLS, G_TYPE_STRING);

    gtk_tree_store_append(treestore, &toplevel, NULL);
    gtk_tree_store_set(treestore, &toplevel, COLUMN, "我的群", -1);
    gtk_tree_store_append(treestore, &child, &toplevel);
    gtk_tree_store_set(treestore, &child, COLUMN, "public", -1);
	g_signal_connect(GTK_OBJECT(treestore),"clicked",G_CALLBACK(gtk_main_quit),NULL);

    gtk_tree_store_append(treestore, &toplevel, NULL);
    gtk_tree_store_set(treestore, &toplevel, COLUMN, "我的好友", -1);
	//char list[];
    gtk_tree_store_append(treestore, &child, &toplevel);
    gtk_tree_store_set(treestore, &child, COLUMN, "hedong", -1);
    gtk_tree_store_append(treestore, &child, &toplevel);
    gtk_tree_store_set(treestore, &child, COLUMN, "123", -1);
    gtk_tree_store_append(treestore, &child, &toplevel);
    gtk_tree_store_set(treestore, &child, COLUMN, "东海", -1);

    return GTK_TREE_MODEL(treestore);
}

static GtkWidget *create_view_and_model(void)
{
    GtkTreeViewColumn *col;
    GtkCellRenderer *renderer;
    GtkWidget *view;
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

    return view;
}
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
    GtkWidget *view;
    GtkTreeSelection *selection;
    GtkWidget *statusbar,*label;
    GtkWidget *vbox,*hbox,*vbox2;
    GtkWidget *sep1,*sep2;
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

	view = create_view_and_model();
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