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

// processa uma explosao 3x3 com centro em x e y
void ExplodirCentro(mapa* jogo_info, strutc_sound* boulder_sound, int x, int y, int tipo) {
	al_play_sample(boulder_sound->explosao, 1.0f, 0.0f, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);

	//explode o meio antes de tudo, como no original, replicando um bug existente
	//assim como impede conflitos diversos posteriores
	if (jogo_info->tiles[x][y].tipo == BOULDER_TYPE_BUTTERFLY) {
		TransformaTile(jogo_info, x, y, BOULDER_TYPE_CEXPLOSION, 0);
	} else {
		TransformaTile(jogo_info, x, y, BOULDER_TYPE_EXPLOSION, 0);
	}

	for (int i = x-1; i <= x+1; i++) {
		for (int j = y-1; j <= y+1; j++) {

			//checa casos especiais da explosao
			switch (jogo_info->tiles[i][j].tipo) {
				case BOULDER_TYPE_IRON :
					continue;
				break;
				case BOULDER_TYPE_ENTRANCE :
					continue;
				break;
				case BOULDER_TYPE_EXIT :
					continue;
				break;
				case BOULDER_TYPE_CEXPLOSION :
					continue;
				break;

				case BOULDER_TYPE_PLAYER :
					ExplodirCentro(jogo_info, boulder_sound, i, j, 0);
				break;

				case BOULDER_TYPE_SQUARE :
					ExplodirCentro(jogo_info, boulder_sound, i, j, 0);
				break;

				case BOULDER_TYPE_BUTTERFLY :
					ExplodirCentro(jogo_info, boulder_sound, i, j, 1);
				break;
			}

			// transforma o tile em explosao
			// marca criacao de cristais se a explosao eh de uma borboleta
			switch (tipo) {
				case 0 :
					TransformaTile(jogo_info, i, j, BOULDER_TYPE_EXPLOSION, 0);
				break;

				case 1 :
					TransformaTile(jogo_info, i, j, BOULDER_TYPE_CEXPLOSION, 0);
				break;

				case 2 :
					if ((i > x && j >= y) || (i == x && j > y)) {
						TransformaTile(jogo_info, i, j, BOULDER_TYPE_EXPLOSION, 0);
					} else {
						TransformaTile(jogo_info, i, j, BOULDER_TYPE_CEXPLOSION, 0);
					}
				break;

				case 3 :
					if (i < x && j <= y) {
						TransformaTile(jogo_info, i, j, BOULDER_TYPE_EXPLOSION, 0);
					} else {
						TransformaTile(jogo_info, i, j, BOULDER_TYPE_CEXPLOSION, 0);
					}
				break;

				case 4 :
					if ((i < x && j >= y) || (i == x && j > y)) {
						TransformaTile(jogo_info, i, j, BOULDER_TYPE_EXPLOSION, 0);
					} else {
						TransformaTile(jogo_info, i, j, BOULDER_TYPE_CEXPLOSION, 0);
					}
				break;

				case 5 :
					if (j > y || (i == x && j == y)) {
						TransformaTile(jogo_info, i, j, BOULDER_TYPE_CEXPLOSION, 0);
					} else {
						TransformaTile(jogo_info, i, j, BOULDER_TYPE_EXPLOSION, 0);
					}
				break;
			}
		}
	}
}

