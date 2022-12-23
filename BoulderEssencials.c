#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>

#include <stdlib.h>

#include "BoulderEssencials.h"

//-----------------------------------------------------------------

strutc_sound* CarregarAudios() {
	strutc_sound* boulder_sound = malloc(sizeof(strutc_sound));

	boulder_sound->bg_musica = al_load_sample("resources/first_level.wav");

	boulder_sound->menu = al_load_sample("resources/cheat.wav");
	boulder_sound->win = al_load_sample("resources/win.wav");

	boulder_sound->explosao = al_load_sample("resources/explosion.ogg");
	boulder_sound->collect = al_load_sample("resources/diamante.ogg");
	boulder_sound->tardis = al_load_sample("resources/easter_egg.wav");

	al_reserve_samples(4);

	return boulder_sound;
}

void DestroiAudios(strutc_sound* boulder_sound) {
	al_destroy_sample(boulder_sound->bg_musica);

	al_destroy_sample(boulder_sound->menu);
	al_destroy_sample(boulder_sound->win);

	al_destroy_sample(boulder_sound->explosao);
	al_destroy_sample(boulder_sound->collect);
	al_destroy_sample(boulder_sound->tardis);

	free(boulder_sound);
}

//------------------------------------------------------