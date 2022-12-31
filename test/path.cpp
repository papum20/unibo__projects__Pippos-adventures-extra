#include <iostream>
#include <cstring>
#include "coordinate.hpp"
using namespace std;


#define WIDTH 254
#define WIDTH_T 255
#define HEIGHT 40
#define AREA (WIDTH * HEIGHT)
#define PATH_MAX (AREA)
#define WALL 'x'
#define FLOOR ' '
#define STEP '.'
char map[HEIGHT][WIDTH_T] =
{
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
	"x         xxxxxxxxx                                                                                                                                                                                                                                          x",
	"x         xxxxxxxxx         xxxxxxxxx                  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx         xxxxxxxxxxxxxxxxxxxxxxxxxxx         xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx         x",
	"x         xxxxxxxxx         xxxxxxxxxxxxxxxxxxxxxxxxxxx                           xxxxxxxxx                  xxxxxxxxx                                                                                                                     xxxxxxxxx         x",
	"x         xxxxxxxxx         xxxxxxxxx                           xxxxxxxxx         xxxxxxxxxxxxxxxxxx         xxxxxxxxx         xxxxxxxxx         xxxxxxxxxxxxxxxxxx         xxxxxxxxx         xxxxxxxxxxxxxxxxxx         xxxxxxxxxxxxxxxxxx                  x",
	"x         xxxxxxxxx                           xxxxxxxxxxxxxxxxxx         xxxxxxxxxxxxxxxxxx         xxxxxxxxxxxxxxxxxx         xxxxxxxxx                           xxxxxxxxxxxxxxxxxx         xxxxxxxxxxxxxxxxxx         xxxxxxxxx                           x",
	"x                           xxxxxxxxxxxxxxxxxx                           xxxxxxxxxxxxxxxxxx                           xxxxxxxxx         xxxxxxxxxxxxxxxxxx                           xxxxxxxxx         xxxxxxxxx                  xxxxxxxxx                  x",
	"xxxxxxxxxx                                                                                          xxxxxxxxx                           xxxxxxxxxxxxxxxxxx                                             xxxxxxxxxxxxxxxxxxxxxxxxxxx         xxxxxxxxxxxxxxxxxxx",
	"x         xxxxxxxxx         xxxxxxxxx         xxxxxxxxxxxxxxxxxxxxxxxxxxx                  xxxxxxxxx         xxxxxxxxxxxxxxxxxxxxxxxxxxx                  xxxxxxxxx         xxxxxxxxx         xxxxxxxxx                                    xxxxxxxxx         x",
	"x         xxxxxxxxx                  xxxxxxxxx                  xxxxxxxxx         xxxxxxxxx                                                      xxxxxxxxxxxxxxxxxx         xxxxxxxxx                  xxxxxxxxx         xxxxxxxxxxxxxxxxxx                  x",
	"x         xxxxxxxxx         xxxxxxxxxxxxxxxxxxxxxxxxxxx         xxxxxxxxx                           xxxxxxxxxxxxxxxxxxxxxxxxxxx         xxxxxxxxx         xxxxxxxxx         xxxxxxxxxxxxxxxxxxxxxxxxxxx                  xxxxxxxxxxxxxxxxxx                  x",
	"x                                             xxxxxxxxx         xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx                  xxxxxxxxx                                                                                 xxxxxxxxx         xxxxxxxxx                  x",
	"x                  xxxxxxxxxxxxxxxxxxxxxxxxxxx                  xxxxxxxxx                           xxxxxxxxx                  xxxxxxxxx         xxxxxxxxxxxxxxxxxxxxxxxxxxx         xxxxxxxxxxxxxxxxxxxxxxxxxxx                  xxxxxxxxxxxxxxxxxx         x",
	"xxxxxxxxxx                                             xxxxxxxxx         xxxxxxxxxxxxxxxxxx         xxxxxxxxx                                                      xxxxxxxxx                                             xxxxxxxxx         xxxxxxxxx         x",
	"xxxxxxxxxx         xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx                  xxxxxxxxx                  xxxxxxxxxxxxxxxxxx                                             xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx                                    x",
	"xxxxxxxxxx                           xxxxxxxxx                  xxxxxxxxx                  xxxxxxxxx                           xxxxxxxxx         xxxxxxxxxxxxxxxxxx                                             xxxxxxxxx         xxxxxxxxx                  x",
	"x         xxxxxxxxx         xxxxxxxxx         xxxxxxxxx                  xxxxxxxxx                           xxxxxxxxx         xxxxxxxxx                           xxxxxxxxx         xxxxxxxxx         xxxxxxxxx                           xxxxxxxxx         x",
	"x         xxxxxxxxx         xxxxxxxxx                  xxxxxxxxx                  xxxxxxxxx                  xxxxxxxxx                  xxxxxxxxxxxxxxxxxx         xxxxxxxxx                                    xxxxxxxxxxxxxxxxxx                           x",
	"x                  xxxxxxxxx                  xxxxxxxxx                           xxxxxxxxx                  xxxxxxxxxxxxxxxxxx                           xxxxxxxxx         xxxxxxxxx         xxxxxxxxx         xxxxxxxxx                  xxxxxxxxx         x",
	"xxxxxxxxxx                           xxxxxxxxx         xxxxxxxxx                           xxxxxxxxxxxxxxxxxx                  xxxxxxxxxxxxxxxxxx                           xxxxxxxxx                           xxxxxxxxx         xxxxxxxxxxxxxxxxxx         x",
	"                   xxxxxxxxx         xxxxxxxxx         xxxxxxxxx                                                                                 xxxxxxxxx         xxxxxxxxxxxxxxxxxx         xxxxxxxxx         xxxxxxxxx         xxxxxxxxx                   ",
	"x         xxxxxxxxxxxxxxxxxx         xxxxxxxxx                                    xxxxxxxxx         xxxxxxxxx         xxxxxxxxxxxxxxxxxx                                    xxxxxxxxx         xxxxxxxxx         xxxxxxxxx         xxxxxxxxx                  x",
	"xxxxxxxxxx         xxxxxxxxx                  xxxxxxxxx                  xxxxxxxxx                           xxxxxxxxx                           xxxxxxxxx         xxxxxxxxxxxxxxxxxx                           xxxxxxxxx         xxxxxxxxxxxxxxxxxx         x",
	"x                  xxxxxxxxx         xxxxxxxxxxxxxxxxxxxxxxxxxxx                  xxxxxxxxxxxxxxxxxx         xxxxxxxxx         xxxxxxxxx                           xxxxxxxxx         xxxxxxxxxxxxxxxxxx         xxxxxxxxx         xxxxxxxxx                  x",
	"xxxxxxxxxx         xxxxxxxxx                           xxxxxxxxx         xxxxxxxxx                  xxxxxxxxxxxxxxxxxx                           xxxxxxxxxxxxxxxxxxxxxxxxxxx         xxxxxxxxx                  xxxxxxxxx         xxxxxxxxx                  x",
	"xxxxxxxxxx                  xxxxxxxxx         xxxxxxxxxxxxxxxxxx                           xxxxxxxxx         xxxxxxxxx         xxxxxxxxx                                             xxxxxxxxx                  xxxxxxxxx         xxxxxxxxxxxxxxxxxx         x",
	"x         xxxxxxxxx         xxxxxxxxx                  xxxxxxxxx         xxxxxxxxx                           xxxxxxxxx                           xxxxxxxxxxxxxxxxxx                           xxxxxxxxx                                    xxxxxxxxx         x",
	"x                                             xxxxxxxxxxxxxxxxxx                  xxxxxxxxxxxxxxxxxx                  xxxxxxxxx                           xxxxxxxxx         xxxxxxxxx                  xxxxxxxxxxxxxxxxxx         xxxxxxxxxxxxxxxxxx         x",
	"xxxxxxxxxx                  xxxxxxxxx                                                               xxxxxxxxx         xxxxxxxxx                           xxxxxxxxx                  xxxxxxxxx                  xxxxxxxxx         xxxxxxxxxxxxxxxxxx         x",
	"x         xxxxxxxxxxxxxxxxxx                  xxxxxxxxx                  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx                  xxxxxxxxxxxxxxxxxx                  xxxxxxxxx                  xxxxxxxxx                                    xxxxxxxxx         x",
	"x                  xxxxxxxxx         xxxxxxxxx                                                               xxxxxxxxx                           xxxxxxxxx         xxxxxxxxxxxxxxxxxx                           xxxxxxxxx         xxxxxxxxxxxxxxxxxx         x",
	"x                  xxxxxxxxxxxxxxxxxx         xxxxxxxxxxxxxxxxxx         xxxxxxxxxxxxxxxxxxxxxxxxxxx                  xxxxxxxxxxxxxxxxxx                  xxxxxxxxx                  xxxxxxxxxxxxxxxxxx         xxxxxxxxx                  xxxxxxxxx         x",
	"x                           xxxxxxxxx                  xxxxxxxxx                                    xxxxxxxxx                  xxxxxxxxxxxxxxxxxx                                             xxxxxxxxx         xxxxxxxxx         xxxxxxxxxxxxxxxxxx         x",
	"xxxxxxxxxxxxxxxxxxx                  xxxxxxxxx                  xxxxxxxxxxxxxxxxxx                           xxxxxxxxxxxxxxxxxx                  xxxxxxxxxxxxxxxxxxxxxxxxxxx         xxxxxxxxx                  xxxxxxxxx         xxxxxxxxxxxxxxxxxx         x",
	"x                                    xxxxxxxxx         xxxxxxxxx                                    xxxxxxxxx         xxxxxxxxx                                                               xxxxxxxxx         xxxxxxxxx         xxxxxxxxxxxxxxxxxx         x",
	"x         xxxxxxxxx                  xxxxxxxxx         xxxxxxxxx         xxxxxxxxx                                                               xxxxxxxxx                  xxxxxxxxx                  xxxxxxxxxxxxxxxxxx         xxxxxxxxxxxxxxxxxx         x",
	"x         xxxxxxxxx                  xxxxxxxxx                  xxxxxxxxxxxxxxxxxx                  xxxxxxxxxxxxxxxxxx         xxxxxxxxxxxxxxxxxx                                    xxxxxxxxx                  xxxxxxxxx                  xxxxxxxxx         x",
	"x                  xxxxxxxxx                           xxxxxxxxx                  xxxxxxxxxxxxxxxxxxxxxxxxxxx                  xxxxxxxxx                           xxxxxxxxx         xxxxxxxxxxxxxxxxxx         xxxxxxxxx                  xxxxxxxxx         x",
	"x                  xxxxxxxxxxxxxxxxxx                                                                                 xxxxxxxxx                                    xxxxxxxxxxxxxxxxxx                           xxxxxxxxx                                    x",
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx         xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
};
bool isWall(char c) {
	return c == WALL || c == WALL - 32;
}

