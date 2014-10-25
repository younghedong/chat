main:main.c login.c
	gcc -o main main.c login.c `pkg-config --libs --cflags gtk+-2.0`
