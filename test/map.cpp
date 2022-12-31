
#include <iostream>
#include "union_find.hpp"
#include <ctime>
using namespace std;





#define EMPTY '.'
#define WALL 'x'
#define FLOOR ' '

#define HEIGHT 20
#define WIDTH 70
const Coordinate SIZE = Coordinate(WIDTH, HEIGHT);

const int DIR_CHANCES[DIRECTIONS_N + 1] = {5, 20, 10, 3, 1};
#define CHANCE_TOT 38

char map[HEIGHT][WIDTH];


void print() {
	for(int y = 0; y < HEIGHT; y++) {
		for(int x = 0; x < WIDTH; x++) {
			cout << map[y][x];
		}cout<<endl;
	}cout<<"\n\n";
}







void sideWalls();
void init();
void genAllWalls(pUnionFind sets);
void genWalls(pUnionFind sets,int x, int y);
void connect(pUnionFind sets);
int getAdjacentWalls(Coordinate out[], UnionFind sets, s_coord currentParent);
int getBorderWalls(Coordinate walls[], int directions[], int walls_n, UnionFind sets, s_coord parent, int distance);
		
void generate() {
	pUnionFind sets = new UnionFind();
	char c;
	init();
	sideWalls();	
	print();
	cin >>c;
	genAllWalls(sets);
	print();
	sets->print(map);
	cin >>c;
	connect(sets);
	print();
	sets->print(map);
	cin >>c;

}





int main() {
	srand(time(NULL));
	generate();
}









void genAllWalls(pUnionFind sets) {
	for(int y = 0; y < HEIGHT; y++) {
		for(int x = 0; x < WIDTH; x++) {
			if(map[y][x] == EMPTY) {
				sets->makeSet(Coordinate(x,y).single_set(SIZE));
				genWalls(sets,x, y);
			}
		}
	}
}
void genWalls(pUnionFind sets,int x, int y) {
	map[y][x] = FLOOR;
	Coordinate s = Coordinate(x,y);

	bool used_dirs[DIRECTIONS_N];	//direzioni usate
	bool new_dirs[DIRECTIONS_N];	//nuove direzioni da generare
	int new_dirs_n;

	// CONTA DIREZIONI INUTILIZZABILI (PERCHÈ GIÀ USATE O FUORI DALLA MAPPA)
	int tot_chance = DIR_CHANCES[0];	//somma delle probabilità delle direzioni disponibili
	int used_dirs_n = DIRECTIONS_N;
	for(int d = 0; d < DIRECTIONS_N; d++) {
		Coordinate dir = DIRECTIONS[d];
		Coordinate nxt = Coordinate(Coordinate(s, DIRECTIONS[d]), SIZE, COORDINATE_ZERO, SIZE);
		if(nxt.inOwnBounds() && map[nxt.inty()][nxt.intx()] == EMPTY) {
			used_dirs[d] = false;
			used_dirs_n--;
			tot_chance += DIR_CHANCES[DIRECTIONS_N - used_dirs_n];
		} else used_dirs[d] = true;
	}

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
			if(nxt.inBounds(COORDINATE_ZERO, SIZE)) {
				if(new_dirs[d]) map[nxt.inty()][nxt.intx()] = FLOOR;
				else if(!used_dirs[d] && map[nxt.inty()][nxt.intx()] == EMPTY) map[nxt.inty()][nxt.intx()] = WALL;
			}
		}
		//POI VA IN RICORSIONE SULLE DIREZIONI
		for(int d = 0; d < DIRECTIONS_N; d++) {
			if(new_dirs[d]) {
				Coordinate nxt = Coordinate(s, DIRECTIONS[d]);
				sets->makeSet(nxt.single_set(SIZE));
				sets->merge(s.single_set(SIZE), nxt.single_set(SIZE));
				genWalls(sets, nxt.x, nxt.y);
			}
		}
	}
}
void init() {
	for(int y = 0; y < HEIGHT; y++) {
		for(int x = 0; x < WIDTH; x++) map[y][x] = EMPTY;
	}
}
void sideWalls() {
	for(int y = 0; y < HEIGHT; y++) {
		int delta = 1;
		if(y != 0 && y < HEIGHT-1) delta = WIDTH - 1;
		for(int x = 0; x < WIDTH; x+=delta) {
			map[y][x] = WALL;
		}
	}
}

