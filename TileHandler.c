#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>

#include "BoulderEssencials.h"

#include "TileHandler.h"
#include "EntityHandler.h"

//--------------------------------------------------------------------

// funcao que checa se um tile esta cercado de tiles considerados "inescapaveis"
int ChecaTilePresso(mapa* jogo_info, int x, int y) {
	switch(jogo_info->tiles[x+1][y].tipo) {
		case BOULDER_TYPE_AIR :
			return 0;
		break;

		case BOULDER_TYPE_DIRT :
			return 0;
		break;

		case BOULDER_TYPE_CRISTAL :
			return 0;
		break;
	}

	switch(jogo_info->tiles[x-1][y].tipo) {
		case BOULDER_TYPE_AIR :
			return 0;
		break;

		case BOULDER_TYPE_DIRT :
			return 0;
		break;

		case BOULDER_TYPE_CRISTAL :
			return 0;
		break;
	}

	switch(jogo_info->tiles[x][y+1].tipo) {
		case BOULDER_TYPE_AIR :
			return 0;
		break;

		case BOULDER_TYPE_DIRT :
			return 0;
		break;

		case BOULDER_TYPE_CRISTAL :
			return 0;
		break;
	}

	switch(jogo_info->tiles[x][y-1].tipo) {
		case BOULDER_TYPE_AIR :
			return 0;
		break;

		case BOULDER_TYPE_DIRT :
			return 0;
		break;

		case BOULDER_TYPE_CRISTAL :
			return 0;
		break;
	}

	return 1;
}

// retorna se o tile eh vazio e dentro das dimensoes do mapa
int ChecaTileVazio(mapa* jogo_info, int x, int y) {
	if (x >= 0 && x < BOULDER_DIMX && y >= 0 && y < BOULDER_DIMY) {
		if (jogo_info->tiles[x][y].tipo != BOULDER_TYPE_AIR) {
			return 0;
		}
	}

	return 1;
}

//--------------------------------------------------------------------

// transforma um tile em outro
void TransformaTile(mapa* jogo_info, int x, int y, int boulder_type, int estado) {
	jogo_info->tiles[x][y].tipo = boulder_type;
	jogo_info->tiles[x][y].estado = estado;
}

// move um tile para o outro, deixando ar na posicao original
void MoverPara(mapa* jogo_info, int x1, int y1, int x2, int y2) {
	jogo_info->tiles[x2][y2].tipo = jogo_info->tiles[x1][y1].tipo;
	jogo_info->tiles[x2][y2].estado = jogo_info->tiles[x1][y1].estado;

	if (x1 > x2 || y2 < y1) {
		jogo_info->tiles[x2][y2].cheque_atualizado = 0;
	} else {
		jogo_info->tiles[x2][y2].cheque_atualizado = 1;
	}

	jogo_info->tiles[x1][y1].tipo = BOULDER_TYPE_AIR;
	jogo_info->tiles[x1][y1].estado = 0;
}

//retorna se os tijolos encantados estão ativados
int TijoloEncantadoAtivavel(mapa* jogo_info) {
	if (jogo_info->EnchantedAtivado)
		return 1;

	if (jogo_info->EnchantedTimer == 0)
		return 1;

	return 0;
}

//--------------------------------------------------------------------

// calcula e processa a fisica de um tile que desliza
void DeslizaTile(mapa* jogo_info, int x, int y) {
	if (ChecaTileVazio(jogo_info, x-1, y) && ChecaTileVazio(jogo_info, x-1, y+1)) {
		//desliza para esquerda
		MoverPara(jogo_info, x, y, x-1, y);
		jogo_info->tiles[x-1][y].estado = 1;
	} else {
		if (ChecaTileVazio(jogo_info, x+1, y) && ChecaTileVazio(jogo_info, x+1, y+1)) {
			//desliza para direita
			MoverPara(jogo_info, x, y, x+1, y);
			jogo_info->tiles[x+1][y].estado = 1;
		} else {
			//objeto inerte
			jogo_info->tiles[x][y].estado = 0;
		}
	}
}

//transforma uma entidade afetada por gravidade pelo tijoo encantado
static void TijoloEncantadoTranformacao(mapa* jogo_info, int x, int y) {
	if (TijoloEncantadoAtivavel(jogo_info)) {
		jogo_info->EnchantedAtivado = 1;

		if (jogo_info->tiles[x][y].estado) {
			if (ChecaTileVazio(jogo_info, x, y+2)) {
				if (jogo_info->tiles[x][y].tipo == BOULDER_TYPE_BOULDER) {
					TransformaTile(jogo_info, x, y+2, BOULDER_TYPE_CRISTAL, 1);
				} else {
					TransformaTile(jogo_info, x, y+2, BOULDER_TYPE_BOULDER, 1);
				}
			}

			TransformaTile(jogo_info, x, y, BOULDER_TYPE_AIR, 0);
		}
	} else {
		jogo_info->tiles[x][y].estado = 0;
	}
}

// calcula a gravidade para um tile
void GravidadeHandler(mapa* jogo_info, strutc_sound* boulder_sound, int x, int y) {
	if (ChecaTileVazio(jogo_info, x, y+1)) {
		MoverPara(jogo_info, x, y, x, y+1);

		jogo_info->tiles[x][y+1].estado = 1;
	} else {

		switch (jogo_info->tiles[x][y+1].tipo) {
			case BOULDER_TYPE_ECHTBRICK :
				TijoloEncantadoTranformacao(jogo_info, x, y);
			break;

			case BOULDER_TYPE_PLAYER :
				if (jogo_info->tiles[x][y].estado)
					ExplodirCentro(jogo_info, boulder_sound, x, y+1, 0);
			break;

			case BOULDER_TYPE_SQUARE :
				if (jogo_info->tiles[x][y].estado)
					ExplodirCentro(jogo_info, boulder_sound, x, y+1, 0);
			break;

			case BOULDER_TYPE_BUTTERFLY :
				if (jogo_info->tiles[x][y].estado)
					ExplodirCentro(jogo_info, boulder_sound, x, y+1, 1);
			break;

			case BOULDER_TYPE_BRICK :
				DeslizaTile(jogo_info, x, y);
			break;

			case BOULDER_TYPE_IRON :
				DeslizaTile(jogo_info, x, y);
			break;

			case BOULDER_TYPE_EXIT :
				DeslizaTile(jogo_info, x, y);
			break;
								
			case BOULDER_TYPE_CRISTAL :
				DeslizaTile(jogo_info, x, y);
			break;

			case BOULDER_TYPE_BOULDER :
				DeslizaTile(jogo_info, x, y);
			break;

			default :
				jogo_info->tiles[x][y].estado = 0;
			break;
		}

	}
}