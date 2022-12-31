//// STANZA CON PORTE


#ifndef CONNECTED_ROOM_HPP
#define CONNECTED_ROOM_HPP


#include "room.hpp"


class ConnectedRoom : public Room {
	private:
		int n_doors_max;						//massimo numero porte (dimensione array)
		int n_doors_sides;						//numero di lati occupati da una porta (attualmente)
		int locked_doors;						//numero di porte che richiedono chiave
		Coordinate door_positions[MAX_SIDES_R];
		pRoom connected[MAX_CONNECTED_R];		//stanze collegate nella rispettiva direzione con una porta
		Coordinate doors[MAX_DOORS];

		//// FUNZONI AUSILIARIE
		Coordinate calcDoorEntrance(Coordinate door_pos);	//calcola door.entrancePosition, il punto in cui si ritrova un character che attraversa la porta
		//FUNZIONI AUSILIARIE DI GENERAZIONE - PRINCIPALI
		void generateDoors(pUnionFind sets);	//genera le porte (generazione stanza)

	public:
		ConnectedRoom();

		void generate();
};

typedef ConnectedRoom *pConnectedRoom;




#endif