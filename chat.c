#include <gtk/gtk.h>
#include <stdio.h>
#define GTK_ENABLE_BROKEN
extern char usrName[20];
extern int cfd;
extern char **real_result;
extern int flags;
static char name[20];

static GtkWidget * mainwindow[50];
static int mainwindownum = 0;
static GtkWidget *sub_window,*keyword_window;
gboolean ishide=FALSE;//是否隐藏 
//定义文本缓冲区
GtkTextBuffer *buffer1;
GtkTextBuffer *buffer;
GtkWidget *list,*entry;
//定义文本缓冲区的迭代，用于替代缓冲区的某一位置，用来插入数据
GtkTextIter start,end;

void on_quit(GtkWidget *widget, gpointer data)
{
	close(cfd);
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

//隐藏
void on_sub_delete(GtkWidget *window,GdkEvent *event,gpointer data)
{
	gtk_widget_hide(window);
	ishide=TRUE;
}

//
void on_search_clicked(GtkWidget *widget, gpointer data)
{
	char content[30];
	strcpy(content, gtk_entry_get_text(GTK_ENTRY(data)));
	g_print("entry: %s\n", content);
	select_chat_content(0, usrName, NULL, content);
	
}

//查询按钮函数
void create_keyword_window()
{
	GtkWidget *window1,*vbox,*hbox,*frame,*vbar,*text,*label,*button1,*entry,*fix;
	window1=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	//g_signal_connect(G_OBJECT(window1),"delete_event",G_CALLBACK(on_sub_delete),window1);
    gtk_window_set_title(GTK_WINDOW(window1),"Check by Keywords");
    gtk_widget_set_size_request(window1,260,360);
    //gtk_window_set_position(GTK_WINDOW(window1),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window1),5); 
	frame=gtk_frame_new("关键字查询");
    gtk_container_add(GTK_CONTAINER(window1),frame); 
	gtk_widget_set_size_request(frame,260,360);
	vbox=gtk_vbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(frame),vbox); 
	hbox=gtk_hbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,2);
	button1=gtk_button_new_with_label("查找");
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
	text=gtk_text_view_new(); 
    gtk_container_add(GTK_CONTAINER(vbar),text); 
	//是文本视图不可编辑
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text),FALSE);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text),GTK_WRAP_WORD);	
	gtk_widget_show_all(window1);
	keyword_window=window1;
}


//历史按钮函数
void create_sub_window()
{	
	int i = 0;
	//char **history = (char **)malloc(50);
	//for(i = 0; i < 50; i++)
	//	history[i] = (char *)malloc(200);
	GtkWidget *window2,*frame,*vbar,*text;
	window2=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window2),"delete_event",G_CALLBACK(on_sub_delete),window2);
    gtk_window_set_title(GTK_WINDOW(window2),"Check by Keywords");
    gtk_widget_set_size_request(window2,200,380);
    //gtk_window_set_position(GTK_WINDOW(window1),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window2),5); 
	frame=gtk_frame_new("聊天记录");
	gtk_container_add(GTK_CONTAINER(window2),frame); 
	//创建滚动窗口并设定滚动条为自动显示   
    vbar=gtk_scrolled_window_new(NULL,NULL); 
	gtk_container_add(GTK_CONTAINER(frame),vbar);	
	//gtk_box_pack_start(GTK_BOX(vbox1),view,TRUE,TRUE,5); 
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(vbar),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC); 
	//创建文本视图，加入滚动窗口
	text=gtk_text_view_new(); 
    gtk_container_add(GTK_CONTAINER(vbar),text); 
	select_chat_content(0, "hedong", "ren", NULL);
	GtkTextIter iter; 
	static GtkTextBuffer *buffer;
	while(i < flags)
	{
		buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(text));
		gtk_text_buffer_get_end_iter(buffer,&iter); 
    	gtk_text_buffer_insert(buffer,&iter,real_result[i],-1);
    
    	gtk_container_add(GTK_CONTAINER(vbar),text); 
		//是文本视图不可编辑
    	gtk_text_view_set_editable(GTK_TEXT_VIEW(text),FALSE);
		gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text),GTK_WRAP_WORD);
		g_printf("00000%s\n",real_result[i]);
		i++;
	}
	
	gtk_widget_show_all(window2);
	keyword_window=window2;
	
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
	bzero(entrybuf,1024);
	const char *messages;
	//GtkTextIter *iter;
	messages = gtk_entry_get_text(GTK_ENTRY(entry));
	sprintf(entrybuf,"%s\n",messages);
	
	//g_print("send: %s\n", entrybuf);
	gtk_entry_set_text(GTK_ENTRY(entry),"");
	gtk_text_buffer_get_end_iter(buffer,&end);
	//gtk_text_buffer_insert_with_tags_by_name(buffer,&iter,"Dialog information\n\n ",-1,"orange_foreground",NULL);
	//gtk_text_buffer_create_tag(buffer,"red_foreground","foreground","red",NULL);//最后一位参数必须为空
	gtk_text_buffer_insert(buffer,&end,entrybuf,-1);
	g_print("userName: %s\n", usrName);
	insert_chat_data(0, usrName, name, entrybuf);
	g_printf("name: %s\n entrybuf: %s\n", name, entrybuf);
	char ss[20];
	sprintf(ss,"%s",name);
	g_print("ss  is %s",ss);
	tcp_send(ss, (char *)entrybuf);
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
	GtkWidget *window,*hbox1,*hbox2,*hbox3,*vbox1,*vbox2,*view,*view1,*frame,*frame1,*frame2,*frame3;
    GtkWidget *text1,*text,*sep,*check1,*check2,*button4,*button5,*send_button,*close_button,*image,*image1;
	//gtk_init(&argc,&argv);
	//创建窗口并设置大小以及位置
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(on_quit),NULL);
    gtk_window_set_title(GTK_WINDOW(window),name);
    gtk_widget_set_size_request(window,500,360);
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
	//vbox3=gtk_vbox_new(FALSE,0);
	//将横向组装盒添加进入窗口
	//gtk_box_pack_start(GTK_BOX(hbox1),vbox3,TRUE,TRUE,2);
	gtk_box_pack_start(GTK_BOX(hbox1),vbox1,TRUE,TRUE,2);
	gtk_box_pack_start(GTK_BOX(hbox1),vbox2,TRUE,TRUE,2);
//...............................第一列设置...........................................
	
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
	image=gtk_image_new_from_file("xiaoxiong.jpg");
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
	
	mainwindow[mainwindownum]=create_main_window(name);
	g_printf("chat: %s\n", name);
	//gtk_widget_show(main_window);
	//gtk_main();
	return mainwindow[mainwindownum++];
}
