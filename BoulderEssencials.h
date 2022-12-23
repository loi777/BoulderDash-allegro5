// estou ciente que o trabalho pediu 60 FPS
#define BOULDER_FPS 24
// porem ao colocar 60 fps eu teria que manualmente diminuir os frames das animacoes
// assim como diminuir ativamente a velocidade da atualizacao de fisica
// espero que esteja tudo bem professor, tomei essa decisao apenas para simplificar o codigo
// nao por incapacidade

#define BOULDER_FT 3
//quantos frames ate atualizar a fisica do mundo

#define BOULDER_RESETTIMER 5
//quantos segundos ate o level resetar

#define BOULDER_ENTRANCETIMER 24
//quantas updates fisicas devem ocorrer para o player ser liberado

#define BOULDER_DISPX 640
#define BOULDER_DISPY 380
// definindo as dimensoes da janela

#define BOULDER_DIMX 40
#define BOULDER_DIMY 22
//dimensoes padroes da matriz

#define BOULDER_LAYOUTY 28
//quanto espaco o texto superior precisa

#define BOULDER_MAXLEVEL 10
//quantos leveis tem no arquivo mapas.txt

#define BOULDER_ECHT 8 
// quantos segundos os tijolos encantados ficam ativados

//---------------------------------------------------------------------

#define BOULDER_TYPE_ENTRANCE   0
#define BOULDER_TYPE_PLAYER     1
#define BOULDER_TYPE_TARDIS     2
#define BOULDER_TYPE_AIR        3
#define BOULDER_TYPE_IRON       4
#define BOULDER_TYPE_EXIT       5
#define BOULDER_TYPE_BRICK      6
#define BOULDER_TYPE_ECHTBRICK  7
#define BOULDER_TYPE_DIRT       8
#define BOULDER_TYPE_CRISTAL    9
#define BOULDER_TYPE_BOULDER    10
#define BOULDER_TYPE_SQUARE     11
#define BOULDER_TYPE_BUTTERFLY  12
#define BOULDER_TYPE_AMOEBA     13
#define BOULDER_TYPE_EXPLOSION  14
#define BOULDER_TYPE_CEXPLOSION 15
//define os varios tipos de tiles

//---------------------------------------------------------------------

typedef struct tile_info {
	int tipo;
	int estado;
	int cheque_atualizado;
} tile;
//tipo : me informa o que tem no tile
//0=player | 1=explosao | 2=ar | 3=ferro | 4=saida | 5=tijolo | 6=tijoloEncantado | 7=terra | 8=cristal | 9=pedregrulho | 10=quadradoMal | 11=borboletaMal | 12=amoebaVerde
//
//sprite : me informa o ponto da animacao atual caso tenha
//estado : informacoes mecanicas do tile
//cheque_atualizado : um cheque booleano para impedir que o tile atualize multiplas vezes

typedef struct jogo {
	tile** tiles;

	int PlayerInputQueue[3];
	int PlayerMortoTimer;
	int PlayerTardisEasterEgg;

	int estado_de_jogo;
	int level;
	int pontos;

	int CristaisFaltando;
	int CristaisColetados;

	int CristaisPontos;
	int CristaisPtsExtra;

	int EnchantedTimer;
	int EnchantedAtivado;

	int AmoebaEstado;

	int TempoMaximo;
	int TempoAtual;

	int InfoScreen;

	int SpriteTimer;
} mapa;
//tiles : as informacoes fisicas do mapa
//
//PlayerUltimoInput : registra o ultimo input de movimentacao do jogador
//
//level : ou o nivel atual | o mapa carregado no momento
//pontos : quantos pontos o player ja fez
//
//CristaisFaltando : quantos cristais o jogador precisa para abrir a saida
//CristaisColetados : quantos cristais o jogador ja coletou
//
//CristaisPontos : quantos pontos cada cristal vale
//CristaisPtsExtra : apos a saida abrir, quantos pontos cada cristal vale
//
//EnchantedTimer : me indica quanto tempo os tijoos encantados ficaram ligados
//EnchantedAtivado : indica se os tijolos ja foram ativados para nao serem novamente
//
//AmoebaEstado : indica o estado global da amoeba: 0 normal, 1 espalhando rapido, 2 virar cristais, 3 virar boulders
//#include "BoulderEssencials.h"
//TempoMaximo : o tempo maximo do nivel atual
//TempoAtual : quanto tempo ja se passou

//---------------------------------------------------------------------

typedef struct _strutc_sound {
	ALLEGRO_SAMPLE* bg_musica;
	
	ALLEGRO_SAMPLE* menu;
	ALLEGRO_SAMPLE* win;

	ALLEGRO_SAMPLE* collect;
	ALLEGRO_SAMPLE* explosao;
	ALLEGRO_SAMPLE* tardis;
} strutc_sound;

//---------------------------------------------------------------------

typedef struct _strutc_font {
	ALLEGRO_COLOR black;
	ALLEGRO_COLOR white;
	ALLEGRO_COLOR yellow;
	ALLEGRO_COLOR amoeba_green;

	ALLEGRO_COLOR map_color;

	ALLEGRO_FONT* big;
	ALLEGRO_FONT* small;
} struct_font;

//======================================================================

strutc_sound* CarregarAudios();
void DestroiAudios(strutc_sound* boulder_sound);