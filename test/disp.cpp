#include <iostream>
#include <ctime>
#include <cmath>
using namespace std;


#define N_ROOMS 20
#define N_ARR (N_ROOMS*2+1)
#define MAX_CONNECTED_ROOMS 5
#define MAX_SIDES 4
char grid[N_ARR][N_ARR];



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


pRoom initRoom(int id) {
	pRoom tmp = new Room_t;
	for(int i = 0; i < MAX_CONNECTED_ROOMS; i++) tmp->connected[i] = NULL;
	tmp->n_doors_max = MAX_CONNECTED_ROOMS;
	tmp->n_doors_sides = 0;
	tmp->x = 0;
	tmp->y = 0;
	tmp->id = id;
	return tmp;
}
pRoom addDoor(pRoom pr, int n, int &x, int &y, int id) {
	int m = n;
	int i = 0;
	while(m > 0 || pr->connected[i] != NULL) {
		if(pr->connected[i] == NULL) m--;
		i++;
	}
	pRoom res = initRoom(id);
	if(i == 0) y--;
	else if(i == 1) x++;
	else if(i == 2) y++;
	else x--;
	res->x = x;
	res->y = y;
	//pRoom tmp = initRoom();
	//pr->connected[i] = tmp;
	//tmp->connected[(n+2)%4] = pr;
	return res;
}
pRoom addDoor2(pRoom pr, int n, int &x, int &y, int id) {
	pRoom res = initRoom(id);
	if(n == 0) y--;
	else if(n == 1) x++;
	else if(n == 2) y++;
	else x--;
	res->x = x;
	res->y = y;
	//pRoom tmp = initRoom();
	//pr->connected[i] = tmp;
	//tmp->connected[(n+2)%4] = pr;
	return res;
}
void checkConnected(pRoom pr, int n) {
	for(int i = 0; i <= n; i++) {
		if((abs(pr->x - rooms[i]->x) == 1 && abs(pr->y - rooms[i]->y) == 0) ||
			(abs(pr->x - rooms[i]->x) == 0 && abs(pr->y - rooms[i]->y) == 1)) {
				if(pr->x < rooms[i]->x) {
					pr->connected[1] = rooms[i];
					rooms[i]->connected[3] = pr;
					pr->n_doors_sides++;
					rooms[i]->n_doors_sides++;
				}
				else if(pr-> x > rooms[i]->x) {
					pr->connected[3] = rooms[i];
					rooms[i]->connected[1] = pr;
					pr->n_doors_sides++;
					rooms[i]->n_doors_sides++;
				}
				else if(pr->y > rooms[i]->y) {
					pr->connected[0] = rooms[i];
					rooms[i]->connected[2] = pr;
					pr->n_doors_sides++;
					rooms[i]->n_doors_sides++;
				}
				else {
					pr->connected[2] = rooms[i];
					rooms[i]->connected[0] = pr;
					pr->n_doors_sides++;
					rooms[i]->n_doors_sides++;
				}
		}
	}
}

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

void debugGrid(int i) {
	cout << endl;
	for(int j = 0; j <= i; j++) cout << rooms[j]->id << " ";
	cout << endl;
	for(int j = 0; j <= i; j++) {
		pRoom r = rooms[j];
		cout << r->n_doors_max << " " << r->n_doors_sides << ";" << r->x << " " << r->y << ":\t";
		for(int k = 0; k < MAX_CONNECTED_ROOMS; k++) {
			if(r->connected[k] == NULL) cout << ".,./";
			else cout << r->connected[k]->x << "," << r->connected[k]->y << "/";
		}
		cout << endl;
	}
	cout << endl;
	printGrid();
	cout << endl;
}

