/*
 ============================================================================
 Name        : Jogo.c
 Author      : Matheus Andrade Alves
 Version     : 1.0
 Last Mod.	 : 20/12/2016
 ============================================================================
 */

/*
 * Animar cenario
 * Adicionar sons ao coletar moedas e etc
 * Alinhas bitmaps
 */

#include <allegro5/allegro_acodec.h>        // Inclui biblioteca para audio
#include <allegro5/allegro_audio.h>         // Inclui biblioteca para audio
#include <allegro5/allegro_font.h>          // Inclui biblioteca para fontes
#include <allegro5/allegro_image.h>         // Inclui biblioteca para imagens
#include <allegro5/allegro_primitives.h>    // Inclui biblioteca para desenhos basicos
#include <allegro5/allegro_ttf.h>           // Inclui biblioteca para fontes
#include <allegro5/bitmap.h>				// Inclui biblioteca para bitmap
#include <stdbool.h>						// Inclui biblioteca para funcoes booleanas
#include <stdio.h>                          // Inclui biblioteca padrao para console
#include <stdlib.h>                         // Inclui biblioteca para funcoes rand
#include <time.h>

#define RESOLUCAO_X	1024
#define RESOLUCAO_Y	640
#define FPS			60
#define	INICIO		0
#define JOGO		1
#define	FIM			2
#define	NUM_INIM	5
#define NUM_OBJ		5
#define NUM_TIRO	3
#define LANE_HEIGHT	65

#define FONT					"fontes/8-Bit Madness.ttf"
#define MUSICA					"musica/bg.wav"
#define	ARQUIVO					"rank.txt"
#define BITMAP_PERSONAGEM		"sprites/marspeople.png"
#define BITMAP_PERSONAGEM_EXP	"sprites/marspeople_exploding.png"
#define BITMAP_PERSONAGEM_HIT	"sprites/marspeople_hit.png"
#define BITMAP_PERSONAGEM_SHOOT	"sprites/marspeople_shooting.png"
#define BITMAP_PERSONAGEM_DYING	"sprites/marspeople_dying.png"
#define	BITMAP_MOEDAS			"sprites/moedas.png"
#define BITMAP_REBEL_WALKING	"sprites/rebel_walking.png"
#define BITMAP_REBEL_SHOOTING	"sprites/rebel_shooting.png"
#define BITMAP_REBEL_ATTACKING	"sprites/rebel_attacking.png"
#define BITMAP_REBEL_DYING		"sprites/rebel_dying.png"
#define	BITMAP_TIRO				"sprites/tiro.png"
#define BITMAP_PERSONAGEM_TIRO	"sprites/tiroPersonagem.png"
#define BITMAP_LANE				"sprites/lane.png"
#define BITMAP_MARS				"sprites/jogo.png"
#define BITMAP_INICIO_TOP		"sprites/bg_menu_top.png"
#define BITMAP_INICIO_BOTTOM	"sprites/bg_menu_bottom.png"
#define	BITMAP_FIM				"sprites/bg2.png"
#define BITMAP_CORACAO			"sprites/heart.png"

struct Teclado{
	bool w, a, s, d, cima, baixo, esquerda, direita, espaco, esc, enter;
	unsigned int timer;
};

struct Personagem{
	float x, y;
	unsigned int pontos;
	unsigned int vida;
	unsigned int frame;
	char nome[30];
	unsigned char lane;
	bool ativo;
	bool exp;
	bool hit;
	bool shoot;
	bool morrendo;

	ALLEGRO_BITMAP *sprite;
	float frameWidth;
	float frameHeight;
	unsigned int maxFrame;

	ALLEGRO_BITMAP *spriteExp;
	float frameWidthExp;
	float frameHeightExp;
	unsigned int maxFrameExp;

	ALLEGRO_BITMAP *spriteHit;
	float frameWidthHit;
	float frameHeightHit;
	unsigned int maxFrameHit;

	ALLEGRO_BITMAP *spriteShoot;
	float frameWidthShoot;
	float frameHeightShoot;
	unsigned int maxFrameShoot;

	ALLEGRO_BITMAP *spriteMorrendo;
	float frameWidthMorrendo;
	float frameHeightMorrendo;
	unsigned int maxFrameMorrendo;

};

struct Inimigos{
	float x, y;
	float frameWidth;
	float frameHeight;
	unsigned int frame;
	unsigned int maxFrame;

	float frameWidthAcao;
	float frameHeightAcao;
	unsigned int frameAcao;
	unsigned int maxFrameAcao;

	float frameWidthAtac;
	float frameHeightAtac;
	unsigned int frameAtac;
	unsigned int maxFrameAtac;

	float frameWidthMorrendo;
	float frameHeightMorrendo;
	unsigned int frameMorrendo;
	unsigned int maxFrameMorrendo;

	float velocidade;
	ALLEGRO_BITMAP *sprite;
	ALLEGRO_BITMAP *spriteAcao;
	ALLEGRO_BITMAP *spriteAtac;
	ALLEGRO_BITMAP *spriteMorrendo;
	bool ativo;
	bool tiro;
	bool atacou;
	bool morrendo;
};

struct Objetos{
	float x, y;
	float frameWidth;
	float frameHeight;
	unsigned int frame;
	unsigned int frameY;
	unsigned int maxFrame;		//Quantidade de frames por linha
	unsigned int rollFrame;		//Quantidade de linhas
	float velocidade;
	ALLEGRO_BITMAP *sprite;
	bool ativo;
};

struct Tiro{
	float x, y;
	float frameWidth;
	float frameHeight;
	unsigned int frame;
	unsigned int maxFrame;
	float velocidade;
	ALLEGRO_BITMAP *sprite;
	bool ativo;
};

struct Cenario{
	ALLEGRO_BITMAP *lane;
	float laneWidth;
	float laneHeight;
	float laneX;

	ALLEGRO_BITMAP *jogo;
	float jogoWidth;
	float jogoHeight;
	float jogoX;

	ALLEGRO_BITMAP *inicioTop;
	float inicioWidthTop;
	float inicioHeightTop;
	float inicioX;

	ALLEGRO_BITMAP *inicioBottom;
	float inicioWidthBottom;
	float inicioHeightBottom;

