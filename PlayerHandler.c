#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>

#include <stdlib.h>

#include "BoulderEssencials.h"

#include "MapHandler.h"
#include "TileHandler.h"
#include "EntityHandler.h"
#include "PlayerHandler.h"

//----------------------------------------------------------

// funcao que checa se o player esta presso para o easter egg
int EasterEggCheck(mapa* jogo_info, strutc_sound* boulder_sound, int x, int y) {
	if (ChecaTilePresso(jogo_info, x, y)) {
		jogo_info->PlayerTardisEasterEgg++;

		if (jogo_info->PlayerTardisEasterEgg >= 25) {
			TransformaTile(jogo_info, x, y, BOULDER_TYPE_TARDIS, 0);
			al_play_sample(boulder_sound->tardis, 1.0f, 0.0f, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);

			return 1;
		}
	}

	return 0;
}

//----------------------------------------------------------

// insere um elemento na queue de inputs do player
static void InsereElementoQueue(int* queue, int val) {
	queue[2] = queue[1];
	queue[1] = queue[0];

	queue[0] = val;
}

// retorna qual foi o player-input da tecla pressionada
void PLayerObterInput(mapa* jogo_info, ALLEGRO_EVENT evento_atual) {
    switch(evento_atual.keyboard.keycode) {
        case(ALLEGRO_KEY_W) :
			InsereElementoQueue(jogo_info->PlayerInputQueue, 0);
        break;

		case(ALLEGRO_KEY_UP) :
			InsereElementoQueue(jogo_info->PlayerInputQueue, 0);
        break;

		//--

        case(ALLEGRO_KEY_A) :
            InsereElementoQueue(jogo_info->PlayerInputQueue, 1);
        break;

		case(ALLEGRO_KEY_LEFT) :
			InsereElementoQueue(jogo_info->PlayerInputQueue, 1);
        break;

		//--

        case(ALLEGRO_KEY_S) :
            InsereElementoQueue(jogo_info->PlayerInputQueue, 2);
        break;

		case(ALLEGRO_KEY_DOWN) :
			InsereElementoQueue(jogo_info->PlayerInputQueue, 2);
        break;

		//--

        case(ALLEGRO_KEY_D) :
           	InsereElementoQueue(jogo_info->PlayerInputQueue, 3);
        break;

		case(ALLEGRO_KEY_RIGHT) :
			InsereElementoQueue(jogo_info->PlayerInputQueue, 3);
        break;
    }
}

// insere um elemento na queue de inputs do player
static void RemoveElementoQueue(int* queue, int val) {
	if (queue[0] == val) {
		queue[0] = queue[1];
		queue[1] = queue[2];
		queue[2] = 4;
	}

	if (queue[1] == val) {
		queue[1] = queue[2];
		queue[2] = 4;
	}

	if (queue[2] == val) {
		queue[2] = 4;
	}
}

// corretamente limpa o player-input da tecla solta
void PlayerLimpaInput(mapa* jogo_info, ALLEGRO_EVENT evento_atual) {
    switch(evento_atual.keyboard.keycode) {
        case(ALLEGRO_KEY_W) :
			RemoveElementoQueue(jogo_info->PlayerInputQueue, 0);
        break;

		case(ALLEGRO_KEY_UP) :
			RemoveElementoQueue(jogo_info->PlayerInputQueue, 0);
        break;

		//--

        case(ALLEGRO_KEY_A) :
            RemoveElementoQueue(jogo_info->PlayerInputQueue, 1);
        break;

		case(ALLEGRO_KEY_LEFT) :
			RemoveElementoQueue(jogo_info->PlayerInputQueue, 1);
        break;

		//--

        case(ALLEGRO_KEY_S) :
            RemoveElementoQueue(jogo_info->PlayerInputQueue, 2);
        break;

		case(ALLEGRO_KEY_DOWN) :
			RemoveElementoQueue(jogo_info->PlayerInputQueue, 2);
        break;

		//--

        case(ALLEGRO_KEY_D) :
           	RemoveElementoQueue(jogo_info->PlayerInputQueue, 3);
        break;

		case(ALLEGRO_KEY_RIGHT) :
			RemoveElementoQueue(jogo_info->PlayerInputQueue, 3);
        break;
    }
}

