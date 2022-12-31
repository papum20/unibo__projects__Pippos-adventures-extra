#ifndef GENERAL_DEFINITIONS
#define GENERAL_DEFINITIONS



#include <curses.h>



#define FLOOR ' '
#define EMPTY '.'
#define WALL 'x'

//// REFRESH RATE : durata di un frame
#pragma region REFRESH_RATE
#define FRAMES_PER_SECOND 15					//aggiornamenti in un secondo
#define REFRESH_RATE (1. / FRAMES_PER_SECOND)	//durata di un aggiornamento
#define REFRESH_TIMER_INDEX 0					//indice del timer (implementazione)
#pragma endregion REFRESH_RATE


#pragma region CAMERA_DEFINITIONS
#define CAMERA_WIDTH  255//135	//larghezza (massima) inquadratura livello
#define CAMERA_HEIGHT 50//35	//altezza (massima) inquadratura livello
#pragma endregion CAMERA_DEFINITIONS


#pragma region ROOM_DEFINITIONS
#define SCALE_X 1//9									//coefficiente di "allargamento orizzontale" della stanza
#define SCALE_Y 1//6
#define ROOM_WIDTH_T 70//30								//larghezza (senza scala) stanza
#define ROOM_WIDTH (ROOM_WIDTH_T * SCALE_X)//320	//larghezza (reale) stanza
#define ROOM_HEIGHT_T 20//14//180
#define ROOM_HEIGHT (ROOM_HEIGHT_T * SCALE_Y)//180
#define ROOM_AREA_T (ROOM_WIDTH_T * ROOM_HEIGHT_T)
#define ROOM_AREA (20*70)//(ROOM_WIDTH * ROOM_HEIGHT)
#define CENTRAL_ROOM_WIDTH_T 9						//dimensioni dello spazio vuoto quadrato al centro (la width è prima dello scale)
#define CENTRAL_ROOM_HEIGHT_T 2
//CONNECTED ROOM
#define MAX_CONNECTED_R 5					//massimo numero di stanze collegate a ognuna
#define MAX_SIDES_R 4						//massimo numero di stanze (normali) collegate sui lati
#pragma endregion ROOM_DEFINITIONS

//// COSTANTI PER LA RAPPRESENTAZIONE GRAFICA
#pragma region GRAPHICS
#define COLORS_NUMBER 8
// COLORI RIDEFINITI:
//COLOR_BLACK
//COLOR_WHITE
//COLOR_RED
//COLOR_GREEN
//COLOR_BLUE
#define COLOR_BROWN COLOR_MAGENTA
#define COLOR_BROWN_LIGHT COLOR_YELLOW
#define COLOR_GREY COLOR_CYAN
// COLORI ASSOCIATI:
#define COLOR_WALL COLOR_BROWN_LIGHT
#define COLOR_TRANSPARENT COLOR_BROWN		//per il muro quando diventa più chiaro se c'è qualcosa dietro
#define COLOR_WALL_TOP COLOR_BLACK			//bordo superiore muro
#define COLOR_WALL_BASE COLOR_BLACK			//bordo superiore muro
#define COLOR_FLOOR COLOR_GREEN
#define COLOR_SHADOW COLOR_GREY				//per il pavimento con l'ombra di un oggetto
#define COLOR_DOOR COLOR_BLUE
#define COLOR_OUTSIDE_FG COLOR_WHITE
#define COLOR_OUTSIDE_BG COLOR_BLACK

#define COLOR_PLAYER COLOR_WHITE
#define COLOR_WEAPON COLOR_BLUE
#define COLOR_ENEMY COLOR_RED
#define COLOR_CHEST COLOR_BLUE
// CARATTERI:
#define CHAR_EMPTY ' '
#define CHAR_WALL_BASE '_'
#define CHAR_WEAPON_MASK 'm'				//quando si trova questo carattere in un'animazione, in quel punto non verrà rappresentato il personaggio
#define CHAR_OUTSIDE '.'
// DIMENSIONI
#define WALL_HEIGHT 1
#define DOOR_HEIGHT (WALL_HEIGHT + 1)
#define DOOR_WIDTH 3
#define DOOR_DEPTH 1
#pragma endregion GRAPHICS

//PLAYER
#define p_max_health 100

//// COMANDI
#pragma region KEY_BINDINGS
const int esc = 27; //tasto per uscire dall'inventario
const int invio = 10; //serve nel menu di usa e scarta dell'oggetto per accettare 

const int scroll_up = KEY_UP;
const int scroll_down = KEY_DOWN;
const int scroll_left = KEY_LEFT;
const int scroll_right = KEY_RIGHT;
#define KEY_PAUSE 'p'
#pragma endregion KEY_BINDINGS


#define ERROR_INT -1


#endif