class Coord {
	public:
		int x, y;
		Coord() {
			x = y = -1;
		}
		Coord(int x, int y) {
			this->x = x;
			this->y = y;
		}
		bool isValid() {
			return x != -1 || y != -1;
		}
		bool equals(Coord B) {
			return this->x == B.x && this->y == B.y;
		}
		Coord summed(Coord B) {
			return Coord(this->x + B.x, this->y + B.y);
		}
};
#define DIRECTIONS_N 4
const Coord DIRECTIONS[DIRECTIONS_N] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

struct queue_coord {
	Coord c;
	queue_coord *next;
}; typedef queue_coord *p_queue_coord;
class Queue {
	private:
		p_queue_coord head;
		p_queue_coord tail;
		int size;
	public:
		Queue() {
			head = tail = NULL;
			size = 0;
		}
		void push(Coord c) {
			if(head == NULL) {
				head = new queue_coord;
				tail = head;
			} else {
				tail->next = new queue_coord;
				tail = tail->next;
			}
			tail->c = c;
			tail->next = NULL;
			size++;
		}
		Coord pop() {
			p_queue_coord tmp = head->next;
			Coord res = head->c;
			delete head;
			head = tmp;
			if(head == NULL) tail = NULL;
			size--;
			return res;
		}
		int getSize() {
			return size;
		}
		bool isEmpty() {
			return (size == 0);
		}
		void erase() {
			while(head != NULL) {
				p_queue_coord tmp = head;
				head = head->next;
				delete tmp;
			}
		}
		void print() {
			cout << endl;
			p_queue_coord p = head;
			while(p != NULL) {
				printf("%d,%d ", p->c.x, p->c.y);
				p = p->next;
			}
			cout << endl;
		}
};