	ALLEGRO_BITMAP *fim;
	float fimWidth;
	float fimHeight;
	float fimX;

	ALLEGRO_BITMAP *vida;
	float vidaWidth;
	float vidaHeight;

};

struct Rank{
	char nome[30];
	unsigned int pontos;
};

void desenhaPersonagem(struct Personagem *personagem);
void desenhaObjeto(struct Objetos objeto[]);
void desenhaInimigos(struct Inimigos inimigos[]);
void moveInimigos(struct Inimigos inimigos[], struct Tiro tiroInimigo[]);
void checaColisao(struct Personagem *personagem, struct Objetos objeto[], struct Inimigos inimigos[], struct Tiro tiroInimigo[], struct Tiro tiroPersonagem[]);
void moveObjetos(struct Objetos objeto[]);
void moveTiro(struct Tiro tiroInimigo[], struct Tiro tiroPersonagem[]);
void desenhaTiro(struct Tiro tiroInimigo[], struct Tiro tiroPersonagem[]);
void movePersonagem(struct Teclado *teclado, struct Personagem *personagem, struct Tiro tiroPersonagem[]);
bool initEstruturas(struct Teclado *teclado, struct Personagem *personagem, struct Objetos objeto[], struct Inimigos inimigos[], struct Tiro tiroInimigo[], struct Tiro tiroPersonagem[], struct Cenario *cenario);
bool readString(ALLEGRO_EVENT *ev, char size, char *string);
void checaTeclado(ALLEGRO_EVENT *ev, struct Teclado *teclado);
void animacao(unsigned char *frameCount, struct Personagem *personagem, struct Objetos objeto[], struct Inimigos inimigos[], struct Tiro tiroInimigo[], struct Tiro tiroPersonagem[]);
void desenhaCenario(struct Cenario *cenario, int stateMachine, int vida);
void leituraArquivo(struct Personagem *personagem, struct Rank top5[]);

