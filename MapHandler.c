#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>

#include <stdlib.h>
#include <stdio.h>

#include "BoulderEssencials.h"

#include "MapHandler.h"

//-----------------------------------------------------------------

// cria a estrutura de um mapa e jogo
mapa* CriarMapa() {
	mapa* jogo_info = malloc(sizeof(mapa));

	jogo_info->tiles = malloc(BOULDER_DIMX * sizeof(tile*));
	for (int i = 0; i < BOULDER_DIMX; i++) {
		jogo_info->tiles[i] = malloc(BOULDER_DIMY * sizeof(tile));
	}

	jogo_info->pontos = 0;

	return jogo_info;
}

// limpa a memoria allocada para o mapa e jogo
int DestroiMapa(mapa* jogo_info) {
	for (int i = 0; i < BOULDER_DIMX; i++) {
		free(jogo_info->tiles[i]);
	}
	free(jogo_info->tiles);

	free(jogo_info);

	return 0;
}

//-----------------------------------------------------------------

// carrega o mapa indicado pelo level
void CarregarMapa(mapa* jogo_info, strutc_sound* boulder_sound, int level) {
	FILE* ArquivoMapa = fopen("resources/mapas.txt", "r");
	
	// testa se o nivel pedido eh valido
	int nivel_calculado = level;

	if (nivel_calculado < 1) nivel_calculado = 1;
	if (nivel_calculado > BOULDER_MAXLEVEL) {
		al_play_sample(boulder_sound->win, 1.0f, 0.0f, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);

		jogo_info->estado_de_jogo = 0;

		return;
	}

	jogo_info->level = nivel_calculado;

	//--  pula linha ate chegar no level desejado --//
	char buffer[BUFSIZ];
	for (int i = 1; i < nivel_calculado; i++) {
		for (int j = 0; j < (BOULDER_DIMY + 1); j++) {
			fgets(buffer, BUFSIZ, ArquivoMapa);
		}
	}
	//--  -----------------------------------  --//

	//--         reseta valores do jogo        --//
	jogo_info->CristaisColetados = 0;

	jogo_info->PlayerInputQueue[0] = 4;
	jogo_info->PlayerMortoTimer = 0;
	jogo_info->PlayerTardisEasterEgg = 0;

	jogo_info->AmoebaEstado = 0;

	jogo_info->EnchantedTimer = 0;
	jogo_info->EnchantedAtivado = 0;

	jogo_info->TempoAtual = 0;
	jogo_info->InfoScreen = 0;

	jogo_info->SpriteTimer = 0;
	//--  -----------------------------------  --//

	//--  le as informacoes do level desejado  --//
	fscanf(ArquivoMapa, "%d", &jogo_info->CristaisFaltando);

	fscanf(ArquivoMapa, "%d", &jogo_info->CristaisPontos);
	fscanf(ArquivoMapa, "%d", &jogo_info->CristaisPtsExtra);

	fscanf(ArquivoMapa, "%d", &jogo_info->TempoMaximo);

	srand((jogo_info->TempoMaximo + jogo_info->CristaisPontos) * jogo_info->level);
	//--  ----------------------------------  --//

	//--       carrega os tiles do mapa       --//
	for (int j = 0; j < BOULDER_DIMY; j++) {
		for (int i = 0; i < BOULDER_DIMX; i++) {
			fscanf(ArquivoMapa, "%d", &jogo_info->tiles[i][j].tipo);
			
			jogo_info->tiles[i][j].estado = 0;
			jogo_info->tiles[i][j].cheque_atualizado = 0;
		}
	}
	//--  ----------------------------------  --//
	fclose(ArquivoMapa);
}