struct Step {
	Coord c;
	int len;
};


int findPath(Coord path[], Coord A, Coord B) {
	Step parent[HEIGHT][WIDTH];
	for(int y = 0; y < HEIGHT; y++)
		for(int x = 0; x <WIDTH; x++) parent[y][x] = {Coord(-1, -1), -1};
	parent[A.y][A.x] = {Coord(-2, -2), 0};
	
	int len = 0;
	Queue Q = Queue();
	Q.push(A);
	bool found = false;
	do {
		Coord t = Q.pop();
		if(t.equals(B)) found = true;
		else {
			for(int d = 0; d < DIRECTIONS_N; d++) {
				Coord nt = t.summed(DIRECTIONS[d]);
				//cout << t.x << "-" << t.y << " ";
				//cout << nt.x << "-" << nt.y << " ";
				//cout << parent[nt.y][nt.x].c.isValid() << map[nt.y][nt.x] << (map[nt.y][nt.x] == FLOOR) << Q.getSize();
				//cout << endl;
				if(!parent[nt.y][nt.x].c.isValid() && map[nt.y][nt.x] == FLOOR) {
					parent[nt.y][nt.x] = {t, parent[t.y][t.x].len + 1};
					Q.push(nt);
					len = parent[nt.y][nt.x].len;
				}
			}
			/*for(int y = 0; y < HEIGHT; y++) {
				for(int x = 0; x < WIDTH; x++) {
					if(parent[y][x].c.isValid()) cout << 'a';
					else cout << map[y][x];
				}
				cout << endl;
			}
			Q.print();*/
		}
	} while(!found && !Q.isEmpty());


	if(parent[B.y][B.x].c.isValid()) {
		path[len - 1] = B;
		for(int i = len - 1; i > 0; i--) {
			path[i - 1] = parent[B.y][B.x].c;
			B = path[i - 1];
		}
		Q.erase();
		return len;
	} else return -1;
}