int main(void) {

	/*
	 * Declaração dos ponteiros das estruturas do Allegro
	 */
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_TIMER *temporizador = NULL;
    ALLEGRO_EVENT_QUEUE *eventQueue = NULL;
    ALLEGRO_FONT *fonte = NULL;
    ALLEGRO_SAMPLE *musica = NULL;

    /*
     * Inicialização e teste
     */
    if(!al_init()){											// Inicializa o allegro
    	printf("Falha no al_init()\n");						// Retorna erro caso falhe
    	return EXIT_FAILURE;
    }

    display = al_create_display(RESOLUCAO_X, RESOLUCAO_Y);  // Configura display
    al_set_window_title(display, "Jogão");          		// Muda o nome da janela
    if(!display){											// Retorna erro se o display não for criado com sucesso
        printf("Falha ao inicializar o display!\n");
    	return EXIT_FAILURE;
    }

    temporizador = al_create_timer(1.0 / FPS);				// Configura o timer
    if(!temporizador){										// Retorna erro se o temporizador não for criado com sucesso
    	printf("Falha ao inicializar o temporizador!\n");
    	return EXIT_FAILURE;
    }

    eventQueue = al_create_event_queue();					// Configura a fila de eventos
    if(!eventQueue){										// Retorna erro se a fila de eventos não for criada com sucesso
    	printf("Falha ao inicializar a fila de eventos!\n");
    	return EXIT_FAILURE;
    }

    al_init_font_addon();                                   // Inicializa as fontes
	if(!al_init_ttf_addon()){                               // Inicializa fontes ttf, retorna erro se falhar
		printf("Falha ao inicializar a fonte!\n");
		return EXIT_FAILURE;
	}

	fonte = al_load_font(FONT, 32, 0);               // Declara a fonte
	if(!fonte){                                             // Retorna erro se a fonte não for criada com sucesso
		printf("Falha ao carregar a fonte: %s!\n", FONT);
		return EXIT_FAILURE;
	}
	if(!al_init_image_addon()){                             // Iniciliza as imagens, retorna erro se falhar
		printf("Falha ao inicializar as imagens!\n");
		return EXIT_FAILURE;
	}
	if(!al_init_primitives_addon()){                        // Inicializa as figuras primitivas, retorna erro se falhar
		printf("Falha ao inicializar os primitivos!\n");
		return EXIT_FAILURE;
	}

	if(!al_install_keyboard()){                              // Inicializa o teclado, retorna erro se falhar
		printf("Falha ao inicializar o teclado!\n");
		return EXIT_FAILURE;
	}

	if(!al_install_audio()){								// Inicializa o audio, retorna erro se falhar
		printf("Falha ao inicializar o audio!\n");
		return EXIT_FAILURE;
	}

	if(!al_init_acodec_addon()){							// Inicializa os codecs de audio, retorna erro se falhar
		printf("Falha ao inicializar os codecs de audio!\n");
		return EXIT_FAILURE;
	}

	if(!al_reserve_samples(1)){								// Inicializa o audio,retorna erro se falar
		printf("Falha ao inicializar o audio!\n");
		return EXIT_FAILURE;
	}

	musica = al_load_sample(MUSICA);				// Adiciona música
	if(musica == NULL){
		printf("Não foi possível carregar a música: %s\n", MUSICA);
		return EXIT_FAILURE;
	}

	al_play_sample(musica, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);	// Toca a música

	// Registra as fontes de evento para a fila de eventos
	al_register_event_source(eventQueue, al_get_display_event_source(display));
	al_register_event_source(eventQueue, al_get_timer_event_source(temporizador));
	al_register_event_source(eventQueue, al_get_keyboard_event_source());

	al_start_timer(temporizador);							// Começa o temporizador

	/*
	 * Declaração das variaveis e estruturas
	 */
	bool redesenha = false;
	unsigned char frameCount = 0;
	unsigned char stateMachine = INICIO;

	struct Teclado teclado;
	struct Personagem personagem;
	struct Objetos objeto[NUM_OBJ];
	struct Inimigos inimigos[NUM_INIM];
	struct Tiro tiroInimigo[NUM_INIM];
	struct Tiro tiroPersonagem[NUM_TIRO];
	struct Cenario cenario;
	struct Rank top5[5];

	if(initEstruturas(&teclado, &personagem, objeto, inimigos, tiroInimigo, tiroPersonagem, &cenario)){
		printf("Falha ao iniciar as estruturas!\n");
		return EXIT_FAILURE;
	}

	/*
	 * Loop Allegro
	 */
	while(1){
		ALLEGRO_EVENT ev;							// Cria uma estrutura de eventos
		al_wait_for_event(eventQueue, &ev);			// Aguarda por um eventos

		if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)	// Se o botão de fechar for clicado, saí do loop
		    break;
		else if(ev.type == ALLEGRO_EVENT_TIMER){
		    redesenha = true;
		    animacao(&frameCount, &personagem, objeto, inimigos, tiroInimigo, tiroPersonagem);
		}
		/*
		 * Funções
		 */
		switch(stateMachine){
		case INICIO:
			if(readString(&ev, 30, personagem.nome))
				stateMachine = JOGO;
			break;
		case JOGO:
			checaTeclado(&ev, &teclado);
			movePersonagem(&teclado, &personagem, tiroPersonagem);
			moveObjetos(objeto);
			moveInimigos(inimigos, tiroInimigo);
			moveTiro(tiroInimigo, tiroPersonagem);
			if(personagem.morrendo == false)
				checaColisao(&personagem, objeto, inimigos, tiroInimigo, tiroPersonagem);
			break;
		case FIM:
			checaTeclado(&ev, &teclado);
			break;
		}
		/*
		 * Desenha
		 */
		if(redesenha && al_is_event_queue_empty(eventQueue)){
			redesenha = false;

			switch(stateMachine){
			case INICIO:
				desenhaCenario(&cenario, INICIO, personagem.vida);
				al_draw_text(fonte, al_map_rgb(255, 255, 255), RESOLUCAO_X/2, RESOLUCAO_Y/2, ALLEGRO_ALIGN_CENTRE, "Digite um nome:");
				al_draw_textf(fonte, al_map_rgb(255, 255, 255), RESOLUCAO_X/2, (RESOLUCAO_Y/2) + 30, ALLEGRO_ALIGN_CENTRE, "%s", personagem.nome);
				break;
			case JOGO:
				desenhaCenario(&cenario, JOGO, personagem.vida);
				desenhaPersonagem(&personagem);
				desenhaObjeto(objeto);
				desenhaInimigos(inimigos);
				desenhaTiro(tiroInimigo, tiroPersonagem);
				if(personagem.ativo == false){
					leituraArquivo(&personagem, top5);
					stateMachine = FIM;
				}
				al_draw_textf(fonte, al_map_rgb(255, 255, 255), 10, 50, ALLEGRO_ALIGN_LEFT, "Pontos: %d", personagem.pontos);
				break;
			case FIM:
				desenhaCenario(&cenario, FIM, personagem.vida);
				al_draw_text(fonte, al_map_rgb(255, 255, 255), RESOLUCAO_X/2, RESOLUCAO_Y/2 - 60, ALLEGRO_ALIGN_CENTRE, "Top 5");
				al_draw_text(fonte, al_map_rgb(255, 255, 255), RESOLUCAO_X/2, RESOLUCAO_Y/2 + 200, ALLEGRO_ALIGN_CENTRE, "Pressione ENTER para reiniciar");
				for(int i = 0; i < 5; i++){
					al_draw_textf(fonte, al_map_rgb(255, 255, 255), RESOLUCAO_X/4, RESOLUCAO_Y/2 + (i*20), ALLEGRO_ALIGN_LEFT, "Nome: %s", top5[i].nome);
					al_draw_textf(fonte, al_map_rgb(255, 255, 255), RESOLUCAO_X/2 + 100, RESOLUCAO_Y/2 + (i*20), ALLEGRO_ALIGN_LEFT, "Pontos: %d", top5[i].pontos);
				}
				if(teclado.enter == true){
					if(initEstruturas(&teclado, &personagem, objeto, inimigos, tiroInimigo, tiroPersonagem, &cenario)){
						printf("Falha ao iniciar as estruturas!\n");
						return EXIT_FAILURE;
					}
					stateMachine = INICIO;
				}
				break;
			}

			al_flip_display();
			switch(stateMachine){
			case INICIO:
				al_clear_to_color(al_map_rgb(0, 0, 127));
				break;
			case JOGO:
				al_clear_to_color(al_map_rgb(0, 32, 0));
				break;
			case FIM:
				al_clear_to_color(al_map_rgb(0, 0, 0));
				break;
			}
		}
	}

	/*
	 * Destroi as estruturas
	 */
	al_destroy_event_queue(eventQueue);
	al_destroy_display(display);
	al_destroy_timer(temporizador);
	al_destroy_font(fonte);
	al_destroy_sample(musica);

	al_destroy_bitmap(personagem.sprite);
	al_destroy_bitmap(personagem.spriteExp);
	al_destroy_bitmap(personagem.spriteHit);
	al_destroy_bitmap(personagem.spriteMorrendo);
	al_destroy_bitmap(personagem.spriteShoot);
	al_destroy_bitmap(cenario.jogo);
	al_destroy_bitmap(cenario.lane);
	al_destroy_bitmap(cenario.fim);
	al_destroy_bitmap(cenario.inicioTop);
	al_destroy_bitmap(cenario.inicioBottom);
	for(int i = 0; i < NUM_OBJ; i++){
		al_destroy_bitmap(objeto[i].sprite);
	}
	for(int i = 0; i < NUM_TIRO; i++){
		al_destroy_bitmap(tiroPersonagem[i].sprite);
	}
	for(int i = 0; i < NUM_INIM; i++){
		al_destroy_bitmap(inimigos[i].sprite);
		al_destroy_bitmap(inimigos[i].spriteAcao);
		al_destroy_bitmap(inimigos[i].spriteAtac);
		al_destroy_bitmap(inimigos[i].spriteMorrendo);
		al_destroy_bitmap(tiroInimigo[i].sprite);
	}

	return EXIT_SUCCESS;
}

