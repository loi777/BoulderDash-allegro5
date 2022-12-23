#define PLAYER_MOVE_UP 0
#define PLAYER_MOVE_LEFT 1
#define PLAYER_MOVE_DOWN 2
#define PLAYER_MOVE_RIGHT 3
//define os varios tipos de movimentos do player

//----------------------------------------------

int EasterEggCheck(mapa* jogo_info, strutc_sound* boulder_sound, int x, int y);

void PLayerObterInput(mapa* jogo_info, ALLEGRO_EVENT evento_atual);
void PlayerLimpaInput(mapa* jogo_info, ALLEGRO_EVENT evento_atual);

void PlayerStandByAnimation(mapa* jogo_info, int x, int y);
void PlayerHandler(mapa* jogo_info, strutc_sound* boulder_sound, int x, int y);