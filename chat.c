#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#define GTK_ENABLE_BROKEN
extern char usrName[20];
extern int cfd;
extern char real_result[50][200];
extern int flags;
extern int chatwindownum;
extern char chatlist[20][20];
extern int count;
extern char chathistorybuffer[20][51200];
static char name[20];
char chatlistfoucs[20];

GtkWidget * ChatWindow;
static int mainwindownum = 0;
//static GtkWidget *sub_window,*keyword_window;
//gboolean ishide=FALSE;//是否隐藏 
//定义文本缓冲区
GtkTextBuffer *buffer1;
GtkTextBuffer *buffer;
GtkTextBuffer *KeyBuffer;
GtkWidget *list,*entry;
GtkWidget *liaolabel;
GtkTreeSelection *selection;
GtkWidget *liaolist;
GtkWidget *keyword_text;
//定义文本缓冲区的迭代，用于替代缓冲区的某一位置，用来插入数据
GtkTextIter start,end;


/*----------------------list operation start------------------------------------*/

enum
{
  LIST_ITEM = 0,
  N_COLUMNS
};
static void init_list(GtkWidget *list)
{

  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkListStore *store;

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("List Items",renderer, "text", LIST_ITEM, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);
  store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
  gtk_tree_view_set_model(GTK_TREE_VIEW(list),GTK_TREE_MODEL(store));

  g_object_unref(store);
}GtkTextIter iter; 
void add_to_list(GtkWidget *list, const gchar *str)
{
  GtkListStore *store;
  GtkTreeIter iter;
  strcpy(chatlistfoucs,str);

  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, LIST_ITEM, str, -1);
}


void  click_list(GtkWidget *widget, gpointer label) 
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  char *value;

  if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), &model, &iter)) {
    gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
    gtk_label_set_text(GTK_LABEL(label), value);
	printf("%s\n",value);
	
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buffer),&start,&end);
	gtk_text_buffer_delete(GTK_TEXT_BUFFER(buffer), &start, &end);
	
	int chatlistnum = 0; 
	for(chatlistnum = 0; chatlistnum < count; chatlistnum ++)
	{
		if(strcmp(value, chatlist[chatlistnum]) == 0)
		{
			break;
		}
	}
	g_print("新用户：\n");
	gtk_text_buffer_insert(buffer,&start,chathistorybuffer[chatlistnum],-1);
	strcpy(chatlistfoucs, value);
    g_free(value);
  }

}

/*-----------------------list operation end--------------------------------*/

GtkTextBuffer *buffer;

void on_quit(GtkWidget *widget, gpointer data)
{
	close(cfd);
	chatwindownum = 0;
	//gtk_main_quit();
	gtk_widget_hide(widget);
}


//文本可编辑的实现
void on_check1_clicked(GtkToggleButton  *check,gpointer text)
{
	gtk_text_view_set_editable(GTK_TEXT_VIEW(text),check->active);
}

//文本可换行的实现
void on_check2_clicked(GtkToggleButton  *check,gpointer text)
{
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text),check->active);
	//gprint("d%",check->active);
}


//
void on_search_clicked(GtkWidget *widget, gpointer data)
{
	GtkTextBuffer *buffertmp;
	GtkTextIter	starttmp,endtmp;
	GtkTextIter iter; 
    int i = 0;
	char content[30];
	strcpy(content, gtk_entry_get_text(GTK_ENTRY(data)));
	g_print("entry: %s\n", content);
	bzero(real_result,50*200);
	select_chat_content(0, usrName, NULL, content);
	
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(KeyBuffer),&starttmp,&endtmp);
	gtk_text_buffer_delete(GTK_TEXT_BUFFER(KeyBuffer),&starttmp,&endtmp);
    while(i < flags)
	{
		//KeyBuffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(text));
		gtk_text_buffer_get_end_iter(KeyBuffer,&iter); 
    	gtk_text_buffer_insert(KeyBuffer,&iter,real_result[i],-1);
    
		g_printf("00000%s\n",real_result[i]);
		i++;
	}
}

