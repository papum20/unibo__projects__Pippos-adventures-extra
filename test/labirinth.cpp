#include <iostream>
#include <ctime>
using namespace std;


#define MAP_WIDTH 30//6
#define MAP_HEIGHT 40//180//7
#define X_SCALE 9
#define EMPTY_C '.'
#define FLOOR_C ' '
#define WALL_C 'x'

char map[MAP_HEIGHT][MAP_WIDTH];
#define FINAL_MAP_WIDTH (MAP_WIDTH * X_SCALE - 2 * (X_SCALE - 1))
char final_map[MAP_HEIGHT][FINAL_MAP_WIDTH];



void init_map() {
	for(int y = 0; y < MAP_HEIGHT; y++) {
		for(int x = 0; x < MAP_WIDTH; x++) {
			if(x == 0 || x == MAP_WIDTH - 1 || y == 0 || y == MAP_HEIGHT - 1) map[y][x] = WALL_C;
			else map[y][x] = EMPTY_C;
		}
	}
}
void print_map(char A[MAP_HEIGHT][MAP_WIDTH]) {
	for(int y = 0; y < MAP_HEIGHT; y++) {
		for(int x = 0; x < MAP_WIDTH; x++) cout << A[y][x];
		cout << endl;
	}
}
void print_final_map() {
	for(int y = 0; y < MAP_HEIGHT; y++) {
		for(int x = 0; x < FINAL_MAP_WIDTH; x++) cout << final_map[y][x];
		cout << endl;
	}
}
void resizeMap() {
	for(int y = 0; y < MAP_HEIGHT; y++) {
		for(int x = 0; x < MAP_WIDTH; x++) {
			if(x == 0) final_map[y][x] = map[y][x];
			else if(x == MAP_WIDTH - 1) final_map[y][(x - 1) * X_SCALE + 1] = map[y][x];
			else {
				for(int i = 0; i < X_SCALE; i++)
					final_map[y][(x - 1) * X_SCALE + 1 + i] = map[y][x];
			}
		}
	}
}


/*
IDEE:
1 - fai percorsi ricorsivi e fermati a caso
2 - in ogni punto parti per un numero casuale di bivi (con un massimo?)
*/


#define STOP_CHANCE 4
#define DIRECTIONS_N 4
const int DIRECTIONS[DIRECTIONS_N][2] = {{0,-1}, {1,0}, {0,1}, {-1,0}};
/*
idea 1
*/
void generate_map1(int x, int y)
{
	if(map[y][x] != EMPTY_C) return;
	else if(rand() % STOP_CHANCE == 0) {
		map[y][x] = WALL_C;
	}
	else {
		for(int d = 0; d < DIRECTIONS_N; d++) {
			generate_map1(x + DIRECTIONS[d][0], y + DIRECTIONS[d][1]);
		}
	}
}