void leituraArquivo(struct Personagem *personagem, struct Rank top5[]){
	struct Rank temp[500];
	struct Rank tempOrder;
	int i, max;

	FILE *fp;
	fp = fopen(ARQUIVO, "a");
	if(fp == NULL){
		printf("Não foi possível abrir o arquivo!\n");
	}

	fprintf(fp, "%s\n%d\n", personagem->nome, personagem->pontos);

	fclose(fp);

	fp = fopen(ARQUIVO, "r");

	for(i = 0; !feof(fp); i++){
		fscanf(fp, "%s\n%d\n", temp[i].nome, &temp[i].pontos);
		//printf("Nome: %s\nPontos: %d\n", temp[i].nome, temp[i].pontos);
	}
	max = i;
	fclose(fp);

	for(int i = 0; i < max; i++){
		for(int j = i+1; j < max; j++){
			if(temp[i].pontos > temp[j].pontos){
				strcpy(tempOrder.nome, temp[i].nome);
				tempOrder.pontos = temp[i].pontos;

				strcpy(temp[i].nome, temp[j].nome);
				temp[i].pontos = temp[j].pontos;

				strcpy(temp[j].nome, tempOrder.nome);
				temp[j].pontos = tempOrder.pontos;
			}
		}
	}
	int j = 0;
	for(int i = max - 1; i >= max - 5; i--){
		//printf("Nome: %s\tPontos: %d\n", temp[i].nome, temp[i].pontos);
		strcpy(top5[j].nome, temp[i].nome);
		top5[j].pontos = temp[i].pontos;
		j++;
	}
}

void animacao(unsigned char *frameCount, struct Personagem *personagem, struct Objetos objeto[], struct Inimigos inimigos[], struct Tiro tiroInimigo[], struct Tiro tiroPersonagem[]){
	if(++*frameCount > 8){
		if(personagem->morrendo == true){
			if(++personagem->frame >= personagem->maxFrameMorrendo){
				personagem->frame = 0;
				personagem->ativo = false;
			}
		}else if(personagem->exp == true){
			if(++personagem->frame >= personagem->maxFrameExp){
				personagem->frame = 0;
				personagem->exp = false;
			}

		}else if(personagem->hit == true){
			if(++personagem->frame >= personagem->maxFrameHit){
				personagem->frame = 0;
				personagem->hit = false;
			}
		}else if(personagem->shoot == true){
			if(++personagem->frame >= personagem->maxFrameShoot){
				personagem->frame = 0;
				personagem->shoot = false;
			}
		}else if(++personagem->frame >= personagem->maxFrame)
			personagem->frame = 0;

		for(int i = 0; i < NUM_OBJ; i++){
			if(++objeto[i].frame >= objeto[i].maxFrame){
				objeto[i].frame = 0;
				if(++objeto[i].frameY >= objeto[i].rollFrame)
					objeto[i].frameY = 0;
			}
		}

		for(int i = 0; i < NUM_INIM; i++){
			if(inimigos[i].morrendo == true){
				if(++inimigos[i].frame >= inimigos[i].maxFrameMorrendo){
					inimigos[i].frame = 0;
					inimigos[i].morrendo = false;
					inimigos[i].ativo = false;
				}
			}else if(inimigos[i].tiro == true){
				if(++inimigos[i].frame >= inimigos[i].maxFrameAcao){
					inimigos[i].frame = 0;
					inimigos[i].tiro = false;
					tiroInimigo[i].ativo = true;
				}
			}else if(inimigos[i].atacou == true){
				if(++inimigos[i].frame >= inimigos[i].maxFrameAtac){
					inimigos[i].frame = 0;
					inimigos[i].atacou = false;
				}
			}else{
				if(++inimigos[i].frame >= inimigos[i].maxFrame)
					inimigos[i].frame = 0;
			}

		}

		for(int i = 0; i < NUM_INIM; i++){
			if(tiroInimigo[i].ativo == true){
				if(++tiroInimigo[i].frame >= tiroInimigo[i].maxFrame)
					tiroInimigo[i].frame = 0;
			}
		}

		for(int i = 0; i < NUM_TIRO; i++){
			if(tiroPersonagem[i].ativo == true){
				if(++tiroPersonagem[i].frame >= tiroPersonagem[i].maxFrame)
					tiroPersonagem[i].frame = 0;
			}
		}

		*frameCount = 0;
	}
}