//查询按钮函数
GtkWidget *create_keyword_window()
{
	GtkWidget *window1,*vbox,*hbox,*frame,*vbar,*label,*button1,*entry,*fix;
	window1=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	//g_signal_connect(G_OBJECT(window1),"delete_event",G_CALLBACK(on_sub_delete),window1);
    gtk_window_set_title(GTK_WINDOW(window1),"Check by Keywords");
    gtk_widget_set_size_request(window1,260,360);
    //gtk_window_set_position(GTK_WINDOW(window1),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window1),5); 
	frame=gtk_frame_new("关键字查询");
    gtk_container_add(GTK_CONTAINER(window1),frame); 
	gtk_widget_set_size_request(frame,260,360);GtkWidget *liaolist,*liaolabel;
	GtkTreeSelection *selection;
	vbox=gtk_vbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(frame),vbox); 
	hbox=gtk_hbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,2);
	button1=gtk_button_new_with_label("查找");
	gtk_widget_set_size_request(button1,120,30);
	fix=gtk_fixed_new();
    //gtk_widget_set_size_request(fix,20,20);
	gtk_fixed_put(GTK_FIXED(fix),button1,100,0);
	
	gtk_box_pack_start(GTK_BOX(vbox),fix,FALSE,FALSE,5);	
	label=gtk_label_new("输入关键字");
	gtk_box_pack_start(GTK_BOX(hbox),label,FALSE,FALSE,2);	
	entry=gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(hbox),entry,FALSE,FALSE,2);	
	vbar=gtk_scrolled_window_new(NULL,NULL); 
	gtk_container_add(GTK_CONTAINER(vbox),vbar);	
	//gtk_box_pack_start(GTK_BOX(vbox1),view,TRUE,TRUE,5); 
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(vbar),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC); 
    g_signal_connect(G_OBJECT(button1),"clicked",G_CALLBACK(on_search_clicked),entry);
	//创建文本视图，加入滚动窗口
	keyword_text=gtk_text_view_new(); 
    gtk_container_add(GTK_CONTAINER(vbar),keyword_text); 
    KeyBuffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(keyword_text));
    
	//是文本视图不可编辑
    gtk_text_view_set_editable(GTK_TEXT_VIEW(keyword_text),FALSE);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(keyword_text),GTK_WRAP_WORD);	
	
	gtk_widget_show_all(window1);
	return window1;
}


//历史按钮函数
GtkWidget *create_sub_window()
{	
	int i = 0;
	//char **history = (char **)malloc(50);
	//for(i = 0; i < 50; i++)
	//	history[i] = (char *)malloc(200);
	GtkWidget *window2,*frame,*vbar,*text;
	window2=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	//g_signal_connect(G_OBJECT(window2),"delete_event",G_CALLBACK(on_sub_delete),window2);
    gtk_window_set_title(GTK_WINDOW(window2),"Check by Keywords");
    gtk_widget_set_size_request(window2,200,380);
    //gtk_window_set_position(GTK_WINDOW(window1),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window2),5); 
	frame=gtk_frame_new("聊天记录");
	gtk_container_add(GTK_CONTAINER(window2),frame); 
	gtk_widget_set_size_request(frame,200,380);
	//创建滚动窗口并设定滚动条为自动显示   
    vbar=gtk_scrolled_window_new(NULL,NULL); 
	gtk_container_add(GTK_CONTAINER(frame),vbar);	
	//gtk_box_pack_start(GTK_BOX(vbox1),view,TRUE,TRUE,5); 
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(vbar),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC); 
	//创建文本视图，加入滚动窗口
	text=gtk_text_view_new(); 
	//是文本视图不可编辑
   	gtk_text_view_set_editable(GTK_TEXT_VIEW(text),FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text),GTK_WRAP_WORD);
    gtk_container_add(GTK_CONTAINER(vbar),text); 
	select_chat_content(0, usrName, name, NULL);
	GtkTextIter iter; 
	static GtkTextBuffer *buffer;
	while(i < flags)
	{
		buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(text));
		gtk_text_buffer_get_end_iter(buffer,&iter); 
    	gtk_text_buffer_insert(buffer,&iter,real_result[i],-1);
    
    	
		
		g_printf("00000%s\n",real_result[i]);
		i++;
	}
	
	gtk_widget_show_all(window2);
	return window2;
	
	//sprintf(entrybuf,"%s\n",history[0]);
	//g_print("send: %s\n", entrybuf);
	//gtk_entry_set_text(GTK_ENTRY(entry),"");
	//gtk_text_buffer_get_end_iter(buffer,&end);
	//gtk_text_buffer_insert_with_tags_by_name(buffer,&iter,"Dialog information\n\n ",-1,"orange_foreground",NULL);
	//gtk_text_buffer_create_tag(buffer,"red_foreground","foreground","red",NULL);//最后一位参数必须为空
	//gtk_text_buffer_insert(buffer,&end,entrybuf,-1);
	
	//int i = 0;
	/*while(history[i] != NULL)
	{
		g_print("%s\n", history[i++]);
	}*/
}

