main:main.c login.c insertdata.c selectdata.c informationdialog.c chat.c encrypt.c decrypt.c contact.c client.c
	gcc -g -o $@ $^ `pkg-config --libs --cflags gtk+-2.0 gthread-2.0` -I/usr/include/mysql -L/usr/lib/mysql -lmysqlclient -lpthread
	
server:server.c
	gcc -o server server.c -lpthread
clean:
	rm -f *.o  main *~