// verifica para borboletas e quadrados se eles devem explodir
static int VerificarCondicaoExplosao(mapa* jogo_info, strutc_sound* boulder_sound, int x, int y) {
	switch (jogo_info->tiles[x+1][y].tipo) {
		case BOULDER_TYPE_PLAYER :
			if (jogo_info->tiles[x][y].tipo == BOULDER_TYPE_BUTTERFLY) {
				ExplodirCentro(jogo_info, boulder_sound, x, y, 2);
			} else {
				ExplodirCentro(jogo_info, boulder_sound, x, y, 0);
			}

			return 1;
		break;

		case BOULDER_TYPE_AMOEBA :
			if (jogo_info->tiles[x][y].tipo == BOULDER_TYPE_BUTTERFLY) {
				ExplodirCentro(jogo_info, boulder_sound, x, y, 1);
			} else {
				ExplodirCentro(jogo_info, boulder_sound, x, y, 0);
			}

			return 1;
		break;
	}

	switch (jogo_info->tiles[x-1][y].tipo) {
		case BOULDER_TYPE_PLAYER :
			if (jogo_info->tiles[x][y].tipo == BOULDER_TYPE_BUTTERFLY) {
				ExplodirCentro(jogo_info, boulder_sound, x, y, 3);
			} else {
				ExplodirCentro(jogo_info, boulder_sound, x, y, 0);
			}

			return 1;
		break;

		case BOULDER_TYPE_AMOEBA :
			if (jogo_info->tiles[x][y].tipo == BOULDER_TYPE_BUTTERFLY) {
				ExplodirCentro(jogo_info, boulder_sound, x, y, 1);
			} else {
				ExplodirCentro(jogo_info, boulder_sound, x, y, 0);
			}

			return 1;
		break;
	}

	switch (jogo_info->tiles[x][y+1].tipo) {
		case BOULDER_TYPE_PLAYER :
			if (jogo_info->tiles[x][y].tipo == BOULDER_TYPE_BUTTERFLY) {
				ExplodirCentro(jogo_info, boulder_sound, x, y, 4);
			} else {
				ExplodirCentro(jogo_info, boulder_sound, x, y, 0);
			}

			return 1;
		break;

		case BOULDER_TYPE_AMOEBA :
			if (jogo_info->tiles[x][y].tipo == BOULDER_TYPE_BUTTERFLY) {
				ExplodirCentro(jogo_info, boulder_sound, x, y, 1);
			} else {
				ExplodirCentro(jogo_info, boulder_sound, x, y, 0);
			}

			return 1;
		break;
	}

	switch (jogo_info->tiles[x][y-1].tipo) {
		case BOULDER_TYPE_PLAYER :
			if (jogo_info->tiles[x][y].tipo == BOULDER_TYPE_BUTTERFLY) {
				ExplodirCentro(jogo_info, boulder_sound, x, y, 5);
			} else {
				ExplodirCentro(jogo_info, boulder_sound, x, y, 0);
			}
			
			return 1;
		break;

		case BOULDER_TYPE_AMOEBA :
			if (jogo_info->tiles[x][y].tipo == BOULDER_TYPE_BUTTERFLY) {
				ExplodirCentro(jogo_info, boulder_sound, x, y, 1);
			} else {
				ExplodirCentro(jogo_info, boulder_sound, x, y, 0);
			}

			return 1;
		break;
	}

	return 0;
}

//--------------------------------------------------------------------

// funcao que trata a funcionalidade da borboleta
int ButterflyHandler(mapa* jogo_info, strutc_sound* boulder_sound, int x, int y) {
	if (VerificarCondicaoExplosao(jogo_info, boulder_sound, x, y)) {
		return 1;
	}

	switch(jogo_info->tiles[x][y].estado) {
		//esquerda
		case 0 :
			if (ChecaTileVazio(jogo_info, x, y-1)) {
				jogo_info->tiles[x][y].estado = 3;

				MoverPara(jogo_info, x, y, x, y-1);
			} else {
				if (ChecaTileVazio(jogo_info, x-1, y)) {
					MoverPara(jogo_info, x, y, x-1, y);
				} else {
					jogo_info->tiles[x][y].estado = 1;
				}
			}
		break;

		//baixo
		case 1 :
			if (ChecaTileVazio(jogo_info, x-1, y)) {
				jogo_info->tiles[x][y].estado = 0;
	
				MoverPara(jogo_info, x, y, x-1, y);
			} else {
				if (ChecaTileVazio(jogo_info, x, y+1)) {
					MoverPara(jogo_info, x, y, x, y+1);
				} else {
					jogo_info->tiles[x][y].estado = 2;
				}
			}
		break;

		//direita
		case 2 :
			if (ChecaTileVazio(jogo_info, x, y+1)) {
				jogo_info->tiles[x][y].estado = 1;

				MoverPara(jogo_info, x, y, x, y+1);
			} else {
				if (ChecaTileVazio(jogo_info, x+1, y)) {
					MoverPara(jogo_info, x, y, x+1, y);
				} else {
					jogo_info->tiles[x][y].estado = 3;
				}
			}
		break;

		//cima
		case 3 :
			if (ChecaTileVazio(jogo_info, x+1, y)) {
				jogo_info->tiles[x][y].estado = 2;

				MoverPara(jogo_info, x, y, x+1, y);
			} else {
				if (ChecaTileVazio(jogo_info, x, y-1)) {
					MoverPara(jogo_info, x, y, x, y-1);
				} else {
					jogo_info->tiles[x][y].estado = 0;
				}
			}
		break;
	}
	return 0;
}