//发送按钮函数
void on_sent_clicked(GtkWidget *button,gpointer* name1)
{	
	g_printf("on_sent_clicked: %s\n", name);
	gchar entrybuf[1024];
	gchar entrybuf1[1024];
	bzero(entrybuf,1024);
	const char *messages;
	char tem[51200];
	//GtkTextIter *iter;
	messages = gtk_entry_get_text(GTK_ENTRY(entry));
	sprintf(entrybuf,"%s\n",messages);
	//***********************************************************************************************
	int chatlistnum = 0; 
	for(chatlistnum = 0; chatlistnum < count; chatlistnum ++)
	{
		if(strcmp(chatlistfoucs, chatlist[chatlistnum]) == 0)
		{
			break;
		}
	}
	
	sprintf(entrybuf1,"%s: %s\n",usrName, entrybuf);
				
	strcpy(tem, chathistorybuffer[chatlistnum]);
	strcat(tem, entrybuf1);
	//bzero(chatlist[chatlistnum], sizeof(chatlist[chatlistnum]));
	strcpy(chathistorybuffer[chatlistnum], tem);
	//**********************************************************************************************
	//g_print("send: %s\n", entrybuf);
	gtk_entry_set_text(GTK_ENTRY(entry),"");
	gtk_text_buffer_get_end_iter(buffer,&end);
	//gtk_text_buffer_insert_with_tags_by_name(buffer,&iter,"Dialog information\n\n ",-1,"orange_foreground",NULL);
	//gtk_text_buffer_create_tag(buffer,"red_foreground","foreground","red",NULL);//最后一位参数必须为空
	gtk_text_buffer_insert(buffer,&end,entrybuf,-1);
	g_print("userName: %s\n", usrName);
	insert_chat_data(0, usrName, chatlistfoucs, entrybuf);
	g_printf("name: %s\n entrybuf: %s\n", chatlistfoucs, entrybuf);
	char ss[20];
	sprintf(ss,"%s",chatlistfoucs);
	g_print("ss  is %s",ss);
	//if(strcmp(entrybuf, "") != 0)
	if(strcmp(chatlistfoucs, "public") != 0)
	{
		tcp_send(ss, (char *)entrybuf);
		//bzero(entrybuf, sizeof(entrybuf));
	}
	else
	{
		tcp_send("public", (char *)entrybuf);
	}
}

//关闭按钮函数
void on_close_clicked(GtkWidget *button,gpointer data)
{
	//gtk_main_quit();
	gtk_widget_hide(data);
}


//创建主窗口
GtkWidget *create_main_window(gchar *name)
{
	GtkWidget *window,*hbox1,*hbox2,*hbox3,*vbox1,*vbox2,*view,*view1,*frame,*frame1,*frame2,*frame3,*listbox,*listframe, *listview;
    GtkWidget *text1,*text,*sep,*check1,*check2,*button4,*button5,*send_button,*close_button,*image;
	//gtk_init(&argc,&argv);
	//创建窗口并设置大小以及位置
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(on_quit),NULL);
    gtk_window_set_title(GTK_WINDOW(window),name);
    gtk_widget_set_size_request(window,600,360);
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	//g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(on_delete_event),NULL);
    gtk_container_set_border_width(GTK_CONTAINER(window),5); 
	
//.............................总体框架划分.............................................	
	//创建纵向组装盒容器并添加进入窗口   
    hbox1=gtk_hbox_new(FALSE,0); 
	gtk_container_add(GTK_CONTAINER(window),hbox1);
	//创建两个横向组装盒容器//添加进入窗口
	vbox1=gtk_vbox_new(FALSE,0); 
	vbox2=gtk_vbox_new(FALSE,0);  
	listbox=gtk_vbox_new(FALSE,0);
	//将横向组装盒添加进入窗口
	gtk_box_pack_start(GTK_BOX(hbox1),listbox,TRUE,TRUE,2);
	gtk_box_pack_start(GTK_BOX(hbox1),vbox1,TRUE,TRUE,2);
	gtk_box_pack_start(GTK_BOX(hbox1),vbox2,TRUE,TRUE,2);
