#include "connected_room.hpp"


#pragma region MAIN
	ConnectedRoom::ConnectedRoom() : Room() {
		n_doors_max = MAX_CONNECTED_R;
		n_doors_sides = 0;
		//inizializza porte/stanze collegate
		for(int i = 0; i < n_doors_max; i++) {
			doors[i] = COORDINATE_ERROR;
		}
		door_positions[0] = Coordinate(size.x / 2, size.y - 1);
		door_positions[1] = Coordinate(size.x - 1, size.y / 2);
		door_positions[2] = Coordinate(size.x / 2, 0);
		door_positions[3] = Coordinate(0, size.y / 2);
		locked_doors = 0;
	}

	void ConnectedRoom::generate()
	{
		pUnionFind sets = new UnionFind();
		//GENERA MURI LATERALI
		generateSidesWalls();
		//GENERA PORTE
		generateDoors(sets);
		//CREA STANZA NELLA STANZA (QUADRATO VUOTO AL CENTRO)
		generateInnerRoom();
		//RIEMPI LA STANZA DI MURI E CORRIDOI
		generateAllPaths(sets);
		//FAI IN MODO CHE OGNI PUNTO SIA RAGGIUNGIBILE DA OGNI ALTRO PUNTO
		connectPaths(sets);
		//RIDIMENSIONA LA STANZA, OVVERO ESEGUI UN ALLARGAMENTO DI "X_SCALE" VOLTE
		resizeMap();
		sets->destroy();
	}
#pragma endregion MAIN



#pragma region AUSILIARIE
	Coordinate ConnectedRoom::calcDoorEntrance(Coordinate door_pos) {
		Coordinate res = COORDINATE_ERROR;
		int d = 0;
		while(res.equals(COORDINATE_ERROR) && d < DIRECTIONS_N) {
			if(Coordinate(door_pos, DIRECTIONS[d]).inBounds(COORDINATE_ONE, Coordinate(size, COORDINATE_NEGATIVE)))	//cerco la posizione interna alla stanza
				res = Coordinate(door_pos, DIRECTIONS[d].negative());
			else d++;
		}
		return res;
	}
	void ConnectedRoom::generateDoors(pUnionFind sets) {
		for(int door = 0; door < n_doors_sides; door++) {
			Coordinate door_pos = doors[door].times(1. / scale_x, 1);;
			door_pos.setMatrix(size_t);
			map[door_pos.single()] = CHAR_DOOR;
		}
	}
#pragma endregion AUSILIARIE