//----------------------------------------------------------

// funcao que adiciona um cristal ao placar
static void ColetarCristal(mapa* jogo_info) {
	jogo_info->CristaisColetados++;

	if (jogo_info->CristaisColetados < jogo_info->CristaisFaltando) {
		jogo_info->pontos += jogo_info->CristaisPontos;
	} else {
		jogo_info->pontos += jogo_info->CristaisPtsExtra;
	}
}

// processa as interacoes do player com um tile que ele esta se movendo para
void PlayerInteracao(mapa* jogo_info, strutc_sound* boulder_sound, int x1, int y1, int x2, int y2) {
	switch(jogo_info->tiles[x2][y2].tipo) {
		case BOULDER_TYPE_EXIT :
			if (jogo_info->CristaisColetados >= jogo_info->CristaisFaltando)
				CarregarMapa(jogo_info, boulder_sound, jogo_info->level + 1);
		break;
		
		case BOULDER_TYPE_DIRT :
			MoverPara(jogo_info, x1, y1, x2, y2);
		break;

		case BOULDER_TYPE_CRISTAL :
			if (jogo_info->tiles[x2][y2].estado == 0) {
				MoverPara(jogo_info, x1, y1, x2, y2);

				al_play_sample(boulder_sound->collect, 1.0f, 0.0f, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);
				ColetarCristal(jogo_info);
			}
		break;

		case BOULDER_TYPE_BOULDER :
			if (y1 == y2 && jogo_info->tiles[x2][y2].estado == 0) {
				if (x1 < x2) {
					if (ChecaTileVazio(jogo_info, x2+1, y2)) {
						if (rand() % 10 > 5) {
							MoverPara(jogo_info, x2, y2, x2+1, y2);
							MoverPara(jogo_info, x1, y1, x2, y2);
						}
					}
				} else {
					if (ChecaTileVazio(jogo_info, x2-1, y2)) {
						if (rand() % 10 > 5) {
							MoverPara(jogo_info, x2, y2, x2-1, y2);
							MoverPara(jogo_info, x1, y1, x2, y2);
						}
					}
				}
			}
		break;
	}
}

void PlayerStandByAnimation(mapa* jogo_info, int x, int y) {
	if ((rand() % 10) > 8) {
		if (jogo_info->tiles[x][y].estado == 0) {
			jogo_info->tiles[x][y].estado = 1;
		} else {
			jogo_info->tiles[x][y].estado = 0;
		}
	}
}

// funcao que trata a funcionalidade do player
void PlayerHandler(mapa* jogo_info, strutc_sound* boulder_sound, int x, int y) {
	if (jogo_info->TempoAtual == jogo_info->TempoMaximo) {
		ExplodirCentro(jogo_info, boulder_sound, x, y, 0);
	} else {
		switch(jogo_info->PlayerInputQueue[0]) {

			case PLAYER_MOVE_UP :
				if (ChecaTileVazio(jogo_info, x, y-1)) {
					MoverPara(jogo_info, x, y, x, y-1);
				} else {
					PlayerInteracao(jogo_info, boulder_sound, x, y, x, y-1);
				}
			break;

			case PLAYER_MOVE_LEFT :
				if (ChecaTileVazio(jogo_info, x-1, y)) {
					MoverPara(jogo_info, x, y, x-1, y);
				} else {
					PlayerInteracao(jogo_info, boulder_sound, x, y, x-1, y);
				}
			break;

			case PLAYER_MOVE_DOWN :
				if (ChecaTileVazio(jogo_info, x, y+1)) {
					MoverPara(jogo_info, x, y, x, y+1);
				} else {
					PlayerInteracao(jogo_info, boulder_sound, x, y, x, y+1);
				}
			break;
			
			case PLAYER_MOVE_RIGHT :
				if (ChecaTileVazio(jogo_info, x+1, y)) {
					MoverPara(jogo_info, x, y, x+1, y);
				} else {
					PlayerInteracao(jogo_info, boulder_sound, x, y, x+1, y);
				}
			break;

		}
	}
}