/* algoritmo banale, che sceglie una stanza tra quelle già presenti,
da questa sceglie una direzione ancora disponibile e lì crea la nuova stanza
*/
void generateMap1()
{
	int x = 0, y = 0;
	for(int i = 1; i < N_ROOMS; i++) rooms[i] = NULL;
	rooms[0] = initRoom(0);
	grid[N_ROOMS][N_ROOMS] = 'a';
	//debugGrid(0);
	//genera stanze
	for(int i = 1; i < N_ROOMS; i++)
	{
		int randStart;			//stanza a cui collegare quella che verrà generata
		do {
			randStart = rand() % i;
		} while(rooms[randStart]->n_doors_sides == MAX_SIDES);
		int randRoom = rand() % (MAX_SIDES - rooms[randStart]->n_doors_sides);	//direzione in cui aggiungere stanza (tra quelle ancora disponibili)
		
		x = rooms[randStart]->x;
		y = rooms[randStart]->y;
		rooms[i] = addDoor(rooms[randStart], randRoom, x, y, i);
		checkConnected(rooms[i], i);
		grid[N_ROOMS + rooms[i]->y][N_ROOMS + rooms[i]->x] = (char)(97+i);
		
		//cout << randStart << " " << randRoom << endl;
		//debugGrid(i);
	}
}



int roomCompare(pRoom a, pRoom b) {	//return a-b
	if(a->n_doors_sides > b->n_doors_sides) return 1;
	else if(a->n_doors_sides < b->n_doors_sides) return -1;
	else return 0;
}

int insertQueue(pRoom r, int len) {
	rooms[len] = r;
	int i = len;
	while(i > 0 && roomCompare(r, rooms[(int)floor((i-1)/2)]) < 0) {
		rooms[i] = rooms[(int)floor((i-1)/2)];
		rooms[(int)floor((i-1)/2)] = r;
		i = floor((i-1)/2);
	}
	return i;
}

/* algoritmo basato su min-heap, che con probabilità 1/2 sceglie
il nodo corrente, altrimenti ha una probabilità di 1/2 per ognuno dei nodi
figli; quando si ferma a un nodo sceglie una direzione tra quelle disponibili
(e crea la stanza come nel primo algoritmo)
*/
void generateMap2()
{
	int x = 0, y = 0;
	for(int i = 1; i < N_ROOMS; i++) rooms[i] = NULL;
	rooms[0] = initRoom(0);
	grid[N_ROOMS][N_ROOMS] = 'a';
	//debugGrid(0);
	//genera stanze
	for(int i = 1; i < N_ROOMS; i++)
	{
		int randStart;
		do {
			randStart = 0;
			while(randStart*2 + 1 < i) {
				int k = rand() % 2;
				if(k == 0) break;
				else {
					int j = rand() % 2;
					randStart = min(randStart*2 + 1 + j, i-1);
				}
			}
		} while(rooms[randStart]->n_doors_sides == MAX_SIDES);
		int randRoom = rand() % (MAX_SIDES - rooms[randStart]->n_doors_sides);	//direzione in cui aggiungere stanza (tra quelle ancora disponibili)
		
		
		x = rooms[randStart]->x;
		y = rooms[randStart]->y;
		int ni = insertQueue(addDoor(rooms[randStart], randRoom, x, y, i), i);
		checkConnected(rooms[ni], i);
		grid[N_ROOMS + rooms[ni]->y][N_ROOMS + rooms[ni]->x] = (char)(97+i);
		
		//cout << randStart << " " << randRoom << endl;
		//debugGrid(i);
	}
}


/* algoritmmo simile al 2, che però usa uguale probabilità di 1/3
per nodo corrente e figli */
void generateMap3()
{
	int x = 0, y = 0;
	for(int i = 1; i < N_ROOMS; i++) rooms[i] = NULL;
	rooms[0] = initRoom(0);
	grid[N_ROOMS][N_ROOMS] = 'a';
	//debugGrid(0);
	//genera stanze
	for(int i = 1; i < N_ROOMS; i++)
	{
		int randStart;
		do {
			randStart = 0;
			while(randStart*2 + 1 < i) {
				int k = rand() % 3;
				if(k == 0) break;
				else randStart = min(randStart*2 + k, i-1);
			}
		} while(rooms[randStart]->n_doors_sides == MAX_SIDES);
		int randRoom = rand() % (MAX_SIDES - rooms[randStart]->n_doors_sides);	//direzione in cui aggiungere stanza (tra quelle ancora disponibili)
		
		
		x = rooms[randStart]->x;
		y = rooms[randStart]->y;
		int ni = insertQueue(addDoor(rooms[randStart], randRoom, x, y, i), i);
		checkConnected(rooms[ni], i);
		grid[N_ROOMS + rooms[ni]->y][N_ROOMS + rooms[ni]->x] = (char)(97+i);
		
		//cout << randStart << " " << randRoom << endl;		
		//debugGrid(i);
	}
}