bool initEstruturas(struct Teclado *teclado, struct Personagem *personagem, struct Objetos objeto[], struct Inimigos inimigos[], struct Tiro tiroInimigo[], struct Tiro tiroPersonagem[], struct Cenario *cenario){

		cenario->lane = al_load_bitmap(BITMAP_LANE);
		if(cenario->lane == NULL){
			printf("Não foi possível abrir o bitmap: %s\n", BITMAP_LANE);
			return EXIT_FAILURE;
		}
		cenario->laneWidth = 320;
		cenario->laneHeight = LANE_HEIGHT;

		cenario->jogo = al_load_bitmap(BITMAP_MARS);
		if(cenario->jogo == NULL){
			printf("Não foi possível abrir o bitmap: %s\n", BITMAP_MARS);
			return EXIT_FAILURE;
		}
		cenario->jogoWidth = 320;
		cenario->jogoHeight = 200;

		cenario->inicioTop = al_load_bitmap(BITMAP_INICIO_TOP);
		if(cenario->inicioTop == NULL){
			printf("Não foi possível abrir o bitmap: %s\n", BITMAP_INICIO_TOP);
			return EXIT_FAILURE;
		}
		cenario->inicioWidthTop = 1280;
		cenario->inicioHeightTop = 330;

		cenario->inicioBottom = al_load_bitmap(BITMAP_INICIO_BOTTOM);
		if(cenario->inicioBottom == NULL){
			printf("Não foi possível abrir o bitmap: %s\n", BITMAP_INICIO_BOTTOM);
			return EXIT_FAILURE;
		}
		cenario->inicioWidthBottom = 1280;
		cenario->inicioHeightBottom = 193;

		cenario->fim = al_load_bitmap(BITMAP_FIM);
		if(cenario->fim == NULL){
			printf("Não foi possível abrir o bitmap: %s\n", BITMAP_FIM);
			return EXIT_FAILURE;
		}
		cenario->fimWidth = 1280;
		cenario->fimHeight = 480;

		cenario->vida = al_load_bitmap(BITMAP_CORACAO);
		if(cenario->vida == NULL){
			printf("Não foi possível abrir o bitmap: %s\n", BITMAP_CORACAO);
			return EXIT_FAILURE;
		}
		cenario->vidaWidth = 30;
		cenario->vidaHeight = 30;

		teclado->timer = 0;
		teclado->a = false;
		teclado->baixo = false;
		teclado->cima = false;
		teclado->d = false;
		teclado->direita = false;
		teclado->enter = false;
		teclado->esc = false;
		teclado->espaco = false;
		teclado->s = false;
		teclado->w = false;

		strcpy(personagem->nome, "\0");
		personagem->frame = 0;
		personagem->x = RESOLUCAO_X/2;
		personagem->y = RESOLUCAO_Y/2;
		personagem->lane = 1;
		personagem->ativo = true;
		personagem->pontos = 0;
		personagem->vida = 3;
		personagem->exp = false;
		personagem->hit = false;
		personagem->shoot = false;
		personagem->morrendo = false;

		personagem->sprite = al_load_bitmap(BITMAP_PERSONAGEM);
		if(personagem->sprite == NULL){
			printf("Não foi possível abrir o bitmap: %s\n", BITMAP_PERSONAGEM);
			return EXIT_FAILURE;
		}
		personagem->frameWidth = 53;
		personagem->frameHeight = 47;
		personagem->maxFrame = 12;

		personagem->spriteExp = al_load_bitmap(BITMAP_PERSONAGEM_EXP);
		if(personagem->spriteExp == NULL){
			printf("Não foi possível abrir o bitmap: %s\n", BITMAP_PERSONAGEM_EXP);
			return EXIT_FAILURE;
		}
		personagem->frameWidthExp = 55;
		personagem->frameHeightExp = 52;
		personagem->maxFrameExp = 4;

		personagem->spriteHit = al_load_bitmap(BITMAP_PERSONAGEM_HIT);
		if(personagem->spriteHit == NULL){
			printf("Não foi possível abrir o bitmap: %s\n", BITMAP_PERSONAGEM_HIT);
			return EXIT_FAILURE;
		}
		personagem->frameWidthHit = 45;
		personagem->frameHeightHit = 47;
		personagem->maxFrameHit = 4;

		personagem->spriteShoot = al_load_bitmap(BITMAP_PERSONAGEM_SHOOT);
		if(personagem->spriteShoot == NULL){
			printf("Não foi possível abrir o bitmap: %s\n", BITMAP_PERSONAGEM_SHOOT);
			return EXIT_FAILURE;
		}
		personagem->frameWidthShoot = 75;
		personagem->frameHeightShoot = 47;
		personagem->maxFrameShoot = 10;

		personagem->spriteMorrendo = al_load_bitmap(BITMAP_PERSONAGEM_DYING);
		if(personagem->spriteMorrendo == NULL){
			printf("Não foi possível abrir o bitmap: %s\n", BITMAP_PERSONAGEM_DYING);
			return EXIT_FAILURE;
		}
		personagem->frameWidthMorrendo = 60;
		personagem->frameHeightMorrendo = 47;
		personagem->maxFrameMorrendo = 28;


		for(int i = 0; i < NUM_OBJ; i++){
			objeto[i].sprite = al_load_bitmap(BITMAP_MOEDAS);
			if(objeto[i].sprite == NULL){
				printf("Não foi possível abrir o bitmap: %s\n", BITMAP_MOEDAS);
				return EXIT_FAILURE;
			}
			al_convert_mask_to_alpha(objeto[i].sprite, al_map_rgb(250, 96, 219));

			objeto[i].frameWidth = 31.25;
			objeto[i].frameHeight = 25;
			objeto[i].frame = 0;
			objeto[i].frameY = 0;
			objeto[i].maxFrame = 4;
			objeto[i].rollFrame = 2;
			objeto[i].x = RESOLUCAO_X/2 + i*objeto[i].frameWidth;
			objeto[i].y = RESOLUCAO_Y/2;
			objeto[i].ativo = false;
			objeto[i].velocidade = 0.3;
		}

		for(int i = 0; i < NUM_INIM; i++){
			inimigos[i].sprite = al_load_bitmap(BITMAP_REBEL_WALKING);
			if(inimigos[i].sprite == NULL){
				printf("Não foi possível abrir o bitmap: %s\n", BITMAP_REBEL_WALKING);
				return EXIT_FAILURE;
			}
			inimigos[i].frameWidth = 31;
			inimigos[i].frameHeight = 52;
			inimigos[i].maxFrame = 12;

			inimigos[i].spriteAcao = al_load_bitmap(BITMAP_REBEL_SHOOTING);
			if(inimigos[i].spriteAcao == NULL){
				printf("Não foi possível abrir o bitmap: %s\n", BITMAP_REBEL_SHOOTING);
				return EXIT_FAILURE;
			}

			inimigos[i].frameWidthAcao = 65;
			inimigos[i].frameHeightAcao = 55;
			inimigos[i].maxFrameAcao = 7;

			inimigos[i].spriteAtac = al_load_bitmap(BITMAP_REBEL_ATTACKING);
			if(inimigos[i].spriteAtac == NULL){
				printf("Não foi possível abrir o bitmap: %s\n", BITMAP_REBEL_ATTACKING);
				return EXIT_FAILURE;
			}

			inimigos[i].frameWidthAtac = 50;
			inimigos[i].frameHeightAtac = 52;
			inimigos[i].maxFrameAtac = 7;

			inimigos[i].spriteMorrendo = al_load_bitmap(BITMAP_REBEL_DYING);
			if(inimigos[i].spriteMorrendo == NULL){
				printf("Não foi possível abrir o bitmap: %s\n", BITMAP_REBEL_DYING);
				return EXIT_FAILURE;
			}

			inimigos[i].frameWidthMorrendo = 58;
			inimigos[i].frameHeightMorrendo = 58;
			inimigos[i].maxFrameMorrendo = 16;

			inimigos[i].frame = 0;
			inimigos[i].x = RESOLUCAO_X/2 + i*inimigos[i].frameWidth;
			inimigos[i].y = RESOLUCAO_Y/2;
			inimigos[i].ativo = false;
			inimigos[i].velocidade = 0.3;
			inimigos[i].tiro = false;
			inimigos[i].atacou = false;
			inimigos[i].morrendo = false;
		}

		for(int i = 0; i < NUM_INIM; i++){
			tiroInimigo[i].sprite = al_load_bitmap(BITMAP_TIRO);
			if(tiroInimigo[i].sprite == NULL){
				printf("Não foi possível abrir o bitmap: %s\n", BITMAP_TIRO);
				return EXIT_FAILURE;
			}

			tiroInimigo[i].frameWidth = 37;
			tiroInimigo[i].frameHeight = 20;
			tiroInimigo[i].frame = 0;
			tiroInimigo[i].maxFrame = 6;
			tiroInimigo[i].x = 0;
			tiroInimigo[i].y = 0;
			tiroInimigo[i].ativo = false;
			tiroInimigo[i].velocidade = 3;
		}

		for(int i = 0; i < NUM_TIRO; i++){
			tiroPersonagem[i].sprite = al_load_bitmap(BITMAP_PERSONAGEM_TIRO);
			if(tiroPersonagem[i].sprite == NULL){
				printf("Não foi possível abrir o bitmap: %s\n", BITMAP_PERSONAGEM_TIRO);
				return EXIT_FAILURE;
			}

			tiroPersonagem[i].frameWidth = 25.45;
			tiroPersonagem[i].frameHeight = 47;
			tiroPersonagem[i].frame = 0;
			tiroPersonagem[i].maxFrame = 10;
			tiroPersonagem[i].x = 0;
			tiroPersonagem[i].y = 0;
			tiroPersonagem[i].ativo = false;
			tiroPersonagem[i].velocidade = 3;
		}

		return EXIT_SUCCESS;
}

