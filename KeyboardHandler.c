#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>

#include "BoulderEssencials.h"

#include "MapHandler.h"
#include "KeyboardHandler.h"

// funcao que analisa o input e muda de mapa
void ChecarDevSkip(mapa* jogo_info, strutc_sound* boulder_sound, ALLEGRO_EVENT evento_atual) {
	switch(evento_atual.keyboard.keycode) {
		case(ALLEGRO_KEY_PGUP) :
			CarregarMapa(jogo_info, boulder_sound, (jogo_info->level - 1));
		break;

		case(ALLEGRO_KEY_PGDN) :
			CarregarMapa(jogo_info, boulder_sound, (jogo_info->level + 1));
		break;
	}
}

// funcao que analisa o input e sai do jogo
void ChecarFinalizarJogo(mapa* jogo_info, ALLEGRO_EVENT evento_atual) {
	if (evento_atual.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
		jogo_info->estado_de_jogo = 0;
	}
}

// funcao que analisa o input e mostra a tela de informacao
void ChecarInfoScreen(mapa* jogo_info, strutc_sound* boulder_sound, ALLEGRO_EVENT evento_atual) {
	switch(evento_atual.keyboard.keycode) {
		case(ALLEGRO_KEY_H) :
			al_play_sample(boulder_sound->menu, 1.0f, 0.0f, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);
			jogo_info->InfoScreen = !jogo_info->InfoScreen;
		break;

		case(ALLEGRO_KEY_F1) :
			al_play_sample(boulder_sound->menu, 1.0f, 0.0f, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);
			jogo_info->InfoScreen = !jogo_info->InfoScreen;
		break;
	}
}