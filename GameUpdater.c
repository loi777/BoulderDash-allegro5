#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>

#include "BoulderEssencials.h"

#include "MapHandler.h"
#include "GameUpdater.h"
#include "TileHandler.h"
#include "EntityHandler.h"
#include "PlayerHandler.h"

//--------------------------------------------------------------------

// atualizador que processa o mapa
void AtualizarFisicaMapa(mapa* jogo_info, strutc_sound* boulder_sound) {
	int AmoebaCheck = 0;
	int AmoebaQuantidade = 0;
	int reset_check = 0;

	for (int j = 0; j < BOULDER_DIMY; j++) {
		for (int i = 0; i < BOULDER_DIMX; i++) {

			if (!jogo_info->tiles[i][j].cheque_atualizado) {
				switch(jogo_info->tiles[i][j].tipo) {

					case BOULDER_TYPE_ENTRANCE :
						jogo_info->TempoAtual = 0;

						jogo_info->tiles[i][j].estado++;
						if (jogo_info->tiles[i][j].estado >= BOULDER_ENTRANCETIMER) {
							TransformaTile(jogo_info, i, j, BOULDER_TYPE_PLAYER, 0);
						}

						reset_check = 1;
					break;
					
					case BOULDER_TYPE_PLAYER :
						if (EasterEggCheck(jogo_info, boulder_sound, i, j)) {
							continue;
						}

						PlayerStandByAnimation(jogo_info, i, j);
						
						PlayerHandler(jogo_info, boulder_sound, i, j);

						reset_check = 1;
					break;

					case BOULDER_TYPE_EXPLOSION :
						jogo_info->tiles[i][j].estado++;
						if (jogo_info->tiles[i][j].estado > 3) {
							TransformaTile(jogo_info, i, j, BOULDER_TYPE_AIR, 0);
						}
					break;

					case BOULDER_TYPE_CEXPLOSION :
						jogo_info->tiles[i][j].estado++;
						if (jogo_info->tiles[i][j].estado > 3) {
							TransformaTile(jogo_info, i, j, BOULDER_TYPE_CRISTAL, 0);
						}
					break;

					case BOULDER_TYPE_CRISTAL :
						GravidadeHandler(jogo_info, boulder_sound, i, j);
					break;

					case BOULDER_TYPE_BOULDER :
						GravidadeHandler(jogo_info, boulder_sound, i, j);
					break;

					case BOULDER_TYPE_SQUARE :
						SquareHandler(jogo_info, boulder_sound, i, j);
					break;

					case BOULDER_TYPE_BUTTERFLY :
						ButterflyHandler(jogo_info, boulder_sound, i, j);
					break;

					case BOULDER_TYPE_AMOEBA :
						AmoebaQuantidade++;

						if (AmoebaHandler(jogo_info, i, j)) {
							AmoebaCheck = 1;
						}
					break;
				}
			}

			jogo_info->tiles[i][j].cheque_atualizado = 0;
		}
	}

	if (AmoebaQuantidade >= 220) {
		jogo_info->AmoebaEstado = 3;
	}

	if (!AmoebaCheck) {
		jogo_info->AmoebaEstado = 2;
	}

	if (!reset_check) {
		if (jogo_info->PlayerMortoTimer == 0) {
			jogo_info->PlayerMortoTimer = BOULDER_RESETTIMER;
		}
	}
}

//--------------------------------------------------------------------

