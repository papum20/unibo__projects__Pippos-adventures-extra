#include <iostream>
#include <ctime>
#include <cmath>
using namespace std;


#define N_ROOMS 10
#define N_ARR (N_ROOMS*2+1)
#define MAX_CONNECTED_ROOMS 5
#define MAX_SIDES 4

//// GRID
	char grid[N_ARR][N_ARR];

	void initGrid() {
		for(int y = 0; y < N_ARR; y++) 
			for(int x = 0; x < N_ARR; x++) grid[y][x] = '.';
	}
	void printGrid() {
		for(int y = 0; y < N_ARR; y++) {
			for(int x = 0; x < N_ARR; x++) cout << grid[y][x];
			cout << endl;
		}
	}

//// ROOM
	typedef struct Room {
		int id;
		int n_doors_max;
		int n_doors_sides;
		int x;
		int y;
		Room *connected[MAX_CONNECTED_ROOMS];
	} Room_t;
	typedef Room_t *pRoom;
	pRoom rooms[N_ROOMS];

	pRoom initRoom(int id, int x = 0, int y = 0) {
		pRoom tmp = new Room_t;
		for(int i = 0; i < MAX_CONNECTED_ROOMS; i++) tmp->connected[i] = NULL;
		tmp->n_doors_max = MAX_CONNECTED_ROOMS;
		tmp->n_doors_sides = 0;
		tmp->x = x;
		tmp->y = y;
		tmp->id = id;
		return tmp;
	}
	pRoom findRoom(int l, int x, int y) {
		for(int i = 0; i < l; i++)
			if(rooms[i]->x == x && rooms[i]->y == y) return rooms[i];
		return NULL;
	}
//// AVAIL
	int avail[N_ROOMS * 2 + 2][3];
	int n_av = 0;
	
	int findAvail(int x, int y) {
		for(int i = 0; i < n_av; i++)
			if(avail[i][0] == x && avail[i][1] == y) return i;
		return -1;
	}
	void switchQueue(int a, int b) {
		int tmp[3];
		for(int i = 0; i < 3; i++) tmp[i] = avail[a][i];
		for(int i = 0; i < 3; i++) avail[a][i] = avail[b][i];
		for(int i = 0; i < 3; i++) avail[b][i] = tmp[i];
	}
	void checkQueue(int i, int len) {
		//controlla su
		while(i > 0 && avail[i][2] < avail[(int)floor((i-1)/2)][2]) {
			int t = floor((i-1)/2);
			switchQueue(i, t);
			i = t;
		}
		//controlla giu
		while(i*2+1 < len && (avail[i][2] > avail[i*2+1][2] || (i*2+2 < len && avail[i][2] > avail[i*2+2][2])) ) {
			int t;
			if(i*2+2 >= len || avail[i*2+1][2] < avail[i*2+2][2]) t = i*2+1;
			else t = i*2+2;
			switchQueue(i, t);
			i = t;
		}
	}


//// ALGORITMO

