all: boulder

clean :
	rm Game.o BoulderEssencials.o Drawer.o GameUpdater.o EntityHandler.o MapHandler.o PlayerHandler.o TileHandler.o KeyboardHandler.o

purge : clean
	rm boulder savefile

#--

ALLEGRO5 = -lallegro -lallegro_image -lallegro_font -lallegro_primitives -lallegro_ttf -lallegro_audio -lallegro_acodec

#--

boulder : Game.o BoulderEssencials.o Drawer.o GameUpdater.o EntityHandler.o MapHandler.o PlayerHandler.o TileHandler.o KeyboardHandler.o
	gcc -o boulder Game.o BoulderEssencials.o Drawer.o GameUpdater.o EntityHandler.o MapHandler.o PlayerHandler.o TileHandler.o KeyboardHandler.o $(ALLEGRO5)

#--

Game.o : Game.c
	gcc -c -Wall Game.c

#--

BoulderEssencials.o : BoulderEssencials.c
	gcc -c -Wall BoulderEssencials.c

Drawer.o : Drawer.c
	gcc -c -Wall Drawer.c

GameUpdater.o : GameUpdater.c
	gcc -c -Wall GameUpdater.c

EntityHandler.o : EntityHandler.c
	gcc -c -Wall EntityHandler.c

MapHandler.o : MapHandler.c
	gcc -c -Wall MapHandler.c

PlayerHandler.o : PlayerHandler.c
	gcc -c -Wall PlayerHandler.c

TileHandler.o : TileHandler.c
	gcc -c -Wall TileHandler.c

KeyboardHandler.o : KeyboardHandler.c
	gcc -c -Wall KeyboardHandler.c