void desenhaCenario(struct Cenario *cenario, int stateMachine, int vida){
	switch(stateMachine){
	case INICIO:
		for(int i = 0; i*cenario->inicioWidthTop < RESOLUCAO_X; i++){
			al_draw_bitmap(cenario->inicioTop, (cenario->inicioWidthTop * i), 0, 0);
		}
		for(int i = 0; i*cenario->jogoWidth < RESOLUCAO_X; i++){
			al_draw_bitmap(cenario->inicioBottom, (cenario->inicioWidthBottom * i), (RESOLUCAO_Y - cenario->inicioHeightBottom), 0);
		}
		break;
	case JOGO:
		for(int i = 0; i*cenario->jogoWidth < RESOLUCAO_X; i++){
			for(int j = 0; j < 2; j++){
				al_draw_bitmap(cenario->jogo, (cenario->jogoWidth * i), (cenario->jogoHeight * j), 0);
			}
		}
		for(int i = 0; i*cenario->laneWidth < RESOLUCAO_X; i++){
			for(int j = 0; j < 5; j++){
				al_draw_bitmap(cenario->lane, (cenario->laneWidth * i), (RESOLUCAO_Y - cenario->laneHeight/2) - cenario->laneHeight*j, 0);
			}
		}
		for(int i = 0; i < vida; i++){
			al_draw_bitmap(cenario->vida, 10 + (cenario->vidaWidth * i), 20, 0);
		}
		break;
	case FIM:
		for(int i = 0; i*cenario->fimWidth < RESOLUCAO_X; i++){
			al_draw_bitmap(cenario->fim, (cenario->fimWidth * i), 0, 0);
		}
		break;
	}

}

void desenhaInimigos(struct Inimigos inimigos[]){
	for(int i = 0; i < NUM_OBJ; i++){
		//Acresta a diferença de altura do sprite do personagem e inimigo, para ficar no mesmo nível
		if(inimigos[i].morrendo == true)
			al_draw_bitmap_region(inimigos[i].spriteMorrendo, inimigos[i].frame*inimigos[i].frameWidthMorrendo, 0, inimigos[i].frameWidthMorrendo, inimigos[i].frameHeightMorrendo, inimigos[i].x, inimigos[i].y - 5, 0);
		else if(inimigos[i].ativo == true && inimigos[i].tiro == false && inimigos[i].atacou == false)
			al_draw_bitmap_region(inimigos[i].sprite, inimigos[i].frame*inimigos[i].frameWidth, 0, inimigos[i].frameWidth, inimigos[i].frameHeight, inimigos[i].x, inimigos[i].y - 5, 0);
		else if(inimigos[i].ativo == true && inimigos[i].tiro == true && inimigos[i].atacou == false)
			al_draw_bitmap_region(inimigos[i].spriteAcao, inimigos[i].frame*inimigos[i].frameWidthAcao, 0, inimigos[i].frameWidthAcao, inimigos[i].frameHeightAcao, inimigos[i].x, inimigos[i].y - 5, 0);
		else if(inimigos[i].ativo == true && inimigos[i].atacou == true)
			al_draw_bitmap_region(inimigos[i].spriteAtac, inimigos[i].frame*inimigos[i].frameWidthAtac, 0, inimigos[i].frameWidthAtac, inimigos[i].frameHeightAtac, inimigos[i].x, inimigos[i].y - 5, 0);
		}
}

void desenhaPersonagem(struct Personagem *personagem){
	if(personagem->ativo == true){
		if(personagem->morrendo == true)
			al_draw_bitmap_region(personagem->spriteMorrendo, (personagem->frame*personagem->frameWidthMorrendo), 0, personagem->frameWidthMorrendo, personagem->frameHeightMorrendo, personagem->x, personagem->y, 0);
		else if(personagem->exp == true)
			al_draw_bitmap_region(personagem->spriteExp, (personagem->frame*personagem->frameWidthExp), 0, personagem->frameWidthExp, personagem->frameHeightExp, personagem->x, personagem->y, 0);
		else if (personagem->hit == true)
			al_draw_bitmap_region(personagem->spriteHit, (personagem->frame*personagem->frameWidthHit), 0, personagem->frameWidthHit, personagem->frameHeightHit, personagem->x, personagem->y, 0);
		else if(personagem->shoot == true)
			al_draw_bitmap_region(personagem->spriteShoot, (personagem->frame*personagem->frameWidthShoot), 0, personagem->frameWidthShoot, personagem->frameHeightShoot, personagem->x, personagem->y, 0);
		else
			al_draw_bitmap_region(personagem->sprite, (personagem->frame*personagem->frameWidth), 0, personagem->frameWidth, personagem->frameHeight, personagem->x, personagem->y, 0);
	}
}