void checkLine_floor(char map[HEIGHT][WIDTH], char c, Coordinate start, Coordinate end) {
	int found = 0;
	Coordinate delta = Coordinate::unitVector(start, end);

	Coordinate i = start;
	bool ended = false;
	//Coordinate s = Coordinate(end,start.getNegative());
	//cout<<s.x<<s.y<<" ";
	//cout << start.x<<start.y<<" "<<end.x<<end.y<<" "<<i.x<<i.y<<" "<<delta.x<<delta.y<<" "<<deltaMax;
//	cout << delta.x<<","<<delta.y<<" "<<start.x<<","<<start.y<<" "<<end.x<<","<<end.y<<endl;
	while(!ended && !isWall(map[i.inty()][i.intx()])) {
//		cout << i.x<<","<<i.y<<" ";
		map[i.inty()][i.intx()] = c;
		Coordinate j = Coordinate(i, delta);
		Coordinate t1 = Coordinate(i.x, j.y);
		Coordinate t2 = Coordinate(j.x, i.y);
		if(isWall(map[t1.inty()][t1.intx()]) && isWall(map[t2.inty()][t2.intx()])) ended = true;
		else i = j;
		if(i.equals(end)) ended = true;
	}
//	cout<<endl;
}
void checkLine_ceil(char map[HEIGHT][WIDTH], char c, Coordinate start, Coordinate end) {
	int found = 0;
	Coordinate delta = Coordinate::unitVector(start, end);

	Coordinate i = start;
	bool ended = false;
	//Coordinate s = Coordinate(end,start.getNegative());
	//cout<<s.x<<s.y<<" ";
	//cout << start.x<<start.y<<" "<<end.x<<end.y<<" "<<i.x<<i.y<<" "<<delta.x<<delta.y<<" "<<deltaMax;
//	cout << delta.x<<","<<delta.y<<" "<<start.x<<","<<start.y<<" "<<end.x<<","<<end.y<<endl;
	while(!ended && !isWall(map[i.ceily()][i.ceilx()])) {
//		cout << i.x<<","<<i.y<<" ";
		map[i.ceily()][i.ceilx()] = c;
		Coordinate j = Coordinate(i, delta);
		Coordinate t1 = Coordinate(i.x, j.y);
		Coordinate t2 = Coordinate(j.x, i.y);
		if(isWall(map[t1.ceily()][t1.ceilx()]) && isWall(map[t2.ceily()][t2.ceilx()])) ended = true;
		else i = j;
		if(i.equals_int(end)) ended = true;
	}
//	cout<<endl;
}