//...............................第一列设置...........................................
	
	
	
	
	/*---------------------------------list start----------------------------------------*/
	
	listframe=gtk_frame_new("聊天列表");
	gtk_widget_set_size_request(listframe,100,360);
	gtk_box_pack_start(GTK_BOX(listbox),listframe,TRUE,TRUE,0);
	
	listview=gtk_scrolled_window_new(NULL,NULL);
	gtk_container_add(GTK_CONTAINER(listframe),listview);	
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(listview),GTK_POLICY_AUTOMATIC,GTK_POLICY_ALWAYS); 

	liaolist = gtk_tree_view_new();
  	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(liaolist), FALSE); 
	gtk_container_add(GTK_CONTAINER(listview),liaolist); 
	
	liaolabel = gtk_label_new("");
  	gtk_label_set_justify(GTK_LABEL(liaolabel), GTK_JUSTIFY_LEFT);
  	gtk_container_add(GTK_CONTAINER(listview),liaolabel);
    
	init_list(liaolist);
  	add_to_list(liaolist, name);
	
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(liaolist));
	g_signal_connect(selection, "changed", G_CALLBACK(click_list), liaolabel);
	
/*---------------------------------list end----------------------------------------*/
	
	

	//................文本视图及内部设置.......................
	//创建规范框架
	frame2=gtk_frame_new("");
	gtk_box_pack_start(GTK_BOX(vbox1),frame2,TRUE,TRUE,5);
	gtk_widget_set_size_request(frame2,230,230);
	
	//创建滚动窗口并设定滚动条为自动显示   
        view=gtk_scrolled_window_new(NULL,NULL); 
	gtk_container_add(GTK_CONTAINER(frame2),view);	
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(view),GTK_POLICY_AUTOMATIC,GTK_POLICY_ALWAYS); 
	
	//创建文本视图，加入滚动窗口
	text=gtk_text_view_new(); 
    gtk_container_add(GTK_CONTAINER(view),text); 
	//是文本视图不可编辑
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text),FALSE); 
    //取得文本显示的缓冲区
    buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(text)); 
	//创建前景标记
	gtk_text_buffer_create_tag(buffer,"red_foreground","foreground","red",NULL);//最后一位参数必须为空
	//创建背景标记
	//创建字体标记
	gtk_text_buffer_create_tag(buffer,"paktypenaqsh","family","PakTypeNaqsh",NULL);//最后一位参数必须为空
	gtk_text_buffer_create_tag(buffer,"italic","family","Italic",NULL);//最后一位参数必须为空
	//边距标记
	//居左设置
	gtk_text_buffer_create_tag(buffer,"heading","justification",GTK_JUSTIFY_LEFT,NULL);//最后一位参数必须为空
	//居中设置	
	gtk_text_buffer_create_tag(buffer,"center","justification",GTK_JUSTIFY_CENTER,NULL);//最后一位参数必须为空
	//居右设置	
	gtk_text_buffer_create_tag(buffer,"right","justification",GTK_JUSTIFY_RIGHT,NULL);//最后一位参数必须为空
	//是否换行
	//gtk_text_view_create_tag(buffer,"no_wrap","wrap_mode",GTK_WRAP_NONE,NULL);//最后一位参数必须为空
       // gtk_text_buffer_create_tag(buffer,"no_wrap","wrap_mode",GTK_WRAP_WORD,NULL);//最后一位参数必须为空*/
       //取得缓冲区的尾部并插入普通文字
	gtk_text_buffer_get_end_iter(buffer,&end);
	gtk_text_buffer_insert(buffer,&end,"Supports ",-1);
	//从此处插入红色标记文字
	gtk_text_buffer_get_end_iter(buffer,&end);
	gtk_text_buffer_insert_with_tags_by_name(buffer,&end,"colored ",-1,"red_foreground",NULL);
	//插入普通文字
	gtk_text_buffer_get_end_iter(buffer,&end);
	gtk_text_buffer_insert(buffer,&end,"text and different ",-1);
	//插入字体标记文字
	gtk_text_buffer_get_end_iter(buffer,&end);
	gtk_text_buffer_insert_with_tags_by_name(buffer,&end,"fonts\n\n",-1,"paktypenaqsh",NULL);
	//从此处插入红色及字体标记文字
	gtk_text_buffer_get_end_iter(buffer,&end);
	gtk_text_buffer_insert_with_tags_by_name(buffer,&end,"The dilog started here:\n ",-1,"red_foreground","paktypenaqsh",NULL);
	gtk_text_buffer_get_end_iter(buffer,&end);

        //.............第二行设置........................... 
	//创建横向组装盒装入纵向组装盒1
        hbox2=gtk_hbox_new(TRUE,0);
	gtk_box_pack_start(GTK_BOX(vbox1),hbox2,TRUE,TRUE,2);
	

	//创建两个checkbutton,并加入到横向组合框2
        //编辑
        check1=gtk_check_button_new_with_label("可编辑");
	gtk_box_pack_start(GTK_BOX(hbox2),check1,TRUE,TRUE,2);
	g_signal_connect(G_OBJECT(check1),"toggled",G_CALLBACK(on_check1_clicked),text);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check1),FALSE);
       
	 //换行
	check2=gtk_check_button_new_with_label("可换行");
	gtk_box_pack_start(GTK_BOX(hbox2),check2,TRUE,TRUE,2);
	g_signal_connect(G_OBJECT(check2),"toggled",G_CALLBACK(on_check2_clicked),text);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check2),FALSE);
	
	//创建查询按钮
	button4=gtk_button_new_with_label("查询");
	gtk_box_pack_start(GTK_BOX(hbox2),button4,TRUE,TRUE,2);
	g_signal_connect(G_OBJECT(button4),"clicked",G_CALLBACK(create_keyword_window),NULL);
        //创建历史按钮
	button5=gtk_button_new_with_label("历史");
	gtk_box_pack_start(GTK_BOX(hbox2),button5,TRUE,TRUE,2);
	g_signal_connect(G_OBJECT(button5),"clicked",G_CALLBACK(create_sub_window),NULL);
	//创建纵向隔离线
	sep=gtk_hseparator_new();
	gtk_box_pack_start(GTK_BOX(vbox1),sep,TRUE,TRUE,0);
	
	//..........................第三行设置....................................
	//创建文本录入构建并装入到纵向组装盒1
	entry=gtk_entry_new();
	gtk_container_add(GTK_CONTAINER(vbox1),entry);
	//创建横向组装盒并装入纵向组装盒1
	hbox3=gtk_hbox_new(TRUE,0);
	//gtk_container_add(GTK_CONTAINER(frame1),hbox3);
	gtk_box_pack_start(GTK_BOX(vbox1),hbox3,TRUE,TRUE,0);

	//..................第三行设置.........................
	//创建发送和关闭按钮并装入到横向组装盒3
    send_button=gtk_button_new_with_label("发送");
	gtk_box_pack_start(GTK_BOX(hbox3),send_button,TRUE,TRUE,3);
	close_button=gtk_button_new_with_label("关闭");
	gtk_box_pack_start(GTK_BOX(hbox3),close_button,FALSE,TRUE,3);
	g_signal_connect(G_OBJECT(close_button),"clicked",G_CALLBACK(on_close_clicked),window);
	g_signal_connect(G_OBJECT(send_button),"clicked",G_CALLBACK(on_sent_clicked), (gpointer*)name);
	g_printf("mainwindow: %s\n", name);