void desenhaObjeto(struct Objetos objeto[]){
	for(int i = 0; i < NUM_OBJ; i++){
		if(objeto[i].ativo == true)
			al_draw_bitmap_region(objeto[i].sprite, objeto[i].frame*objeto[i].frameWidth, objeto[i].frameY*objeto[i].frameHeight, objeto[i].frameWidth, objeto[i].frameHeight, objeto[i].x, objeto[i].y, 0);
	}
}

void desenhaTiro(struct Tiro tiroInimigo[], struct Tiro tiroPersonagem[]){
	for(int i = 0; i < NUM_INIM; i++){
		if(tiroInimigo[i].ativo == true)
			al_draw_bitmap_region(tiroInimigo[i].sprite, tiroInimigo[i].frame*tiroInimigo[i].frameWidth, 0, tiroInimigo[i].frameWidth, tiroInimigo[i].frameHeight, tiroInimigo[i].x, tiroInimigo[i].y, 0);
	}
	for(int i = 0; i < NUM_TIRO; i++){
		if(tiroPersonagem[i].ativo == true)
		al_draw_bitmap_region(tiroPersonagem[i].sprite, tiroPersonagem[i].frame*tiroPersonagem[i].frameWidth, 0, tiroPersonagem[i].frameWidth, tiroPersonagem[i].frameHeight, tiroPersonagem[i].x, tiroPersonagem[i].y, 0);
	}
}

void moveObjetos(struct Objetos objeto[]){
	for(int i = 0; i < NUM_OBJ; i++){
		if(objeto[i].ativo == false){
			srand(time(NULL) + i);		//Mudar para microsegundos
			char temp = rand() % 5 + 1;
			objeto[i].ativo = true;
			objeto[i].x = RESOLUCAO_X - i*objeto[i].frameWidth;
			objeto[i].y = RESOLUCAO_Y - (LANE_HEIGHT * temp);
		}else{
			for(int i = 0; i < NUM_OBJ; i++){
				if(objeto[i].x <= 0)
					objeto[i].ativo = false;
				else
					objeto[i].x = objeto[i].x - objeto[i].velocidade;
			}
		}
	}
}

void moveInimigos(struct Inimigos inimigos[], struct Tiro tiroInimigo[]){
	for(int i = 0; i < NUM_OBJ; i++){
		if(inimigos[i].ativo == false){
			srand(time(NULL) + i*5*i);		//Mudar para microsegundos, não deixar ficar em cima das moedas
			char temp = rand() % 5 + 1;
			inimigos[i].ativo = true;
			inimigos[i].atacou = false;
			inimigos[i].x = RESOLUCAO_X - i*inimigos[i].frameWidth;
			inimigos[i].y = RESOLUCAO_Y - (LANE_HEIGHT * temp);
			if(++temp >= 6)
				temp = 0;
		}else{
			for(int i = 0; i < NUM_OBJ; i++){
				if(inimigos[i].x <= 0)
					inimigos[i].ativo = false;
				else if(inimigos[i].atacou == false && inimigos[i].tiro == false && inimigos[i].morrendo == false)
					inimigos[i].x = inimigos[i].x - inimigos[i].velocidade;
			}
		}

		//Gera tiros
		unsigned long int temp;
		for(int i = 0; i < NUM_INIM; i++){
			srand(time(NULL) + i*255);
			temp = rand();
			if((temp % 15) == 0){
				temp++;
				if(tiroInimigo[i].ativo == false && inimigos[i].x > RESOLUCAO_X/2 && inimigos[i].tiro == false && inimigos[i].morrendo == false){
					inimigos[i].tiro = true;
					inimigos[i].frame = 0;
					tiroInimigo[i].x = inimigos[i].x - inimigos[i].frameWidthAcao/2;
					tiroInimigo[i].y = inimigos[i].y;
				}
			}
		}
	}
}

void moveTiro(struct Tiro tiroInimigo[], struct Tiro tiroPersonagem[]){
	for(int i = 0; i < NUM_INIM; i++){
		if(tiroInimigo[i].ativo == true){
			if(tiroInimigo[i].x >= 0)
				tiroInimigo[i].x -= tiroInimigo[i].velocidade;
			else
				tiroInimigo[i].ativo = false;
		}
	}
	for(int i = 0; i < NUM_TIRO; i++){
		if(tiroPersonagem[i].ativo == true){
			if(tiroInimigo[i].x < RESOLUCAO_X)
				tiroPersonagem[i].x += tiroPersonagem[i].velocidade;
		}
		if(tiroPersonagem[i].x >= RESOLUCAO_X)
			tiroPersonagem[i].ativo = false;
	}
}

void movePersonagem(struct Teclado *teclado, struct Personagem *personagem, struct Tiro tiroPersonagem[]){
	char velocidade = 5;
	char delayTeclado = 5;

	if(personagem->morrendo == false){
		if(teclado->cima || teclado->w){
			teclado->timer++;
			if(teclado->timer > delayTeclado){
				teclado->timer = 0;
				if(personagem->lane <= 4){
					personagem->lane++;
				}

			}
		}
		if(teclado->s || teclado->baixo){
			teclado->timer++;
			if(teclado->timer > delayTeclado){
				teclado->timer = 0;
				if(personagem->lane > 1)
					personagem->lane--;
			}
		}
		if(teclado->a || teclado->esquerda)
			if(personagem->x >= 0)
				personagem->x -= velocidade;
		if(teclado->d || teclado->direita)
			if(personagem->x <= RESOLUCAO_X - RESOLUCAO_X/4)
				personagem->x += velocidade;

		personagem->y = RESOLUCAO_Y - (LANE_HEIGHT * personagem->lane);

		if(teclado->espaco == true){
			for(int i = 0; i < NUM_TIRO; i++){
				if(tiroPersonagem[i].ativo == false && personagem->shoot == false){
					personagem->shoot = true;
					tiroPersonagem[i].ativo = true;
					tiroPersonagem[i].x = personagem->x + personagem->frameWidth;
					tiroPersonagem[i].y = personagem->y;
					break;
				}
			}
		}
	}

}