void printMap(char map[HEIGHT][WIDTH]) {
	for(int y = 0; y < HEIGHT; y++)	{
		for(int x = 0; x < WIDTH; x++) cout << map[y][x];
		cout << endl;
	}
	cout << endl;
}


void printC(Coordinate c) {
	cout << c.x<<","<<c.y<<" "<<c.getNegative().x<<","<<c.getNegative().y<<endl;
}



void vision(char res[HEIGHT][WIDTH], Coordinate A, char c1, char c2, char c3) {
	char floor_map[HEIGHT][WIDTH];
	char ceil_map[HEIGHT][WIDTH];
	for(int y = 0; y < HEIGHT; y++) {
		for(int x = 0; x < WIDTH; x++) {
			floor_map[y][x] = map[y][x];
			ceil_map[y][x] = map[y][x];
		}
	}
		
	for(int y = 0; y < HEIGHT; y++) {
		int step = WIDTH - 1;
		if(y == 0 || y == HEIGHT - 1) step = 1;
		for(int x = 0; x < WIDTH; x += step) {
			checkLine_floor(floor_map, c1, A, Coordinate(x, y));
			checkLine_ceil(ceil_map, c2, A, Coordinate(x, y));
			//printMap(floor_map);
			//printMap(ceil_map);
		}
		/*
		char t;
		cin >> t;	
		*/
	}
	/*floor_map[A.inty()][A.intx()] = 'A';
	ceil_map[A.ceily()][A.ceilx()] = 'B';
	printMap(floor_map);
	printMap(ceil_map);*/

	for(int y = 0; y < HEIGHT; y++) {
		for(int x = 0; x < WIDTH; x++) {
			if(!isWall(res[y][x])) {
				if(floor_map[y][x] != FLOOR && ceil_map[y][x] != FLOOR) res[y][x] = c3;
				else if(floor_map[y][x] != FLOOR) res[y][x] = c1;
				else if(ceil_map[y][x] != FLOOR) res[y][x] = c2;
			}
		}
	}
	if(res[A.inty()][A.intx()] != WALL) res[A.inty()][A.intx()] = c3 - 32;
	else res[A.inty()][A.intx()] = 'X';
}


int main() {

	Coordinate A[8] = {Coordinate(100, 20), Coordinate (1,1), Coordinate(200,2), Coordinate(200, 38), Coordinate(12, 30), Coordinate(30, 18)};
	int len = 6;

	char res[HEIGHT][WIDTH];
	for(int y = 0; y < HEIGHT; y++) {
		for(int x = 0; x < WIDTH; x++) {
			res[y][x] = map[y][x];
		}
	}

	char c = 'a';
	for(int i = 0; i < len; i++) {
		vision(res, A[i], c++, c++, c++);
		printMap(res);
	}

		/*
	Coord A(1, 1), B(150, 38);
	Coord path[PATH:MAX];
	int path_len = findPath(path, A, B);

	map[A.y][A.x] = 'A';
	map[B.y][B.x] = 'B';
	for(int y = 0; y < HEIGHT; y++) 
		cout << map[y] << endl;

	for(int i = 0; i < path_len; i++) map[path[i].y][path[i].x] = STEP;
	map[A.y][A.x] = 'A';
	map[B.y][B.x] = 'B';

	cout << path_len << endl;
	for(int y = 0; y < HEIGHT; y++) 
		cout << map[y] << endl;
	cout << endl;
	*/
}