// atualizador que apenas desenha o mapa
void AtualizarVisualMapa(mapa* jogo_info, ALLEGRO_BITMAP* pixel_sheet) {

	jogo_info->SpriteTimer++;
	if (jogo_info->SpriteTimer == 72) {
		jogo_info->SpriteTimer = 0;
	}

	for (int j = 0; j < BOULDER_DIMY; j++) {
		for (int i = 0; i < BOULDER_DIMX; i++) {

			switch(jogo_info->tiles[i][j].tipo) {
				case BOULDER_TYPE_ENTRANCE :
					al_draw_bitmap_region(pixel_sheet, 16*((jogo_info->SpriteTimer / 3) % 2), 48, 16, 16, i*16, BOULDER_LAYOUTY + j*16, 0);
				break;

				case BOULDER_TYPE_PLAYER :

					switch (jogo_info->PlayerInputQueue[0]) {

						case 0 :
							al_draw_bitmap_region(pixel_sheet, 0 + 16*((jogo_info->SpriteTimer / 2) % 7), 16, 16, 16, i*16, BOULDER_LAYOUTY + j*16, 0);
						break;

						case 1 :
							al_draw_bitmap_region(pixel_sheet, 0 + 16*((jogo_info->SpriteTimer / 2) % 7), 16, 16, 16, i*16, BOULDER_LAYOUTY + j*16, 0);
						break;

						case 2 :
							al_draw_bitmap_region(pixel_sheet, 0 + 16*((jogo_info->SpriteTimer / 2) % 7), 32, 16, 16, i*16, BOULDER_LAYOUTY + j*16, 0);
						break;

						case 3 :
							al_draw_bitmap_region(pixel_sheet, 0 + 16*((jogo_info->SpriteTimer / 2) % 7), 32, 16, 16, i*16, BOULDER_LAYOUTY + j*16, 0);
						break;

						default :
							if (jogo_info->tiles[i][j].estado) {
								al_draw_bitmap_region(pixel_sheet, 0 + 16*((jogo_info->SpriteTimer / 3) % 3), 0, 16, 16, i*16, BOULDER_LAYOUTY + j*16, 0);
							} else {
								al_draw_bitmap_region(pixel_sheet, 48 + 16*((jogo_info->SpriteTimer / 2) % 4), 0, 16, 16, i*16, BOULDER_LAYOUTY + j*16, 0);
							}
						break;
					}

				break;

				case BOULDER_TYPE_TARDIS :
					al_draw_bitmap_region(pixel_sheet, 96 + 16*((jogo_info->SpriteTimer / 4) % 2), 48, 16, 16, i*16, BOULDER_LAYOUTY + j*16, 0);
				break;
				
				case BOULDER_TYPE_EXPLOSION :
					al_draw_bitmap_region(pixel_sheet, 112, 64 + 16*(jogo_info->SpriteTimer % 4), 16, 16, i*16, BOULDER_LAYOUTY + j*16, 0);
				break;

				case BOULDER_TYPE_CEXPLOSION :
					al_draw_bitmap_region(pixel_sheet, 128, 64 + 16*(jogo_info->SpriteTimer % 4), 16, 16, i*16, BOULDER_LAYOUTY + j*16, 0);
				break;

				case BOULDER_TYPE_AIR :
					al_draw_bitmap_region(pixel_sheet, 112, 112, 16, 16, i*16, BOULDER_LAYOUTY + j*16, 0);
				break;

				case BOULDER_TYPE_IRON :
					al_draw_bitmap_region(pixel_sheet, 0, 48, 16, 16, i*16, BOULDER_LAYOUTY + j*16, 0);
				break;

				case BOULDER_TYPE_EXIT :
					if (jogo_info->CristaisColetados < jogo_info->CristaisFaltando) {
						al_draw_bitmap_region(pixel_sheet, 0, 48, 16, 16, i*16, BOULDER_LAYOUTY + j*16, 0);
					} else {
						al_draw_bitmap_region(pixel_sheet, 16*((jogo_info->SpriteTimer / 3) % 2), 48, 16, 16, i*16, BOULDER_LAYOUTY + j*16, 0);
					}
				break;

				case BOULDER_TYPE_BRICK :
					al_draw_bitmap_region(pixel_sheet, 32, 48, 16, 16, i*16, BOULDER_LAYOUTY + j*16, 0);
				break;

				case BOULDER_TYPE_ECHTBRICK :
					if (jogo_info->EnchantedAtivado) {
						al_draw_bitmap_region(pixel_sheet, 32, 64 + 16*(jogo_info->SpriteTimer % 4), 16, 16, i*16, BOULDER_LAYOUTY + j*16, 0);
					} else {
						al_draw_bitmap_region(pixel_sheet, 32, 48, 16, 16, i*16, BOULDER_LAYOUTY + j*16, 0);
					}
				break;

				case BOULDER_TYPE_DIRT :
					al_draw_bitmap_region(pixel_sheet, 48, 48, 16, 16, i*16, BOULDER_LAYOUTY + j*16, 0);
				break;

				case BOULDER_TYPE_CRISTAL :
					al_draw_bitmap_region(pixel_sheet, 0 + 16 * ((jogo_info->SpriteTimer / 4) % 2), 112 - (16 * (jogo_info->SpriteTimer % 4)), 16, 16, i*16, BOULDER_LAYOUTY + j*16, 0);
				break;

				case BOULDER_TYPE_BOULDER :
					al_draw_bitmap_region(pixel_sheet, 80, 48, 16, 16, i*16, BOULDER_LAYOUTY + j*16, 0);
				break;

				case BOULDER_TYPE_SQUARE :
					al_draw_bitmap_region(pixel_sheet, 80, 112 - 16*(jogo_info->SpriteTimer % 4), 16, 16, i*16, BOULDER_LAYOUTY + j*16, 0);
				break;

				case BOULDER_TYPE_BUTTERFLY :
					al_draw_bitmap_region(pixel_sheet, 96, 112 - 16*(jogo_info->SpriteTimer % 4), 16, 16, i*16, BOULDER_LAYOUTY + j*16, 0);
				break;

				case BOULDER_TYPE_AMOEBA :
					al_draw_bitmap_region(pixel_sheet, 48, 64 + 16*(jogo_info->SpriteTimer % 4), 16, 16, i*16, BOULDER_LAYOUTY + j*16, 0);
				break;
			}
		}
	}
}

//--------------------------------------------------------------------

// atualizador por segundo do jogo
void AtualizarRelogio(mapa* jogo_info, strutc_sound* boulder_sound) {
	// atualiza o cronometro para reiniciar o nivel
	if (jogo_info->PlayerMortoTimer > 0) {
		jogo_info->PlayerMortoTimer--;

		if (jogo_info->PlayerMortoTimer <= 0) {
			CarregarMapa(jogo_info, boulder_sound, jogo_info->level);

			return;
		}
	}

	// atualiza os tijolos encantados
	if (jogo_info->EnchantedAtivado) {
		jogo_info->EnchantedTimer++;

		if (jogo_info->EnchantedTimer >= BOULDER_ECHT) {
			jogo_info->EnchantedAtivado = 0;
		}
	}

	// atualiza o cronometro do jogo
	if (jogo_info->TempoMaximo > jogo_info->TempoAtual) {
		jogo_info->TempoAtual++;

		// se resta apenas 40% do tempo aumenta a agressividade da amoeba
		if (jogo_info->AmoebaEstado == 0) {
			if (jogo_info->TempoAtual >= (jogo_info->TempoMaximo * 6) / 10) {
				jogo_info->AmoebaEstado = 1;
			}
		}
	}
}