const int DIR_CHANCES[DIRECTIONS_N + 1] = {5, 20, 10, 3, 1};	//per ogni indice i, DIR_CHANCES[i] = probabilità di genereare i stanze
/*
idea 2;
ci deve sempre essere una direzione che non si interrompe mai
*/
void generate_map2(int x, int y, bool cant_stop = true) {
	if(x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return;
	else if(map[y][x] != EMPTY_C) return;
	else {
		map[y][x] = FLOOR_C;

//		cout << "XY: " << x << " " << y << endl;
//		print_map();

		int chance_tot = DIR_CHANCES[0];
		int tot_ind = 1;
		//conta direzioni usate (o fuori dalla mappa)
		bool used_dirs[DIRECTIONS_N];
		int used_dirs_n = 0;
		for(int i = 0; i < DIRECTIONS_N; i++) {
			int nx = x + DIRECTIONS[i][0], ny = y + DIRECTIONS[i][1];
			if(nx < 0 || nx >= MAP_WIDTH || ny < 0 || ny >= MAP_HEIGHT) {
				used_dirs[i] = true;
				used_dirs_n++;
			}
			else if(map[ny][nx] == EMPTY_C) {
				used_dirs[i] = false;
				chance_tot += DIR_CHANCES[tot_ind];
				tot_ind++;
			}
			else {
				used_dirs[i] = true;
				used_dirs_n++;
			}
		}

		if(used_dirs_n == DIRECTIONS_N) return;

//		cout << "used: ";
//		for(int i = 0; i < DIRECTIONS_N; i++) cout << used_dirs[i];
//		cout << endl;

		//scegli numero direzioni
		int new_dirs_r;	//random index for DIR_CHANCES
		if(cant_stop) new_dirs_r = rand() % (chance_tot - DIR_CHANCES[0]) + DIR_CHANCES[0];
		else new_dirs_r = rand() % (chance_tot);

		int new_dirs_n = 0, chance_count = 0;
		while(new_dirs_r >= chance_count + DIR_CHANCES[new_dirs_n]) {
			chance_count += DIR_CHANCES[new_dirs_n];
			new_dirs_n++;
		};
//		cout << "dirs num: " << new_dirs_r << " " << new_dirs_n << endl;

		if(new_dirs_n == 0) {
			for(int i = 0; i < DIRECTIONS_N; i++) {
				if(map[y + DIRECTIONS[i][1]][x + DIRECTIONS[i][0]] == EMPTY_C)
					map[y + DIRECTIONS[i][1]][x + DIRECTIONS[i][0]] = WALL_C;
			}
		}
		else
		{
			//scegli direzioni
			bool new_dirs[DIRECTIONS_N];
			for(int i = 0; i < DIRECTIONS_N; i++) new_dirs[i] = false;
			for(int i = 0; i < new_dirs_n; i++) {
				int r = rand() % (new_dirs_n - i);
//				cout << "r" << r << ";";
				int ind = 0;
				while(r > 0 || used_dirs[ind] == true || new_dirs[ind] == true) {
					if(used_dirs[ind] == false && new_dirs[ind] == false) r--;
					ind++;
				}
				new_dirs[ind] = true;
			}

	//		cout << "\nnew: ";
	//		for(int i = 0; i < DIRECTIONS_N; i++) cout << new_dirs[i];
	//		cout << endl;

			//scegli direzione con can_stop
			int cant_stop_d;
			if(!cant_stop) cant_stop_d = DIRECTIONS_N;
			else {
				cant_stop_d = 0;
				int r = rand() % new_dirs_n;
//				cout << "stop: " << r << ", ";
				while(r > 0 || new_dirs[cant_stop_d] == false) {
//					cout << "\\" << r << " " << cant_stop_d << "..";
					if(new_dirs[cant_stop_d] == true) r--;
					cant_stop_d++;
				}
//				cout << "--" << cant_stop_d << endl;
			}


			//ricorsione su direzioni
			for(int i = 0; i < DIRECTIONS_N; i++) {
//				cout << x + DIRECTIONS[i][0] << y + DIRECTIONS[i][1] << map[y + DIRECTIONS[i][1]][x + DIRECTIONS[i][0]] <<(map[y + DIRECTIONS[i][1]][x + DIRECTIONS[i][0]]==EMPTY_C) <<" ";
				if(new_dirs[i] == false && used_dirs[i] == false){
					//cout<<"CACCA";
					map[y + DIRECTIONS[i][1]][x + DIRECTIONS[i][0]] = WALL_C;
				}
			}
//			cout<<endl;
			//genera prima la direzione che non si ferma per evitare problemi (che parta prima un'altra direzione e poi vada a coprire
			//quella che non si ferma, interrompendo la generazione)
			for(int i = 0; i < DIRECTIONS_N; i++) {
				int ind = (cant_stop_d + i) % DIRECTIONS_N;
				if(new_dirs[ind] == true) {
					if(ind == cant_stop_d)
						generate_map2(x + DIRECTIONS[ind][0], y + DIRECTIONS[ind][1], true);
					else
						generate_map2(x + DIRECTIONS[ind][0], y + DIRECTIONS[ind][1], false);
				}
			}
		}

	}
}



/*
per generate_map3, per fare in modo che tutti i punti siano collegati, uso una struttura
union-find: (prima creo tutti i map_width*map_height insiemi), poi con delle chiamate
ricorsive unisco tutti i punti "adiacenti" (raggiungibili l'uno dall'altro); infine, finché
non ho un unico insieme, di volta in volta ne prendo uno, "rompo" un muro a caso in modo da
collegarmi a un altro insieme e unisco i due insiemi.
Il numero di union e find è lo stesso (nella prima fase sono tutti banali, con l'unione 
a insiemi di un elemento e nessun find; nella seconda si esegue prima un find e poi un union
per un numero di volte pari al numero di insiemi creatisi nella prima fase meno uno): scelgo
l'implementazione quick-union con euristica sul rango randomizzata, che utilizza un array
di dimensioni map_width*map_height, dove nel punto di indice y*map_width+x si trova
l'indice del padre, e per l'euristica sceglie casualmente un nodo da usare come nuovo padre
quando fa un union
*/
class CoordinateUnionFind {
	public:
		int parents[MAP_WIDTH * MAP_HEIGHT];
		int ranks[MAP_WIDTH * MAP_HEIGHT];		//se 0, non è un insieme
		int number;
	public:
		CoordinateUnionFind() {
			for(int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++) ranks[i] = 0;
			number = 0;
		}
		void makeSet(int x, int y) {
			int sc = singleCoordinate(x, y);
			if(ranks[sc] == 0) {
				parents[sc] = sc;
				ranks[sc] = 1;
				number++;
			}
		}
		int find(int x, int y) {
			int sc = singleCoordinate(x, y);
			if(ranks[sc] == 0) return -1;
			else {
				while(parents[sc]!= sc) sc = parents[sc];
				return sc;
			}
		}
		void quickUnion(int x1, int y1, int x2, int y2) {
			int parent1 = find(x1, y1), parent2 = find(x2, y2);
			if(parent1 != parent2 || parent1 == -1) {
				//cout <<"CACCA " << parent1 << " " <<parent2 <<"\n";
				if(parent1 == -1) {
					makeSet(x1, y1);
					parent1 = singleCoordinate(x1, y1);
				}
				if(parent2 == -1) {
					makeSet(x2, y2);
					parent2 = singleCoordinate(x2, y2);
				}

				if(ranks[parent1] < ranks[parent2]) {
					parents[parent1] = parent2;
					ranks[parent2] = max(ranks[parent2], 1 + ranks[parent1]);
				}
				else {
					parents[parent2] = parent1;
					ranks[parent1] = max(ranks[parent1], 1 + ranks[parent2]);
				}
				//cout <<"CACCA " << parent1 << " " <<parent2 <<" ";
				number--;
			}
		}
		//AUSILIARIE / SECONDARIE
		int singleCoordinate(int x, int y) {
			return y * MAP_WIDTH + x;
		}
		void doubleCoordinate(int xy, int &x, int &y) {
			y = xy / MAP_WIDTH;
			x = xy - y * MAP_WIDTH;
		}
		//GET
		int getNumber() {
			return number;
		}
		void getNth(int n, int &x, int &y) {
			if(n >= 0 && number >= n) {
				int i = 0;
				while(n > 0) {
					if(ranks[i] != 0 && parents[i] == i) n--;
					if(n != 0) i++;
				}
				doubleCoordinate(i, x, y);
			}
		}
		/*int getSet(const int x, const int y, int out[MAP_WIDTH * MAP_HEIGHT]) {
			int parent = find(x, y);
			int len = 0;
			for(int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++) {
				if(ranks[i] != 0 && parents[i] == parent) {
					out[len] == i;
					len++;
				}
			}
			return len;
		}*/
		//DEBUG
		void printSets() {
			for(int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++) {
				if(ranks[i] != 0 && parents[i] == i) {
					cout << i << ":\t";
					for(int j = 0; j < MAP_WIDTH * MAP_HEIGHT; j++) {
						int x, y;
						doubleCoordinate(j, x, y);
						if(ranks[j] != 0 && find(x, y) == i) {
							cout << "(" << x << "," << y << ") ";
						}
					}
					cout << endl;
				}
			}
			cout << endl;
		}
};


void unionAdjacent(CoordinateUnionFind *sets, bool used[MAP_HEIGHT][MAP_WIDTH], int x, int y) {
	if(!used[y][x]) {
		used[y][x] = true;
		for(int d = 0; d < DIRECTIONS_N; d++) {
			int nx = x + DIRECTIONS[d][0], ny = y + DIRECTIONS[d][1];
			if(nx >= 0 && nx < MAP_WIDTH && ny >= 0 && ny < MAP_HEIGHT && map[ny][nx] == FLOOR_C) {
				sets->quickUnion(x, y, nx, ny);
				//cout<<"N"<<sets->getNumber()<< " (" << x << "," << y << ") (" << nx << ","<<ny<<")" <<endl;
				unionAdjacent(sets, used, nx, ny);
			}
		}
	}
}

/*
generates map so that the points in parameters are part of the floor and reachable;
it applies generate_map2 until the whole map is covered either by walls or floor,
and it does such that each point (of the floor) can be reached from every other point
*/
void generate_map3(CoordinateUnionFind *sets, int constX[], int constY[], int len) {
	bool used[MAP_HEIGHT][MAP_WIDTH];
	for(int y = 0; y < MAP_HEIGHT; y++)
		for(int x = 0; x < MAP_WIDTH; x++) used[y][x] = false;
	for(int i = 0; i < len; i++) {
		map[constY[i]][constX[i]] = FLOOR_C;
		sets->makeSet(constX[i], constY[i]);
		/*bool by1 = y[i] == 0, by2 = y[i] == MAP_HEIGHT - 1, bx1 = x[i] == 0, bx2 = x[i] == MAP_WIDTH - 1;
		if(by1 || by2) {
			if(by1) map[1][x[i]] = FLOOR_C;
			else map[MAP_HEIGHT - 2][x[i]] = FLOOR_C;
		}
		if(bx1 || bx2) {
			if(bx1) map[y[i]][1] = FLOOR_C;
			else map[y[i]][MAP_WIDTH - 2] = FLOOR_C;
		}
		if((by1 || by2) && (bx1 || bx2)) {
			if(by1 && bx1) map[1][1] = FLOOR_C;
			else if(by1 && bx2)  map[1][MAP_WIDTH - 2] = FLOOR_C;
			else if(by2 && bx1)  map[MAP_HEIGHT - 2][1] = FLOOR_C;
			else  map[MAP_HEIGHT - 2][MAP_WIDTH - 2] = FLOOR_C;
		}*/
	}
	int randX = rand() % MAP_WIDTH, randY = rand() % MAP_HEIGHT;
	for(int y = 0; y < MAP_HEIGHT; y++) {
		for(int x = 0; x < MAP_WIDTH; x++) {
			int rx = (randX + x) % MAP_WIDTH, ry = (randY + y) % MAP_HEIGHT;
			if(map[ry][rx] == EMPTY_C) {
				sets->makeSet(rx, ry);
				generate_map2(rx, ry);
				unionAdjacent(sets, used, rx, ry);
			//cout << "n"<<sets->getNumber()<<endl;;
	//sets->printSets();
				//print_map(map);
			}
		}
	}
}



void printSetsMap(CoordinateUnionFind *sets) {
	char t_map[MAP_HEIGHT][MAP_WIDTH];
	int sets_n = sets->getNumber();
	char color = 'A';
	for(int s = 1; s <= sets_n; s++) {
		int setX, setY;
		sets->getNth(s, setX, setY);
		int setXY = sets->singleCoordinate(setX, setY);

		if(s == 26) color = 'a';
		if(s == 52) color = '0';
		for(int y = 0; y < MAP_HEIGHT; y++) {
			for(int x = 0; x < MAP_WIDTH; x++) {
				if(sets->find(x, y) == setXY) t_map[y][x] = color + s - 1;
				else if(map[y][x] == WALL_C) t_map[y][x] = WALL_C;
			}
		}
	}
	//print_map(t_map);
}

void printSetMap(CoordinateUnionFind *sets, int x, int y) {
	char t_map[MAP_HEIGHT][MAP_WIDTH];
	int parent = sets->find(x, y);
	char color = 'A';
	for(int y = 0; y < MAP_HEIGHT; y++) {
		for(int x = 0; x < MAP_WIDTH; x++) {
			if(sets->find(x, y) == parent) t_map[y][x] = color;
			else if(map[y][x] == WALL_C) t_map[y][x] = WALL_C;
			else t_map[y][x] = FLOOR_C;
		}
	}
	//print_map(t_map);
}



bool isSetAdjacentWall(CoordinateUnionFind *sets, int parent, int wallX, int wallY, int distance) {	//start - wall = distance
	bool isAdjacent = false;
	for(int d_self = 0; d_self < DIRECTIONS_N; d_self++)
	{
		int self_x = wallX + DIRECTIONS[d_self][0], self_y = wallY + DIRECTIONS[d_self][1];
		if(self_x >= 0 && self_x < MAP_WIDTH && self_y >= 0 && self_y < MAP_HEIGHT && sets->find(self_x, self_y) == parent) {
			for(int d_other = 0; d_other < DIRECTIONS_N; d_other++) {
				int other_x = wallX + distance * (wallX - self_x) + DIRECTIONS[d_other][0], other_y = wallY + distance * (wallY - self_y) + DIRECTIONS[d_other][1];
				if(other_x >= 0 && other_x < MAP_WIDTH && other_y >= 0 && other_y < MAP_HEIGHT)
					if(sets->find(other_x, other_y) != parent && sets->find(other_x, other_y) != -1) isAdjacent = true;
			}
		}
	}
//		printf("\t(%d,%d),(%d,%d),(%d,%d)", wallX, wallY, self_x, self_y, other_x, other_y);
	return isAdjacent;
}

int countBorderWalls(CoordinateUnionFind *sets, int walls[], int setX, int setY, int distance) {
	int set = sets->find(setX, setY);
	int len = 0;
	for(int my = 0; my < MAP_HEIGHT; my++) {
		for(int mx = 0; mx < MAP_WIDTH; mx++) {
			//printf("(%d,%d): %d\n", mx, my, isSetAdjacentWall(sets, set, mx, my, distance));
			if(map[my][mx] == WALL_C && isSetAdjacentWall(sets, set, mx, my, distance)) {
				walls[len] = sets->singleCoordinate(mx, my);
				len++;
			}
		}
	}
	return len;
}

void connectMap(CoordinateUnionFind *sets) {
	int adj_walls[MAP_HEIGHT * MAP_WIDTH];
	int adj_walls_n = 0;

	while(sets->getNumber() > 1) {
		bool hasConnected = false;
		int distance = 0;
		while(!hasConnected)
		{
			int setX, setY;
			sets->getNth(1, setX, setY);
			//printf("%d,%d\n\n", setX, setY);
			adj_walls_n = countBorderWalls(sets, adj_walls, setX, setY, distance);

			if(adj_walls_n != 0) {
				//cout << "distance: " << distance << " " << adj_walls_n << endl;

				int brokenWall = rand() % adj_walls_n;
				int breakX, breakY;
				sets->doubleCoordinate(adj_walls[brokenWall], breakX, breakY);
				map[breakY][breakX] = FLOOR_C;
				sets->quickUnion(setX, setY, breakX, breakY);

				for(int d = 0; d < DIRECTIONS_N; d++) {
					int dx = breakX + DIRECTIONS[d][0], dy = breakY + DIRECTIONS[d][1];
					if(dx >= 0 && dx < MAP_WIDTH && dy >= 0 && dy < MAP_HEIGHT) {
						if(map[dy][dx] == FLOOR_C && sets->find(dx, dy) != sets->singleCoordinate(setX, setY)) {
							sets->quickUnion(setX, setY, dx, dy);
							hasConnected = true;
						}
					}
				}
				distance = 0;
				//printf("%d, %d\n", breakX, breakY);
				//printSetMap(sets, setX, setY);
				//cout << endl;
			}
			else distance++;
		}
	}
}
/*
void connectMap(CoordinateUnionFind *sets) {
	int adj_walls[MAP_HEIGHT * MAP_WIDTH];
	int adj_walls_n = 0;

	while(sets->getNumber() > 1) {
		int setX, setY;
		sets->getNth(1, setX, setY);
		adj_walls_n = countBorderWalls(sets, adj_walls, setX, setY, );

		int brokenWall = rand() % adj_walls_n;
		int breakX, breakY;
		sets->doubleCoordinate(adj_walls[brokenWall], breakX, breakY);
		map[breakY][breakX] = FLOOR_C;

		sets->quickUnion(setX, setY, breakX, breakY);
		for(int d = 0; d < DIRECTIONS_N; d++) {
			int dx = breakX + DIRECTIONS[d][0], dy = breakY + DIRECTIONS[d][1];
			if(map[dy][dx] == FLOOR_C && sets->find(dx, dy) != sets->singleCoordinate(setX, setY))
				sets->quickUnion(setX, setY, dx, dy);
		}
		printf("%d, %d\n", breakX, breakY);
		printSetMap(sets, setX, setY);
		cout << endl;
	}
}
*/



int main() {

	srand(time(NULL));
	freopen("labirinth2.txt", "w", stdout);

	init_map();
	//generate_map2(1, 1, true);
	CoordinateUnionFind *sets = new CoordinateUnionFind();

	/*
	for(int y = 0; y < MAP_HEIGHT; y++)
		for(int x = 0; x < MAP_WIDTH; x++) map[y][x] = WALL_C;
	map[2][3] = FLOOR_C;
	map[3][1] = FLOOR_C;
	*/

	bool used[MAP_HEIGHT][MAP_WIDTH];
	for(int y = 0; y < MAP_HEIGHT; y++)
		for(int x = 0; x < MAP_WIDTH; x++) used[y][x] = false;

	int len = 3;
	int X[len] = {0, MAP_WIDTH / 2, MAP_WIDTH - 1};
	int Y[len] = {MAP_HEIGHT / 2, MAP_HEIGHT - 1, MAP_HEIGHT / 2};
	generate_map3(sets, X, Y, len);
	print_map(map);
	printSetsMap(sets);

	connectMap(sets);
	print_map(map);
	printSetsMap(sets);


	resizeMap();
	print_final_map();



	/*
	map[1] = "xxxxxx";
	map[2] = "xxx xx";
	map[3] = "x xxxx";
	map[4] = "xxxxxx";
	map[5] = "xxxxxx";
	*/
/*
	print_map(map);
	cout << endl;
	for(int y = 0; y < MAP_HEIGHT; y++) {
		for(int x = 0; x < MAP_WIDTH; x++) {
			if(map[y][x] == FLOOR_C) {
				sets->makeSet(x, y);
				unionAdjacent(sets, used, x, y);
			//cout << "n"<<sets->getNumber()<<endl;;
				sets->printSets();
				cout << endl;
				print_map(map);
				cout << endl;
			}
		}
	}

	printSetsMap(sets);
	cout << "CIAO"<<endl<<endl;
	
	connectMap(sets);

	printSetsMap(sets);*/
/*
	for(int y = 0; y < MAP_HEIGHT; y++) {
		for(int x = 0; x < MAP_WIDTH; x++) {
			if(map[y][x] == FLOOR_C) {
				unionAdjacent(sets, used, x, y);
				printSetsMap(sets);
				cout<<endl;
			}
		}
	}*/
	
}



/*
PROP.: partendo da un qualsiasi punto di un set confinante con un muro che porta verso un altro set (e tale che il muro
non sia laterale, cioè sui bordi della mappa), ci si può sempre collegare all'altro set distruggendo massimo due muri
(cioè, una volta distrutto tale muro, o ci si collega direttamente, o c'è sempre almeno un muro che se distrutto collega)
*/