default:	program

program:	Jogo.c
	gcc Jogo.c -o main -Wall `pkg-config --cflags --libs allegro-5 allegro_acodec-5 allegro_audio-5 allegro_color-5 allegro_dialog-5 allegro_font-5 allegro_image-5 allegro_main-5 allegro_memfile-5 allegro_physfs-5 allegro_primitives-5 allegro_ttf-5`

	
clean:
	-rm -f main.o
	-rm -f main