/* algortitmo basato su min-heap, che, finché non trova una nuova posizione disponibile,
ripete questi passaggi: cerca una stanza di partenza con probabilità 1/3 (come nel 3),
quando la trova sceglie una direzione qualsiasi (anche tra quelle non disponibili)
*/
void generateMap4()
{
	int x = 0, y = 0;
	for(int i = 1; i < N_ROOMS; i++) rooms[i] = NULL;
	rooms[0] = initRoom(0);
	grid[N_ROOMS][N_ROOMS] = 'a';
	//debugGrid(0);
	//genera stanze
	for(int i = 1; i < N_ROOMS; i++)
	{
		int randStart, randRoom;
		do {
			randStart = 0;
			while(randStart*2 + 1 < i) {
				int k = rand() % 3;
				if(k == 0) break;
				else randStart = min(randStart*2 + k, i-1);
			}
			randRoom = rand() % MAX_SIDES;
		} while((rooms[randStart]->connected)[randRoom] != NULL);
		
		
		x = rooms[randStart]->x;
		y = rooms[randStart]->y;
		int ni = insertQueue(addDoor2(rooms[randStart], randRoom, x, y, i), i);
		checkConnected(rooms[ni], i);
		grid[N_ROOMS + rooms[ni]->y][N_ROOMS + rooms[ni]->x] = (char)(97+i);
		
		//cout << randStart << " " << randRoom << endl;	
		//debugGrid(i);
	}
}

int available[N_ROOMS * 2 + 2][2];
int n_av = 0;

pRoom addDoor3(int x, int y, int id) {
	pRoom res = initRoom(id);
	res->x = x;
	res->y = y;
	return res;
}

pRoom findRoom(int l, int x, int y) {
	for(int i = 0; i < l; i++)
		if(rooms[i]->x == x && rooms[i]->y == y) return rooms[i];
	return NULL;
}
bool findAvailable(int x, int y) {
	for(int i = 0; i < n_av; i++)
		if(available[i][0] == x && available[i][1] == y) return true;
	return false;
}

/* algoritmo che tiene traccia delle possibili nuove disposizioni per le stanze,
ovvero gli spazi vicini a stanze già presenti, e ne sceglie una casuale tra esse
*/
void generateMap5()
{
	int x = 0, y = 0;
	for(int i = 1; i < N_ROOMS; i++) rooms[i] = NULL;
	rooms[0] = initRoom(0);
	grid[N_ROOMS][N_ROOMS] = 'a';
	available[0][0] = 0;
	available[0][1] = -1;
	available[1][0] = 1;
	available[1][1] = 0;
	available[2][0] = 0;
	available[2][1] = 1;
	available[3][0] = -1;
	available[3][1] = 0;
	n_av = 4;
	//debugGrid(0);
	//genera stanze
	for(int i = 1; i < N_ROOMS; i++)
	{
		//cout << n_av<<endl;
		int randRoom = rand() % n_av;
		rooms[i] = addDoor3(available[randRoom][0], available[randRoom][1], i);

		bool first = true;
		int es[4][2] = {{0,-1},{1,0},{0,1},{-1,0}};
		int cx = available[randRoom][0], cy = available[randRoom][1];
		for(int j = 0; j < 4; j++) {
			int nx = cx + es[j][0], ny = cy + es[j][1];
			pRoom pr = findRoom(i, nx, ny);
			//if(pr == NULL) cout << "NULL";
			//else cout << "--" << pr->id;
			if(pr != NULL) {
				pr->connected[(j+2)%4] = rooms[i];
				rooms[i]->connected[j] = pr;
				pr->n_doors_sides++;
				rooms[i]->n_doors_sides++;
			}
			else if(findAvailable(nx, ny)) {
				continue;
			}
			else if(first) {
				available[randRoom][0] = nx;
				available[randRoom][1] = ny;
				first = false;
			}
			else {
				available[n_av][0] = nx;
				available[n_av][1] = ny;
				n_av++;
			}
		}
		if(first) {
			available[randRoom][0] = available[n_av-1][0];
			available[randRoom][1] = available[n_av-1][1];
			n_av--;
		}

		grid[N_ROOMS + rooms[i]->y][N_ROOMS + rooms[i]->x] = (char)(97+i);

		//debugGrid(i);
		//for(int i = 0; i < n_av; i++) cout << available[i][0] << "," << available[i][1] << " ";
		//cout << "\n\n\n";
	}
}