void checaColisao(struct Personagem *personagem, struct Objetos objeto[], struct Inimigos inimigos[], struct Tiro tiroInimigo[], struct Tiro tiroPersonagem[]){
	for(int i = 0; i < NUM_OBJ; i++){
		if(((personagem->x > objeto[i].x - personagem->frameWidth/2) && (personagem->x < objeto[i].x + personagem->frameWidth/2)) && (personagem->y == objeto[i].y)){
			objeto[i].ativo = false;
			personagem->pontos++;
		}
	}
	for(int i = 0; i < NUM_INIM; i++){
		if(((personagem->x > tiroInimigo[i].x - personagem->frameWidth/2) && (personagem->x < tiroInimigo[i].x + personagem->frameWidth/2)) && (personagem->y == tiroInimigo[i].y) && tiroInimigo[i].ativo == true && inimigos[i].morrendo == false){
			tiroInimigo[i].ativo = false;
			inimigos[i].frame = 0;
			personagem->vida--;
			personagem->exp = true;
			personagem->frame = 0;
			if(personagem->vida <= 0)
				personagem->morrendo = true;
		}
		if(((personagem->x > inimigos[i].x - personagem->frameWidth/2) && (personagem->x < inimigos[i].x + personagem->frameWidth/2)) && (personagem->y == inimigos[i].y) && inimigos[i].atacou == false && inimigos[i].morrendo == false){
			inimigos[i].atacou = true;
			inimigos[i].frame = 0;
			personagem->vida--;
			personagem->frame = 0;
			personagem->hit = true;
			if(personagem->vida <= 0)
				personagem->morrendo = true;
		}
	}

	for(int i = 0; i < NUM_TIRO; i++){
		for(int j = 0; j < NUM_INIM; j++){
			if((tiroPersonagem[i].x >= inimigos[j].x - inimigos[j].frameWidth/2) && (tiroPersonagem[i].x <= inimigos[j].x + inimigos[j].frameWidth/2) && (tiroPersonagem[i].y == inimigos[j].y) && inimigos[j].morrendo == false && tiroPersonagem[i].ativo == true){
				tiroPersonagem[i].ativo = false;
				tiroPersonagem[i].y = -RESOLUCAO_Y;
				inimigos[j].morrendo = true;
				inimigos[j].frame = 0;
				personagem->pontos += 5;
			}
		}
	}
}

bool readString(ALLEGRO_EVENT *ev, char size, char *string){
	/*
	 * Lê uma string, retorna 1 quando a tecla ENTER fo pressionada
	 */
	if (ev->type == ALLEGRO_EVENT_KEY_CHAR){
		if(strlen(string) <= size){
			char temp[] = {ev->keyboard.unichar, '\0'};
			if(ev->keyboard.unichar == ' '){
				strcat(string, temp);
			}
			else if(ev->keyboard.unichar >= '0' && ev->keyboard.unichar <= '9'){
				strcat(string, temp);
			}
			else if(ev->keyboard.unichar >= 'A' && ev->keyboard.unichar <= 'Z'){
				strcat(string, temp);
			}
			else if(ev->keyboard.unichar >= 'a' && ev->keyboard.unichar <= 'z'){
				strcat(string, temp);
			}
		}
		if(ev->keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(string) != 0){
			string[strlen(string) - 1] = '\0';
		}
		if(ev->keyboard.keycode == ALLEGRO_KEY_ENTER && strlen(string) != 0){
			return true;
		}
	}
	return false;
}

void checaTeclado(ALLEGRO_EVENT *ev, struct Teclado *teclado){
	/*
	 * Checa as teclas pressionadas
	 */

	if(ev->type == ALLEGRO_EVENT_KEY_DOWN){
		switch(ev->keyboard.keycode){
		case ALLEGRO_KEY_W:
			teclado->w = true;
			break;
		case ALLEGRO_KEY_S:
			teclado->s = true;
			break;
		case ALLEGRO_KEY_A:
			teclado->a = true;
			break;
		case ALLEGRO_KEY_D:
			teclado->d = true;
			break;
		case ALLEGRO_KEY_SPACE:
			teclado->espaco = true;
			break;
		case ALLEGRO_KEY_ESCAPE:
			teclado->esc = true;
			break;
		case ALLEGRO_KEY_ENTER:
			teclado->enter = true;
			break;
		case ALLEGRO_KEY_PAD_ENTER:
			teclado->enter = true;
			break;
		case ALLEGRO_KEY_UP:
			teclado->cima = true;
			break;
		case ALLEGRO_KEY_DOWN:
			teclado->baixo = true;
			break;
		case ALLEGRO_KEY_LEFT:
			teclado->esquerda = true;
			break;
		case ALLEGRO_KEY_RIGHT:
			teclado->direita = true;
			break;
		}
	}
	if(ev->type == ALLEGRO_EVENT_KEY_UP){
		switch(ev->keyboard.keycode){
		case ALLEGRO_KEY_W:
			teclado->w = false;
			break;
		case ALLEGRO_KEY_S:
			teclado->s = false;
			break;
		case ALLEGRO_KEY_A:
			teclado->a = false;
			break;
		case ALLEGRO_KEY_D:
			teclado->d = false;
			break;
		case ALLEGRO_KEY_SPACE:
			teclado->espaco = false;
			break;
		case ALLEGRO_KEY_ESCAPE:
			teclado->esc = false;
			break;
		case ALLEGRO_KEY_ENTER:
			teclado->enter = false;
			break;
		case ALLEGRO_KEY_PAD_ENTER:
			teclado->enter = false;
			break;
		case ALLEGRO_KEY_UP:
			teclado->cima = false;
			break;
		case ALLEGRO_KEY_DOWN:
			teclado->baixo = false;
			break;
		case ALLEGRO_KEY_LEFT:
			teclado->esquerda = false;
			break;
		case ALLEGRO_KEY_RIGHT:
			teclado->direita = false;
			break;
		}
	}
}
