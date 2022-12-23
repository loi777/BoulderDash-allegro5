#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>

#include <stdio.h>
#include <stdlib.h>

#include "BoulderEssencials.h"

#include "Drawer.h"

// carrega e alloca memoria de uma fonte minha
struct_font* CarregarFonte() {
	struct_font* boulder_font = malloc(sizeof(struct_font));

	boulder_font->black = al_map_rgb(0, 0, 0);
	boulder_font->white = al_map_rgb(255, 255, 255);
	boulder_font->yellow = al_map_rgb(255, 247, 0);
	boulder_font->amoeba_green = al_map_rgb(110, 206, 115);

	boulder_font->map_color = al_map_rgb(0, 0, 0);

	boulder_font->big = al_load_font("resources/pixeboy.ttf", 32, 0);
	boulder_font->small = al_load_font("resources/pixeboy.ttf", 20, 0);

	return boulder_font;
}

// destroi uma estrutura font minha
void DestroiFonte(struct_font* boulder_font) {
	al_destroy_font(boulder_font->big);
	al_destroy_font(boulder_font->small);

	free(boulder_font);
}

//--------------------------------------------------------------------

//escreve um texto no formato desejavel para o layout superior
static void desenha_texto_formatado(ALLEGRO_FONT* font, int x, int y, ALLEGRO_COLOR cor, int halign, int valor, int maximo) {
	int intcaracteres = valor;
	char texto[7];

	if (intcaracteres >= maximo) 
		intcaracteres = maximo - 1;
	if (intcaracteres < 0)
		intcaracteres = 0;

	sprintf(texto, "%d", intcaracteres);
	al_draw_text(font, cor, x, BOULDER_LAYOUTY - y, halign, texto);
}

//desenha todo o layout superior com suas informacoes
void AtualizarTexto(mapa* jogo_info, ALLEGRO_BITMAP* pixel_sheet, struct_font* boulder_font) {

	//desenha quantos cristais faltam
	desenha_texto_formatado(boulder_font->big, 43, 24, boulder_font->yellow, ALLEGRO_ALIGN_RIGHT, jogo_info->CristaisFaltando, 100);

	//desenha o iconezinho bunitinhu fofinhu no meio dos 2
	al_draw_bitmap_region(pixel_sheet, 128, 48, 16, 16, 45, 4, 0);

	//desenha quantos pontos cada cristal vale
	if (jogo_info->CristaisColetados < jogo_info->CristaisFaltando) {
		desenha_texto_formatado(boulder_font->big, 62, 24, boulder_font->white, ALLEGRO_ALIGN_LEFT, jogo_info->CristaisPontos, 100);
	} else {
		desenha_texto_formatado(boulder_font->big, 62, 24, boulder_font->white, ALLEGRO_ALIGN_LEFT, jogo_info->CristaisPtsExtra, 100);
	}

	//desenha quantos cristais foram coletados
	desenha_texto_formatado(boulder_font->big, 200, 24, boulder_font->yellow, ALLEGRO_ALIGN_LEFT, jogo_info->CristaisColetados, 100);

	// dese6nha quantos segundos restam
	desenha_texto_formatado(boulder_font->big, 432, 24, boulder_font->white, ALLEGRO_ALIGN_LEFT, jogo_info->TempoMaximo - jogo_info->TempoAtual, 1000);

	// desenha quantos pontos o player obteve no total
	desenha_texto_formatado(boulder_font->big, 600, 24, boulder_font->white, ALLEGRO_ALIGN_RIGHT, jogo_info->pontos, 1000000);
}

// desenha a tela de informacoes extras
void DesenhaInfoScreen(struct_font* boulder_font) {
	al_draw_filled_rectangle(0, 0, 288, BOULDER_DISPY, boulder_font->black);
	al_draw_filled_rectangle(0, 0, 280, BOULDER_DISPY, boulder_font->white);

	al_draw_text(boulder_font->big, boulder_font->black, 140, 5, 1, "luiz fernando");
	al_draw_text(boulder_font->big, boulder_font->black, 140, 27, 1, "grr20203965");

	al_draw_textf(boulder_font->small, boulder_font->black, 140, 60, 1, "movimentacao:");
	al_draw_textf(boulder_font->small, boulder_font->black, 140, 76, 1, "w.a.s.d  ou  setinhas do teclado");

	al_draw_textf(boulder_font->small, boulder_font->black, 140, 106, 1, "pgup retorna um nivel");
	al_draw_textf(boulder_font->small, boulder_font->black, 140, 122, 1, "pgdn avanca um nivel");

	al_draw_textf(boulder_font->small, boulder_font->black, 140, 152, 1, "easter egg: tardis");
	al_draw_textf(boulder_font->small, boulder_font->black, 140, 168, 1, "isole o player com pedras");

	al_draw_textf(boulder_font->small, boulder_font->black, 140, 198, 1, "esc  para sair");

	al_draw_textf(boulder_font->big, boulder_font->black, 140, 338, 1, "professor:");
	al_draw_textf(boulder_font->big, boulder_font->black, 140, 360, 1, "david menotti");
}

