#ifndef ROOM_HPP
#define ROOM_HPP


#include "coordinate.hpp"
#include "definitions.hpp"


//per ogni indice i, DIR_CHANCES[i] è la probabilità di generare i percorsi a partire da un punto (nella generazione della stanza);
//la prima posizione indica la probabilità di generare in 0 nuove direzioni (cioè di fermarsi)
const int DIR_CHANCES[DIRECTIONS_N + 1] = {5, 20, 10, 3, 1};
//COSTANTI PER MAKECONNECTION(): PER QUALI PORTE BLOCCARE CON CHIAVE
#define LOCKED_DOORS_MAX 1		//massimo numero di porte bloccate da chiave in una stanza
typedef char lock_type;
#define LOCK_TYPES_N 4			//numero di stati
#define LOCK_NONE	(lock_type)0
#define LOCK_OWN	(lock_type)1
#define LOCK_OTHER	(lock_type)2
#define LOCK_BOTH	(lock_type)3

// SPAZI
const Coordinate ZONE_DOOR_HORIZONTAL = Coordinate(DOOR_WIDTH, 3);		//zona in cui non possono spawnare muri di fronte alla porta (orizzontale)
const Coordinate ZONE_DOOR_VERTICAL = ZONE_DOOR_HORIZONTAL.swapped();
#define ZONE_SPAWN_DISTANCE 5											//distanza dal player in cui non possono spawnare nemici


#include "union_find.hpp"





class Room {
	private:
		Coordinate pos;											//coordinate rispetto alla prima stanza del livello

		//// FUNZIONI AUSILIARIE
		int getFreeCells(s_coord available[], Coordinate size);							//modifica l'array con le celle disponibili per lo spawn di qualcosa di dimensione size e ne ritorna il numero
		// ADD
		// FUNZIONI AUSILIARIE DI GENERAZIONE - SECONDARIE
		void generatePath(Coordinate s, pUnionFind sets);								//genera un percorso casuale a partire da x,y
		int getAdjacentWalls(Coordinate out[], UnionFind sets, s_coord currentParent);	//riempie out con i muri adiacenti a una casella del set e ne ritorna il numero; PRECONDIZIONE: currentParent è parent del suo set
		int getBorderWalls(Coordinate walls[], int directions[], int walls_n, UnionFind sets, s_coord parent, int distance);
					//riempie walls, già contenente i muri adiacenti, con i soli muri di confine tra il set di parent e un altro (con spessore distance)
					//e directions con le rispettive direzioni; ne ritorna il numero
	protected:
		void rec(pUnionFind sets, Coordinate pos);
		void makeUnion(pUnionFind sets);

		Coordinate size;		//dimensioni effettive
		Coordinate size_t;		//dimensioni senza l'allargamento scale_x
		Coordinate scale;		//ridimensionamento
		char map[ROOM_AREA];
		
	// FUNZIONI AUSILIARIE DI GENERAZIONE - PRINCIPALI
		void generateSidesWalls();
		void generateInnerRoom(pUnionFind sets);
		void generateAllPaths(pUnionFind sets);
		void connectPaths(pUnionFind sets);		//fa in modo che ogni punto sia raggiungibile da ogni altro
		void resizeMap();						//ridimensiona la mappa, allargando quella temporanea generata di X_SCALE


	public:
		Room();
		void print();
		void print2();
		
		virtual void generate(); 								//genera uno schema randomico per i muri, inserendoli nell'array map
		
		// DISEGNO
																									//inquadra solo un rettangolo con le dimensioni dei parametri intorno al giocatore

		// GET
		//void getMap(pPhysical map[], Coordinate &size);			//modifica mappa, ritorna dimensioni

		// SET

		//funzioni di debug: stampano la stanza in caratteri (rispettivamente con size e size_t)
		//void debug();
		//void debug2();
};



typedef Room *pRoom;


#include "math.hpp"


#endif