/* come il 4, ma con probabilità come nel 2 */
void generateMap6()
{
	int x = 0, y = 0;
	for(int i = 1; i < N_ROOMS; i++) rooms[i] = NULL;
	rooms[0] = initRoom(0);
	grid[N_ROOMS][N_ROOMS] = 'a';
	//debugGrid(0);
	//genera stanze
	for(int i = 1; i < N_ROOMS; i++)
	{
		int randStart, randRoom;
		do {
			randStart = 0;
			while(randStart*2 + 1 < i) {
				if(rand() % 2) break;
				else {
					int k = rand() % 2;
					randStart = min(randStart*2 + k + 1, i-1);
				}
			}
			randRoom = rand() % MAX_SIDES;
		} while((rooms[randStart]->connected)[randRoom] != NULL);
		
		
		x = rooms[randStart]->x;
		y = rooms[randStart]->y;
		int ni = insertQueue(addDoor2(rooms[randStart], randRoom, x, y, i), i);
		checkConnected(rooms[ni], i);
		grid[N_ROOMS + rooms[ni]->y][N_ROOMS + rooms[ni]->x] = (char)(97+i);
		
		//cout << randStart << " " << randRoom << endl;	
		//debugGrid(i);
	}
}

int avail[N_ROOMS * 2 + 2][3];
int es[4][2] = {{0,-1},{1,0},{0,1},{-1,0}};
void switchQueue(int a, int b) {
	int t1 = avail[a][0], t2 = avail[a][1], t3 = avail[a][2];
	avail[a][0] = avail[b][0];
	avail[a][1] = avail[b][1];
	avail[a][2] = avail[b][2];
	avail[b][0] = t1;
	avail[b][1] = t2;
	avail[b][2] = t3;
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
int findAvail(int x, int y) {
	for(int i = 0; i < n_av; i++)
		if(avail[i][0] == x && avail[i][1] == y) return i;
	return -1;
}
/*	algoritmo come il 5 ma con un min-heap per le posizioni disponibili,
con confronto basato su stanze adiacenti (già presenti) e probabilità 1/2;
ricomincia dall'inizio se arriva a un nodo che ha una foglia sola ed esce come
scelta casuale la seconda foglia
*/
void generateMap7()
{
	//inizializza rooms a NULL, tranne prima cella inizializzata
	for(int i = 1; i < N_ROOMS; i++) rooms[i] = NULL;
	rooms[0] = initRoom(0);
	grid[N_ROOMS][N_ROOMS] = 'a';	//scrivi la prima stanza nella griglia
	//aggiungi stanze adiacenti ad avail
	for(int i = 0; i < 4; i++) {
		avail[i][0] = es[i][0];
		avail[i][1] = es[i][1];
		avail[i][2] = 1;
	}
	n_av = 4;
	//debugGrid(0);
	//genera stanze
	for(int i = 1; i < N_ROOMS; i++)
	{
 		//cout << n_av<<endl;
		int randRoom = 0;
		while(randRoom*2 + 1 < n_av) {
			if(rand() % 3 == 0) break;
			else {
				if(rand() % 2) randRoom = randRoom*2 + 1;
				else if(randRoom*2 + 1 < n_av) randRoom = randRoom*2 + 1;
				else randRoom = 0;
			}
		}
		rooms[i] = addDoor3(avail[randRoom][0], avail[randRoom][1], i);
		bool first = true;
		int cx = rooms[i]->x, cy = rooms[i]->y;
		for(int j = 0; j < 4; j++) {
			int nx = cx + es[j][0], ny = cy + es[j][1];
			pRoom pr = findRoom(i, nx, ny);
			int ind_avail = findAvail(nx, ny);
			//cout << "AAH:" <<i<<":"<< (pr==NULL) << " " << ind_avail<<"; ";
			//if(pr == NULL) cout << "NULL";
			//else cout << "--" << pr->id;
			if(pr != NULL) {
				pr->connected[(j+2)%4] = rooms[i];
				rooms[i]->connected[j] = pr;
				pr->n_doors_sides++;
				rooms[i]->n_doors_sides++;
			}
			else if(ind_avail != -1) {
				//cout << "B";
				avail[ind_avail][2]++;
				checkQueue(ind_avail, n_av);
			}
			else {
				int ind;
				if(first) {
					ind = randRoom;
					first = false;
				} else {
					ind = n_av;
					n_av++;
				}
				avail[ind][0] = nx;
				avail[ind][1] = ny;
				avail[ind][2] = 1;
				/*cout << "NULL"<<i<<":"<<nx<<","<<ny<<":";
				for(int k = 0; k < 4; k++) {
					int x = nx + es[k][0], y = ny + es[k][1];
					cout << (findRoom(i+1,x,y)==NULL)<<" ";
					if(findRoom(i+1, x, y) != NULL) avail[ind][2]++;
				}*/
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

		grid[N_ROOMS + rooms[i]->y][N_ROOMS + rooms[i]->x] = (char)(97+i);

		//debugGrid(i);
		//for(int i = 0; i < n_av; i++) cout << avail[i][0] << "," << avail[i][1] << "," << avail[i][2] << " ";
		//for(int i = 0; i < n_av; i++) cout << avail[i][0] << "," << avail[i][1] << " ";
		//for(int i = 0; i < n_av; i++) cout << avail[i][2] << " ";
		//cout << "\n\n\n";
	}
}

/* come 7 con inizio casuale per controllo caselle adiacenti
*/
void generateMap8()
{
	//inizializza rooms a NULL, tranne prima cella inizializzata
	for(int i = 1; i < N_ROOMS; i++) rooms[i] = NULL;
	rooms[0] = initRoom(0);
	grid[N_ROOMS][N_ROOMS] = 'a';	//scrivi la prima stanza nella griglia
	//aggiungi stanze adiacenti ad avail
	int r = rand() % 4;
	for(int i = 0; i < 4; i++) {
		avail[i][0] = es[(r+i)%4][0];
		avail[i][1] = es[(r+i)%4][1];
		avail[i][2] = 1;
	}
	n_av = 4;
	//debugGrid(0);
	//genera stanze
	for(int i = 1; i < N_ROOMS; i++)
	{
 		//cout << n_av<<endl;
		int randRoom = 0;
		while(randRoom*2 + 1 < n_av) {
			if(rand() % 3 == 0) break;
			else {
				if(rand() % 2) randRoom = randRoom*2 + 1;
				else if(randRoom*2 + 1 < n_av) randRoom = randRoom*2 + 1;
				else randRoom = 0;
			}
		}
		rooms[i] = addDoor3(avail[randRoom][0], avail[randRoom][1], i);

		bool first = true;
		int cx = rooms[i]->x, cy = rooms[i]->y;
		r = rand() % 4;
		bool r_first = true;
		for(int j = r; j!=r||r_first; j=(j+1)%4) {
			if(r_first) r_first = false;
			int nx = cx + es[j][0], ny = cy + es[j][1];
			pRoom pr = findRoom(i, nx, ny);
			int ind_avail = findAvail(nx, ny);
			//cout << "AAH:" <<i<<":"<< (pr==NULL) << " " << ind_avail<<"; ";
			//if(pr == NULL) cout << "NULL";
			//else cout << "--" << pr->id;
			if(pr != NULL) {
				pr->connected[(j+2)%4] = rooms[i];
				rooms[i]->connected[j] = pr;
				pr->n_doors_sides++;
				rooms[i]->n_doors_sides++;
			}
			else if(ind_avail != -1) {
				//cout << "B";
				avail[ind_avail][2]++;
				checkQueue(ind_avail, n_av);
			}
			else {
				int ind;
				if(first) {
					ind = randRoom;
					first = false;
				} else {
					ind = n_av;
					n_av++;
				}
				avail[ind][0] = nx;
				avail[ind][1] = ny;
				avail[ind][2] = 1;
				/*cout << "NULL"<<i<<":"<<nx<<","<<ny<<":";
				for(int k = 0; k < 4; k++) {
					int x = nx + es[k][0], y = ny + es[k][1];
					cout << (findRoom(i+1,x,y)==NULL)<<" ";
					if(findRoom(i+1, x, y) != NULL) avail[ind][2]++;
				}*/
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

		grid[N_ROOMS + rooms[i]->y][N_ROOMS + rooms[i]->x] = (char)(97+i);

		//debugGrid(i);
		//for(int i = 0; i < n_av; i++) cout << avail[i][0] << "," << avail[i][1] << "," << avail[i][2] << " ";
		//for(int i = 0; i < n_av; i++) cout << avail[i][0] << "," << avail[i][1] << " ";
		//for(int i = 0; i < n_av; i++) cout << avail[i][2] << " ";
		//cout << "\n\n\n";
	}
}



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

/*
0 1 2 3 4 5 6 7 8
				0
		1			2
	3		4	5		6
7		8
				1
		2			3
	4		5	6		7
8		9
*/


int main() {

	srand(time(NULL));
/*
	initGrid();
	generateMap8();
	countGrid();
*/
/*	for(int i = 0; i < 100; i++) {
		initGrid();
		generateMap4();
		printGrid();
		countGrid();
		cout << endl << i+1;
		for(int j = 0; j < 20; j++) cout << "=";
		cout << endl;
		cout << endl;
	}*/
	
/*
	freopen("alg1.txt", "w", stdout);
	for(int i = 0; i < 5; i++) mem[i] = 0;
	for(int i = 0; i < 100; i++) {
		initGrid();
		generateMap1();
		printGrid();
		countGrid();
		cout << endl << i+1;
		for(int j = 0; j < 20; j++) cout << "=";
		cout << endl;
		cout << endl;
	}
	for(int i = 0; i < 5; i++) cout << i << ":" << mem[i]/100. << " ";
	freopen("alg2.txt", "w", stdout);
	for(int i = 0; i < 5; i++) mem[i] = 0;
	for(int i = 0; i < 100; i++) {
		initGrid();
		generateMap2();
		printGrid();
		countGrid();
		cout << endl << i+1;
		for(int j = 0; j < 20; j++) cout << "=";
		cout << endl;
		cout << endl;
	}
	for(int i = 0; i < 5; i++) cout << i << ":" << mem[i]/100. << " ";
	freopen("alg3.txt", "w", stdout);
	for(int i = 0; i < 5; i++) mem[i] = 0;
	for(int i = 0; i < 100; i++) {
		initGrid();
		generateMap3();
		printGrid();
		countGrid();
		cout << endl << i+1;
		for(int j = 0; j < 20; j++) cout << "=";
		cout << endl;
		cout << endl;
	}
	for(int i = 0; i < 5; i++) cout << i << ":" << mem[i]/100. << " ";
	freopen("alg4.txt", "w", stdout);
	for(int i = 0; i < 5; i++) mem[i] = 0;
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
	for(int i = 0; i < 5; i++) cout << i << ":" << mem[i]/100. << " ";
	freopen("alg5.txt", "w", stdout);
	for(int i = 0; i < 5; i++) mem[i] = 0;
	for(int i = 0; i < 100; i++) {
		initGrid();
		generateMap5();
		printGrid();
		countGrid();
		cout << endl << i+1;
		for(int j = 0; j < 20; j++) cout << "=";
		cout << endl;
		cout << endl;
	}
	for(int i = 0; i < 5; i++) cout << i << ":" << mem[i]/100. << " ";
	freopen("alg6.txt", "w", stdout);
	for(int i = 0; i < 5; i++) mem[i] = 0;
	for(int i = 0; i < 100; i++) {
		initGrid();
		generateMap6();
		printGrid();
		countGrid();
		cout << endl << i+1;
		for(int j = 0; j < 20; j++) cout << "=";
		cout << endl;
		cout << endl;
	}
	for(int i = 0; i < 5; i++) cout << i << ":" << mem[i]/100. << " ";
	freopen("alg7.txt", "w", stdout);
	for(int i = 0; i < 5; i++) mem[i] = 0;
	for(int i = 0; i < 100; i++) {
		initGrid();
		generateMap7();
		printGrid();
		countGrid();
		cout << endl << i+1;
		for(int j = 0; j < 20; j++) cout << "=";
		cout << endl;
		cout << endl;
	}
	for(int i = 0; i < 5; i++) cout << i << ":" << mem[i]/100. << " ";
	freopen("alg8.txt", "w", stdout);
	for(int i = 0; i < 5; i++) mem[i] = 0;
	for(int i = 0; i < 100; i++) {
		initGrid();
		generateMap8();
		printGrid();
		countGrid();
		cout << endl << i+1;
		for(int j = 0; j < 20; j++) cout << "=";
		cout << endl;
		cout << endl;
	}
	for(int i = 0; i < 5; i++) cout << i << ":" << mem[i]/100. << " ";
*/

	const double ATT = 100000;
	cout << "1: ";
	for(int i = 0; i < 5; i++) mem[i] = 0;
	for(int i = 0; i < 2; i++) dim[i] = 0;
	for(int i = 0; i < ATT; i++) {
		initGrid();
		generateMap1();
		countGrid();
		countDimensions();
	}
	for(int i = 0; i < 5; i++) cout << i << ":" << mem[i]/ATT << " ";
	cout << endl;
	for(int i = 0; i < 2; i++) cout << dim[i]/ATT << " ";
	cout << endl;
	cout << "2: ";
	for(int i = 0; i < 5; i++) mem[i] = 0;
	for(int i = 0; i < 2; i++) dim[i] = 0;
	for(int i = 0; i < ATT; i++) {
		initGrid();
		generateMap2();
		countGrid();
		countDimensions();
	}
	for(int i = 0; i < 5; i++) cout << i << ":" << mem[i]/ATT << " ";
	cout << endl;
	for(int i = 0; i < 2; i++) cout << dim[i]/ATT << " ";
	cout << endl;
	cout << "3: ";
	for(int i = 0; i < 5; i++) mem[i] = 0;
	for(int i = 0; i < 2; i++) dim[i] = 0;
	for(int i = 0; i < ATT; i++) {
		initGrid();
		generateMap3();
		countGrid();
		countDimensions();
	}
	for(int i = 0; i < 5; i++) cout << i << ":" << mem[i]/ATT << " ";
	cout << endl;
	for(int i = 0; i < 2; i++) cout << dim[i]/ATT << " ";
	cout << endl;
	cout << "4: ";
	for(int i = 0; i < 5; i++) mem[i] = 0;
	for(int i = 0; i < 2; i++) dim[i] = 0;
	for(int i = 0; i < ATT; i++) {
		initGrid();
		generateMap4();
		countGrid();
		countDimensions();
	}
	for(int i = 0; i < 5; i++) cout << i << ":" << mem[i]/ATT << " ";
	cout << endl;
	for(int i = 0; i < 2; i++) cout << dim[i]/ATT << " ";
	cout << endl;
	cout << "5: ";
	for(int i = 0; i < 5; i++) mem[i] = 0;
	for(int i = 0; i < 2; i++) dim[i] = 0;
	for(int i = 0; i < ATT; i++) {
		initGrid();
		generateMap5();
		countGrid();
		countDimensions();
	}
	for(int i = 0; i < 5; i++) cout << i << ":" << mem[i]/ATT << " ";
	cout << endl;
	for(int i = 0; i < 2; i++) cout << dim[i]/ATT << " ";
	cout << endl;
	cout << "6: ";
	for(int i = 0; i < 5; i++) mem[i] = 0;
	for(int i = 0; i < 2; i++) dim[i] = 0;
	for(int i = 0; i < ATT; i++) {
		initGrid();
		generateMap6();
		countGrid();
		countDimensions();
	}
	for(int i = 0; i < 5; i++) cout << i << ":" << mem[i]/ATT << " ";
	cout << endl;
	for(int i = 0; i < 2; i++) cout << dim[i]/ATT << " ";
	cout << endl;
	cout << "7: ";
	for(int i = 0; i < 5; i++) mem[i] = 0;
	for(int i = 0; i < 2; i++) dim[i] = 0;
	for(int i = 0; i < ATT; i++) {
		initGrid();
		generateMap7();
		countGrid();
		countDimensions();
	}
	for(int i = 0; i < 5; i++) cout << i << ":" << mem[i]/ATT << " ";
	cout << endl;
	for(int i = 0; i < 2; i++) cout << dim[i]/ATT << " ";
	cout << endl;
	cout << "8: ";
	for(int i = 0; i < 5; i++) mem[i] = 0;
	for(int i = 0; i < 2; i++) dim[i] = 0;
	for(int i = 0; i < ATT; i++) {
		initGrid();
		generateMap8();
		countGrid();
		countDimensions();
	}
	for(int i = 0; i < 5; i++) cout << i << ":" << mem[i]/ATT << " ";
	cout << endl;
	for(int i = 0; i < 2; i++) cout << dim[i]/ATT << " ";
	cout << endl;



}