void connect(pUnionFind sets) {
		s_coord currentSet = sets->firstSet();

		while(sets->getNumber() > 1) {		//finché ci sono parti non connesse
			//print();
			//sets->print(map);
			//char c;
			//cin >> c;

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
					Coordinate dir = DIRECTIONS[breakDirections[broken_ind]];
					for(int dist = 0; dist <= distance; dist++) {
						Coordinate broken = Coordinate(adjacentWalls[broken_ind], dir.times(dist, dist));
						map[broken.inty()][broken.intx()] = FLOOR;
						sets->makeSet(broken.single_set(SIZE));
						sets->merge(currentSet, broken.single_set(SIZE));
					}
					hasConnected = true;
				}
				//ALTRIMENTI CERCA A UNA DISTANZA MAGGIORE
				else
					distance++;
			}
		}
	}



	int getAdjacentWalls(Coordinate out[], UnionFind sets, s_coord currentParent) {
		int walls = 0;
		s_coord square = currentParent;
		do {
			for(int d = 0; d < DIRECTIONS_N; d++) {
				Coordinate nxt = Coordinate(Coordinate(square, SIZE), DIRECTIONS[d]);
				if(nxt.inBounds(COORDINATE_ZERO, SIZE)) {
					if(map[nxt.inty()][nxt.intx()] == WALL) {
						out[walls] = nxt;
						walls++;
					}
				}
			}
			square = sets.next(square);
		} while(square != currentParent);
		//for(int y = 0; y < HEIGHT; y++) {
		//	for(int x = 0; x < WIDTH; x++) {
		//		bool found = false;
		//		for(int i = 0; i < walls; i++) {
		//			if(out[i].equals(Coordinate(x,y))) {
		//				found = true;
		//				break;
		//			}
		//		}
		//		if(found) cout << 'X';
		//		else cout << map[y][x];
		//	}cout<<endl;
		//}cout<<"\n\n";
		return walls;
	}
	int getBorderWalls(Coordinate walls[], int directions[], int walls_n, UnionFind sets, s_coord parent, int distance) {
		int border_n = 0;
		for(int i = 0; i < walls_n; i++)
		{
			int rand_d = rand() % DIRECTIONS_N, d = 0;		//inizio a controllare da una direzione casuale per avere più casualità
			bool found = false;
			while(!found && d < DIRECTIONS_N) {
				Coordinate dir = DIRECTIONS[rand_d];									//direzione in cui si prova a "rompere" il muro per connettere
				Coordinate nxt = Coordinate(walls[i], dir.times(distance, distance));	//casella da controllare dopo aver rotto il muro

				if(nxt.inBounds(COORDINATE_ZERO, SIZE) && map[nxt.inty()][nxt.intx()] == FLOOR && sets.find(nxt.single_set(SIZE)) != parent) {
					cout<<walls[i].x<<"-"<<walls[i].y<<" "<<nxt.x<<"-"<<nxt.y<<endl;
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
		//for(int y = 0; y < HEIGHT; y++) {
		//	for(int x = 0; x < WIDTH; x++) {
		//		bool found = false;
		//		for(int i = 0; i < border_n; i++) {
		//			if(walls[i].equals(Coordinate(x,y))) {
		//				found = true;
		//				break;
		//			}
		//		}
		//		if(found) cout << 'X';
		//		else cout << map[y][x];
		//	}cout<<endl;
		//}cout<<"\n\n";
		return border_n;
	}