//......................................第二列设置................................	
	//用于创建图片控件和端口显示
	image=gtk_image_new_from_file("./resource/2.jpg");
        frame=gtk_frame_new("");
	gtk_frame_set_shadow_type(GTK_FRAME(frame),GTK_SHADOW_ETCHED_OUT);
	gtk_widget_set_size_request(frame,135,135);	
	gtk_box_pack_start(GTK_BOX(vbox2),frame,TRUE,TRUE,0);
	gtk_container_add(GTK_CONTAINER(frame),image); 
	
	//创建滚动窗口并设定滚动条为自动显示   
        view1=gtk_scrolled_window_new(NULL,NULL); 
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(view1),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC); 
	gtk_widget_set_size_request(view1,100,70);
	
	//创建文本视图，加入滚动窗口
	text1=gtk_text_view_new(); 
        gtk_box_pack_start(GTK_BOX(vbox2),view1,TRUE,TRUE,5); 
        gtk_container_add(GTK_CONTAINER(view1),text1); 
        //取得文本视图的缓冲区
        buffer1=gtk_text_view_get_buffer(GTK_TEXT_VIEW(text1)); 
	//设置前景标记
	gtk_text_buffer_create_tag(buffer1,"orange_foreground","foreground","orange",NULL);//最后一位参数必须为空
	//插入颜色标记文字
	gtk_text_buffer_get_end_iter(buffer1,&start);
	gtk_text_buffer_insert_with_tags_by_name(buffer1,&start,"Dialog information\n\n ",-1,"orange_foreground",NULL);
    gtk_widget_show_all(window);
        
	return window;
}


GtkWidget* chat(gchar *name1)
{
	//gtk_init(&argc,&argv);
	
	strcpy(name, name1);
	g_print("newname: %s", name);
	
	ChatWindow=create_main_window(name);
	//g_printf("chat: %s\n", name);
	//gtk_main();
	return ChatWindow;
}