int dir[4][2] = {{0,-1},{1,0},{0,1},{-1,0}};	//directions
/* 8 scritto "bene"
*/
/*COSTO: tempo=O(n**2), memoria=O(n+(2n+2))=O(n); con n=N_ROOMS
APPUNTO1: con rooms e avail implementati con AVL (forse) si avrebbe un costo logartmico
	per ricerca, inserimento... e quindi costo totale (n log n), con n=N_ROOMS
APPUNTO2: si possono togliere gli attributi x,y della classe e calcolarli ogni volta
	(presumibilmente senza variazione di costo in tempo)
*/
void generateMap9()
{
	//inizializza rooms a NULL, tranne prima cella inizializzata
	for(int i = 1; i < N_ROOMS; i++) rooms[i] = NULL;
	rooms[0] = initRoom(0, 0, 0);
	grid[N_ROOMS][N_ROOMS] = '0';	//scrivi la prima stanza nella griglia
	//aggiungi stanze adiacenti ad avail
	int r = rand() % 4;
	for(int i = 0; i < 4; i++) {
		avail[i][0] = dir[(r+i)%4][0];
		avail[i][1] = dir[(r+i)%4][1];
		avail[i][2] = 1;
	}
	n_av = 4;
	//genera stanze
	for(int i = 1; i < N_ROOMS; i++)
	{
		//scegli la nuova posizione
		int randRoom = 0;
		int executions = 3;
		do {	//verrebbe comunque eseguito almeno una volta anche con il while
			if(rand() % 2 == 0) executions = 0;
			else {
				if(randRoom*2 + 1 < n_av && rand() % 2) randRoom = randRoom*2 + 1;
				else if(randRoom*2 + 2 < n_av) randRoom = randRoom*2 + 2;
				else {
					randRoom = 0;
					executions--;
				}
			}
		} while(executions > 0);
		/*int randRoom = 0;												//next room position
		while(randRoom*2 + 1 < n_av) {
			if(rand() % 2 == 0) break;
			else {
				if(rand() % 2 == 0) randRoom = randRoom*2 + 1;
				else if(randRoom*2 + 2 < n_av) randRoom = randRoom*2 + 2;
				else randRoom = 0;
			}
		}*/
		int cx = avail[randRoom][0], cy = avail[randRoom][1];			//current (new room) x,y
		rooms[i] = initRoom(i, cx, cy);

		bool first = true;
		r = rand() % 4;
		for(int j = 0; j < 4; j++)
		{
			int cdir = (r+j)%4;											//current direction
			int nx = cx + dir[cdir][0], ny = cy + dir[cdir][1];			//next x,y (for adjacent rooms)
			pRoom adjacent = findRoom(i, nx, ny);
			int ind_avail = findAvail(nx, ny);
			if(adjacent != NULL) {
				adjacent->connected[(cdir+2)%4] = rooms[i];
				rooms[i]->connected[cdir] = adjacent;
				adjacent->n_doors_sides++;
				rooms[i]->n_doors_sides++;
			}
			else if(ind_avail != -1) {
				avail[ind_avail][2]++;
				checkQueue(ind_avail, n_av);
			}
			else {
				int ind;
				if(first) {
					ind = randRoom;
					first = false;
				}
				else ind = n_av++;
				
				avail[ind][0] = nx;
				avail[ind][1] = ny;
				avail[ind][2] = 1;
				checkQueue(ind, n_av);
			}
		}
		if(first) {
			avail[randRoom][0] = avail[n_av-1][0];
			avail[randRoom][1] = avail[n_av-1][1];
			avail[randRoom][2] = avail[n_av-1][2];
			n_av--;
			checkQueue(randRoom, n_av);
		}

		grid[N_ROOMS + rooms[i]->y][N_ROOMS + rooms[i]->x] = (char)(48+i);
	}
}









//// DEBUG

	int mem[5];
	void countGrid() {
		int num[4] = {0,0,0,0};
		int tot = 0;
		for(int i = 0; i < N_ROOMS; i++) {
			num[rooms[i]->n_doors_sides-1]++;
			tot += rooms[i]->n_doors_sides;
		}
		//for(int i = 0; i < 4; i++) cout << i+1 << ":" << num[i] << " ";
		//cout << ";" << tot << endl;
		mem[0] += tot;
		for(int i = 0; i < 4; i++) mem[i+1] += num[i];
	}
	int dim[2];
	void countDimensions() {
		int xmin = N_ROOMS, xmax = -N_ROOMS, ymin = N_ROOMS, ymax = -N_ROOMS;
		for(int i = 0; i < N_ROOMS; i++) {
			xmin = min(xmin, rooms[i]->x);
			xmax = max(xmax, rooms[i]->x);
			ymin = min(ymin, rooms[i]->y);
			ymax = max(ymax, rooms[i]->y);
		}
		dim[0] += xmax - xmin;
		dim[1] += ymax - ymin;
	}



int main() {

	srand(time(NULL));
/*
	initGrid();
	generateMap9();
	printGrid();
	countGrid();
*/
/*
	for(int i = 0; i < 100; i++) {
		initGrid();
		generateMap4();
		printGrid();
		countGrid();
		cout << endl << i+1;
		for(int j = 0; j < 20; j++) cout << "=";
		cout << endl;
		cout << endl;
	}
	*/

	freopen("alg91.txt", "w", stdout);
	for(int i = 0; i < 5; i++) mem[i] = 0;
	for(int i = 0; i < 100; i++) {
		initGrid();
		generateMap9();
		printGrid();
		countGrid();
		cout << endl << i+1;
		for(int j = 0; j < 20; j++) cout << "=";
		cout << endl;
		cout << endl;
	}
	for(int i = 0; i < 5; i++) cout << i << ":" << mem[i]/100. << " ";


	const double ATT = 100000;
	cout << "1: ";
	for(int i = 0; i < 5; i++) mem[i] = 0;
	for(int i = 0; i < 2; i++) dim[i] = 0;
	for(int i = 0; i < ATT; i++) {
		initGrid();
		generateMap9();
		countGrid();
		countDimensions();
	}
	for(int i = 0; i < 5; i++) cout << i << ":" << mem[i]/ATT << " ";
	cout << endl;
	for(int i = 0; i < 2; i++) cout << dim[i]/ATT << " ";
	cout << endl;


}