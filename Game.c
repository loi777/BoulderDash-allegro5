#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>

#include <stdio.h>

#include "BoulderEssencials.h"
#include "Drawer.h"

#include "MapHandler.h"
#include "GameUpdater.h"
#include "KeyboardHandler.h"
#include "PlayerHandler.h"

//------------------------------------------------------------------------------

int main() {

	//----------------  inicia as varias funcoes do allegro  ----------------//

	al_init();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_acodec_addon();
	al_init_primitives_addon();
	al_install_keyboard();
	al_install_audio();

	// variaveis que sao usadas fora do loop
	ALLEGRO_DISPLAY* display = al_create_display(BOULDER_DISPX, BOULDER_DISPY);
	ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();
	ALLEGRO_TIMER* timer = al_create_timer(1.0 / BOULDER_FPS);

	// eventos que serao usados
	al_register_event_source(fila_eventos, al_get_display_event_source(display));
	al_register_event_source(fila_eventos, al_get_keyboard_event_source());

	al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
	al_start_timer(timer);

	// variaveis que serao usadas no loop
	ALLEGRO_EVENT evento_atual;
	int timer_fisica;
	int timer_jogo;

	//----------------  gera minhas fontes de dados  ----------------//

	// obtem meu sprite sheet
	ALLEGRO_BITMAP* pixel_sheet = al_create_bitmap(160, 128);
	pixel_sheet = al_load_bitmap("resources/spritesheet.bmp");

	// obtem a fonte do boulder dash
	struct_font* boulder_font = CarregarFonte();

	// obtem minhas samples de audio
	strutc_sound* boulder_sound = CarregarAudios();
	al_play_sample(boulder_sound->bg_musica, 1.0f, 0.0f, 1.0f, ALLEGRO_PLAYMODE_LOOP, 0);

	// estrutura do meu jogo
	mapa* jogo_info = CriarMapa();
	CarregarMapa(jogo_info, boulder_sound, 1);

	//----------------  game loop principal  ----------------//

	jogo_info->estado_de_jogo = 1;
	while (jogo_info->estado_de_jogo) {

		// pega o proximo evento
		al_wait_for_event(fila_eventos, &evento_atual);

		// checa os varios eventos que teremos
		switch(evento_atual.type) {
			
			case ALLEGRO_EVENT_KEY_DOWN :
				PLayerObterInput(jogo_info, evento_atual);
			break;

			case ALLEGRO_EVENT_KEY_UP :

				ChecarFinalizarJogo(jogo_info, evento_atual);

				ChecarInfoScreen(jogo_info, boulder_sound, evento_atual);

				PlayerLimpaInput(jogo_info, evento_atual);

				ChecarDevSkip(jogo_info, boulder_sound, evento_atual);

			break;

			case ALLEGRO_EVENT_TIMER :

				// atualizador em segundos
				timer_jogo++;
				if (timer_jogo >= BOULDER_FPS) {
					timer_jogo = 0;
					AtualizarRelogio(jogo_info, boulder_sound);
				}

				// atualizador de fisica
				timer_fisica++;
				if (timer_fisica >= BOULDER_FT) {
					timer_fisica = 0;
					AtualizarFisicaMapa(jogo_info, boulder_sound);
				}

				// limpa a tela para o proximo commit
				al_clear_to_color(al_map_rgb(0, 0, 0));

				// desenha todas as coisas
				AtualizarVisualMapa(jogo_info, pixel_sheet);

				// desenha o meu info-texto superior
				AtualizarTexto(jogo_info, pixel_sheet, boulder_font);

				// desenha a tela de informacao
				if (jogo_info->InfoScreen) {
					DesenhaInfoScreen(boulder_font);
				}

				// da commit nas mudancas visuais
				al_flip_display();

			break;
		}
	}

	//---------------  loop da tela final do jogo  ----------------//

	// desenha a tela de pontuacao final
	DesenhaTelaFinal(jogo_info, boulder_font);

	while(1) {
		// pega o proximo evento
		al_wait_for_event(fila_eventos, &evento_atual);

		if (evento_atual.type == ALLEGRO_EVENT_KEY_UP) {
			if (evento_atual.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
				break;
			}
		}
	}

	//----------------  limpa a memoria allocada  ----------------//

	DestroiMapa(jogo_info);
	DestroiFonte(boulder_font);
	DestroiAudios(boulder_sound);

	al_destroy_bitmap(pixel_sheet);
	al_destroy_display(display);
	al_destroy_event_queue(fila_eventos);

	al_uninstall_keyboard();
	al_uninstall_audio();

	return 0;
}