//--------------------------------------------------------------------

// funcao que desenha a tela final com os vencedores e o placar
void DesenhaTelaFinal(mapa* jogo_info, struct_font* boulder_font) {
	al_draw_filled_rectangle(0, 0, BOULDER_DISPX, BOULDER_DISPY, boulder_font->white);
	al_draw_filled_rectangle(7, 7, BOULDER_DISPX-7, BOULDER_DISPY-7, boulder_font->black);

	al_draw_text(boulder_font->big, boulder_font->white, (BOULDER_DISPX/2), 30, 1, "escreva no bash seu nome");
	al_draw_text(boulder_font->small, boulder_font->white, (BOULDER_DISPX/2), 54, 1, "maximo de 3 letras");

	al_flip_display();

	//---------------

	char novo_nome[4];
	scanf("%3s", novo_nome);

	//---------------

	FILE* saveFile = fopen("savefile", "a");
	fprintf(saveFile, " %d", jogo_info->pontos);
	fprintf(saveFile, " %s", novo_nome);
	fclose(saveFile);

	//---------------//---------------//---------------

	int pontuacoes_antigas[17];
	char** nomes_antigos = malloc(sizeof(char*)*17);
	for(int i = 0; i < 17; i ++) {
		nomes_antigos[i] = malloc(sizeof(char)*4);
	}

	//---------------

	int quantos_saves = 0;
	int temp = 0;
	int check = 1;

	saveFile = fopen("savefile", "r");

	while(fscanf(saveFile, " %d", &temp) > 0 && quantos_saves < 16) {
		if (check) {
			if (temp > jogo_info->pontos) {
				fscanf(saveFile, " %s", nomes_antigos[quantos_saves]);
				pontuacoes_antigas[quantos_saves] = temp;

			} else {
				check = 0;

				pontuacoes_antigas[quantos_saves] = jogo_info->pontos;
				strcpy(nomes_antigos[quantos_saves], novo_nome);

				if (quantos_saves < 15) {
					quantos_saves++;

					pontuacoes_antigas[quantos_saves] = temp;
					fscanf(saveFile, " %s", nomes_antigos[quantos_saves]);
				}
			}
		} else {
			fscanf(saveFile, " %s", nomes_antigos[quantos_saves]);
			pontuacoes_antigas[quantos_saves] = temp;
		}

		quantos_saves++;
	}

	if (!check) {
		quantos_saves--;
	}

	fclose(saveFile);

	// coleta todas as informacoes salvas ate entao
	// alem disso organiza a nova informacao no meio
	// tambem garante que nao tem mais de 17 saves no arquivo
	//---------------

	saveFile = fopen("savefile", "w");
	for (int i = 0; i < quantos_saves; i++) {
		fprintf(saveFile, " %d", pontuacoes_antigas[i]);
		fprintf(saveFile, " %s", nomes_antigos[i]);
	}
	fclose(saveFile);

	//---------------//---------------//---------------

	al_draw_filled_rectangle(0, 0, BOULDER_DISPX, BOULDER_DISPY, boulder_font->white);
	al_draw_filled_rectangle(7, 7, BOULDER_DISPX-7, BOULDER_DISPY-7, boulder_font->black);

	char buffer[7];

	for(int i = 0; i < quantos_saves; i++) {
		sprintf(buffer, "%d", pontuacoes_antigas[i]);

		al_draw_text(boulder_font->big, boulder_font->white, 200, 15+(23*i), 1, nomes_antigos[i]);
		al_draw_text(boulder_font->big, boulder_font->white, (BOULDER_DISPX/2), 15+(23*i), 1, "-");
		al_draw_text(boulder_font->big, boulder_font->white, 440, 15+(23*i), 1, buffer);
	}

	al_flip_display();
	
	//---------------//---------------//---------------

	// limpa a memoria allocada
	for(int i = 0; i < 17; i ++) {
		free(nomes_antigos[i]);
	}
	free(nomes_antigos);
}