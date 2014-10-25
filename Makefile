main:main.c login.c insertdata.c
	gcc -o $@ $^ `pkg-config --libs --cflags gtk+-2.0` -I/usr/include/mysql -L/usr/lib/x86_64-linux-gnu/libmysqlclient.a -lmysqlclient
	
clean:
	rm -f *.o, main