// funcao que trata a funcionalidade do quadrado
int SquareHandler(mapa* jogo_info, strutc_sound* boulder_sound, int x, int y) {
    if (VerificarCondicaoExplosao(jogo_info, boulder_sound, x, y)) {
		return 1;
	}

	switch(jogo_info->tiles[x][y].estado) {
    	//esquerda
		case 0 :
		    if (ChecaTileVazio(jogo_info, x, y+1)) {
				jogo_info->tiles[x][y].estado = 3;

				MoverPara(jogo_info, x, y, x, y+1);
			} else {
				if (ChecaTileVazio(jogo_info, x-1, y)) {
		    		MoverPara(jogo_info, x, y, x-1, y);
				} else {
					jogo_info->tiles[x][y].estado = 1;
				}
			}
	    break;

		//cima
		case 1 :
			if (ChecaTileVazio(jogo_info, x-1, y)) {
				jogo_info->tiles[x][y].estado = 0;

				MoverPara(jogo_info, x, y, x-1, y);
			} else {
				if (ChecaTileVazio(jogo_info, x, y-1)) {
					MoverPara(jogo_info, x, y, x, y-1);
				} else {
					jogo_info->tiles[x][y].estado = 2;
				}
			}
		break;

		//direita
		case 2 :
			if (ChecaTileVazio(jogo_info, x, y-1)) {
				jogo_info->tiles[x][y].estado = 1;

				MoverPara(jogo_info, x, y, x, y-1);
			} else {
				if (ChecaTileVazio(jogo_info, x+1, y)) {
					MoverPara(jogo_info, x, y, x+1, y);
				} else {
					jogo_info->tiles[x][y].estado = 3;
				}
			}
		break;
	
	    //baixo
		case 3 :
			if (ChecaTileVazio(jogo_info, x+1, y)) {
				jogo_info->tiles[x][y].estado = 2;

				MoverPara(jogo_info, x, y, x+1, y);
			} else {
				if (ChecaTileVazio(jogo_info, x, y+1)) {
					MoverPara(jogo_info, x, y, x, y+1);
				} else {
					jogo_info->tiles[x][y].estado = 0;
	    		}
			}
		break;
	}
	return 0;
}

//--------------------------------------------------------------------

// funcao que trata a funcionalidade das amoebas
int AmoebaHandler(mapa* jogo_info, int x, int y) {
	if (jogo_info->AmoebaEstado == 2) {
		TransformaTile(jogo_info, x, y, BOULDER_TYPE_CRISTAL, 0);
		
		return 1;
	}

	if (jogo_info->AmoebaEstado == 3) {
		TransformaTile(jogo_info, x, y, BOULDER_TYPE_BOULDER, 0);

		return 1;
	}

	//---------------------------------------------------//
	int temp;
	if (jogo_info->AmoebaEstado == 1) {
		temp = rand() % 10;
	} else {
		temp = rand() % 70;
	}
	//---------------------------------------------------//

	if (jogo_info->tiles[x+1][y].tipo == BOULDER_TYPE_AIR || jogo_info->tiles[x+1][y].tipo == BOULDER_TYPE_DIRT) {
		if (temp == 0) {
			TransformaTile(jogo_info, x+1, y, BOULDER_TYPE_AMOEBA, 0);
			
			jogo_info->tiles[x+1][y].cheque_atualizado = 1;
		}

		return 1;
	}
	if (jogo_info->tiles[x-1][y].tipo == BOULDER_TYPE_AIR || jogo_info->tiles[x-1][y].tipo == BOULDER_TYPE_DIRT) {
		if (temp == 1) {
			TransformaTile(jogo_info, x-1, y, BOULDER_TYPE_AMOEBA, 0);

			jogo_info->tiles[x-1][y].cheque_atualizado = 0;
		}

		return 1;
	}
	if (jogo_info->tiles[x][y+1].tipo == BOULDER_TYPE_AIR || jogo_info->tiles[x][y+1].tipo == BOULDER_TYPE_DIRT) {
		if (temp == 2) {
			TransformaTile(jogo_info, x, y+1, BOULDER_TYPE_AMOEBA, 0);

			jogo_info->tiles[x][y+1].cheque_atualizado = 1;
		}

		return 1;
	}
	if (jogo_info->tiles[x][y-1].tipo == BOULDER_TYPE_AIR || jogo_info->tiles[x][y-1].tipo == BOULDER_TYPE_DIRT) {
		if (temp == 3) {
			TransformaTile(jogo_info, x, y-1, BOULDER_TYPE_AMOEBA, 0);

			jogo_info->tiles[x][y-1].cheque_atualizado = 0;
		}

		return 1;
	}

	return 0;
}