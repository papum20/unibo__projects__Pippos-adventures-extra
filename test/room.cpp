#include "room.hpp"
#include <iostream>
using namespace std;

	Room::Room() {
		//inzializza stanza
		scale = Coordinate(SCALE_X, SCALE_Y);
		size_t = Coordinate(ROOM_WIDTH_T, ROOM_HEIGHT_T);
		size = size_t.times(scale.x, scale.y);

		//mappe
		for(int i = 0; i < size.x * size.y; i++) {
			map[i] = EMPTY;
		}
	}

	void Room::generate()
	{
		pUnionFind sets = new UnionFind();
		//GENERA MURI LATERALI
		generateSidesWalls();
		//GENERA PORTE
		//CREA STANZA NELLA STANZA (QUADRATO VUOTO AL CENTRO)
		generateInnerRoom(sets);
		//RIEMPI LA STANZA DI MURI E CORRIDOI
		generateAllPaths(sets);
		print2();
		char c;
		sets->print(map);
		cin >> c;
		makeUnion(sets);
		sets->print(map);
		cin >> c;
		
		//FAI IN MODO CHE OGNI PUNTO SIA RAGGIUNGIBILE DA OGNI ALTRO PUNTO
		connectPaths(sets);
		print2();
		return;
		//RIDIMENSIONA LA STANZA, OVVERO ESEGUI UN ALLARGAMENTO DI "X_SCALE" VOLTE
		resizeMap();
	}




	void Room::print() {
		for(int y = 0; y < size.y; y++)	 {
			for(int  x = 0; x < size.x; x++) {
				Coordinate c = Coordinate(x,y);
				cout << map[c.single_set(size)];
			} cout << endl;
		}	cout << "\n\n";
	}
	void Room::print2() {
		for(int y = 0; y < size_t.y; y++)	 {
			for(int  x = 0; x < size_t.x; x++) {
				Coordinate c = Coordinate(x,y);
				cout << map[c.single_set(size)];
			} cout << endl;
		}	cout << "\n\n";
	}




	void Room::generateSidesWalls() {
		Coordinate i = Coordinate(COORDINATE_ZERO, size, COORDINATE_ZERO, size_t);
		do {
			map[i.single()] = WALL;
			if(i.x == 0 && i.y > 0 && i.y < size_t.y - 1) i.x = size_t.x - 1;
			else i.next();
		} while(!i.equals(COORDINATE_ZERO));
	}
	void Room::generateInnerRoom(pUnionFind sets) {
		Coordinate rstart = Coordinate((size_t.x - CENTRAL_ROOM_WIDTH_T) / 2, (size_t.y - CENTRAL_ROOM_HEIGHT_T) / 2), rend = Coordinate(rstart, Coordinate(CENTRAL_ROOM_WIDTH_T, CENTRAL_ROOM_HEIGHT_T));
		Coordinate i(rstart, size, rstart, rend);
		do {
			map[i.single()] = FLOOR;
			sets->makeSet(i.single());
			i.next();
		} while(!i.equals(i.start()));
	}
	void Room::generateAllPaths(pUnionFind sets) {
		Coordinate rand_start = Coordinate(COORDINATE_ZERO, size, COORDINATE_ZERO, size_t);
		rand_start.randomize(0, size_t.x, 0, size_t.y);
		Coordinate it = rand_start;
		do {			
			if(map[it.single()] == EMPTY) {
				map[it.single()] = FLOOR;
				sets->makeSet(it.single());
				generatePath(it, sets);
			}
			it.next();
		} while(!it.equals(rand_start));
	}
	void Room::connectPaths(pUnionFind sets) {
		s_coord currentSet = sets->firstSet();

		while(sets->getNumber() > 1) {		//finché ci sono parti non connesse
			sets->print(map);
			char c;
			cin >> c;

			bool hasConnected = false;
			int distance = 1;

			//TROVA I MURI ADIACENTI AL SET
			currentSet = sets->find(currentSet);
			Coordinate adjacentWalls[ROOM_AREA_T];		//muri adiacenti al set
			int breakDirections[ROOM_AREA_T];			//direzione in cui rompere un muro per unire due set
			int adjacentWalls_n = getAdjacentWalls(adjacentWalls, *sets, currentSet), borderWalls_n = 0;

			//FINCHÉ NON HA CONNESSO QUALCOSA, CERCA SET CHE CONFININO A UNA DETERMINATA DISTANZA OPPURE AUMENTA LA DISTANZA
			while(!hasConnected) {
				//OTTIENI I MURI DI CONFINE TRA DUE SET DIVERSI
				borderWalls_n = getBorderWalls(adjacentWalls, breakDirections, adjacentWalls_n, *sets, currentSet, distance);

				//SE NE HA TROVATI: CONNETTI (ROMPI IL MURO/I MURI)
				if(borderWalls_n != 0) {
					//ROMPI UN MURO A CASO TRA QUELLI TROVATI
					int broken_ind = rand() % borderWalls_n;
					
					//ROMPI distance MURI A PARTIRE DA QUELLO, NELLA GIUSTA DIREZIONE
					for(int dist = 0; dist <= distance; dist++) {
						Coordinate dir = DIRECTIONS[breakDirections[broken_ind]];
						Coordinate broken = Coordinate(adjacentWalls[broken_ind], dir.times(dist, dist));
						broken.setMatrix(size);
						map[broken.single()] = FLOOR;
						sets->merge(currentSet, broken.single());
					}
					hasConnected = true;
				}
				//ALTRIMENTI CERCA A UNA DISTANZA MAGGIORE
				else
					distance++;
			}
		}
	}
	void Room::resizeMap() {
		for(s_coord xy = size.x * size.y - 1; xy >= 0; xy--) {
			Coordinate xy_t = Coordinate(xy, size).times(1. / scale.x, 1. / scale.y);
			map[xy] = map[xy_t.single()];
		}
	}
	void Room::generatePath(Coordinate s, pUnionFind sets)
	{
		s.setMatrix(size);
		map[s.single()] = FLOOR;

		bool used_dirs[DIRECTIONS_N];	//direzioni usate
		int used_dirs_n;
		bool new_dirs[DIRECTIONS_N];	//nuove direzioni da generare
		int new_dirs_n;

		// CONTA DIREZIONI INUTILIZZABILI (PERCHÈ GIÀ USATE O FUORI DALLA MAPPA)
		int tot_chance = DIR_CHANCES[0];	//somma delle probabilità delle direzioni disponibili
		int unused_dirs_n = 0;
		for(int d = 0; d < DIRECTIONS_N; d++) {
			Coordinate nxt = Coordinate(Coordinate(s, DIRECTIONS[d]), size, COORDINATE_ZERO, size_t);
			if(nxt.inOwnBounds() && map[nxt.single()] == EMPTY) {
				used_dirs[d] = false;
				unused_dirs_n++;
				tot_chance += DIR_CHANCES[unused_dirs_n];
			} else used_dirs[d] = true;
		}
		used_dirs_n = DIRECTIONS_N - unused_dirs_n;

		if(used_dirs_n != DIRECTIONS_N) {
			// SCEGLI NUMERO DI DIREZIONI DA GENERARE (TRA QUELLE DISPONIBILI)
			new_dirs_n = 0;
			int new_dirs_r = rand() % tot_chance;	//indice per generare un numero random di nuove direzioni
			int chance_counter = 0;
			while(new_dirs_r >= chance_counter + DIR_CHANCES[new_dirs_n]) {
				chance_counter += DIR_CHANCES[new_dirs_n];
				new_dirs_n++;
			}

			//	SCEGLI DIREZIONI IN CUI GENERARE
			for(int d = 0; d < DIRECTIONS_N; d++) new_dirs[d] = false;
			for(int i = 0; i < new_dirs_n; i++) {
				int r = rand() % (new_dirs_n - i);
				int d = 0;
				while(r > 0 || used_dirs[d] || new_dirs[d]) {
					if(!used_dirs[d] && !new_dirs[d]) r--;
					d++;
				}
				new_dirs[d] = true;
			}

			// PRIMA GENERA MURI E CASELLE ADIACENTI,
			for(int d = 0; d < DIRECTIONS_N; d++) {
				Coordinate nxt = Coordinate (s, DIRECTIONS[d]);
				if(nxt.inBounds(COORDINATE_ZERO, size_t)) {
					if(new_dirs[d]) map[nxt.single()] = FLOOR;
					else if(!used_dirs[d] && map[nxt.single()] == EMPTY) map[nxt.single()] = WALL;
				}
			}
			//POI VA IN RICORSIONE SULLE DIREZIONI
			for(int d = 0; d < DIRECTIONS_N; d++) {
				if(new_dirs[d]) {
					Coordinate nxt = Coordinate(s, DIRECTIONS[d]);
					sets->makeSet(nxt.single());
					sets->merge(s.single(), nxt.single());
					generatePath(nxt, sets);
				}
			}
		}
	}

	void Room::makeUnion(pUnionFind sets) {
		Coordinate it = Coordinate(COORDINATE_ZERO, size, COORDINATE_ZERO, size_t);
		do {
			rec(sets,pos);
			it.next();
		} while(!it.equals(COORDINATE_ZERO));
	}
	void Room::rec(pUnionFind sets, Coordinate pos) {
		for(int d = 0; d < DIRECTIONS_N; d++) {
			Coordinate nxt = Coordinate(pos, DIRECTIONS[d]);
			if(nxt.inBounds(COORDINATE_ZERO, size_t) && map[nxt.single_set(size)] == FLOOR) {
				if(map[nxt.single_set(size)] &&  sets->find(pos.single_set(size)) != sets->find(pos.single_set(size))) {
					sets->merge(pos.single_set(size), nxt.single_set(size));
					rec(sets, nxt);
				}
			}
		}
	}

	int Room::getAdjacentWalls(Coordinate out[], UnionFind sets, s_coord currentParent) {
		int walls = 0;
		s_coord square = currentParent;
		do {
			for(int d = 0; d < DIRECTIONS_N; d++) {
				Coordinate nxt = Coordinate(Coordinate(square, size), DIRECTIONS[d]);
				if(nxt.inBounds(COORDINATE_ZERO, size_t)) {
					if(map[nxt.single_set(size)] == WALL) {
						out[walls] = nxt;
						walls++;
					}
				}
			}
			square = sets.next(square);
		} while(square != currentParent);
		return walls;
	}
	int Room::getBorderWalls(Coordinate walls[], int directions[], int walls_n, UnionFind sets, s_coord parent, int distance) {
		int border_n = 0;
		for(int i = 0; i < walls_n; i++)
		{
			int rand_d = rand() % DIRECTIONS_N, d = 0;		//inizio a controllare da una direzione casuale per avere più casualità
			bool found = false;
			while(!found && d < DIRECTIONS_N) {
				Coordinate dir = DIRECTIONS[rand_d];									//direzione in cui si prova a "rompere" il muro per connettere
				Coordinate nxt = Coordinate(walls[i], dir.times(distance, distance));	//casella da controllare dopo aver rotto il muro

				if(nxt.inBounds(COORDINATE_ZERO, size_t) && sets.find(nxt.single_set(size)) != parent) {
					walls[border_n] = walls[i];
					directions[border_n] = rand_d;
					border_n++;
					found = true;
				} else {
					d++;
					rand_d = (rand_d + 1) % DIRECTIONS_N;
				}
			}
		}
		return border_n;
	}
