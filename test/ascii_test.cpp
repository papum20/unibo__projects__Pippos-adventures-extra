#include <iostream>
#include <curses.h>
using namespace std;



#define PLAYER_CH 'p'
#define WALL_CH 'w'
#define EMPTY_CH ' '

#define WIDTH 135
#define HEIGHT 35

chtype wallColor, floorColor;



void fillWindow(WINDOW *win, chtype color) {
	int x1, y1, x2, y2;
	getbegyx(win, y1, x1);
	getmaxyx(win, y2, x2);
	wmove(win, 0, 0);
	wattron(win, color);
	for(int i = 0; i < (x2-x1)*(y2-y1); i++) waddch(win, ' ');
	wrefresh(win);
}

void emptyArray(char A[][WIDTH], int w, int h) {
	for(int y = 0; y < h; y++) {
		for(int x = 0; x < w; x++) A[y][x] = EMPTY_CH;
	}
}

void wallArray(char A[][WIDTH], int w, int h) {
	for(int x = 0; x < w; x++) {
		A[0][x] = WALL_CH;
		A[h-1][x] = WALL_CH;
	}
	for(int y = 0; y < h; y++) {
		A[y][0] = WALL_CH;
		A[y][w-1] = WALL_CH;
	}
}


void printArray(WINDOW *win, char A[][WIDTH], int w, int h) {
	wmove(win, 0, 0);
	for(int y = 0; y < h; y++) {
		for(int x = 0; x < w; x++) {
			if(A[y][x] == 'w') {
				wattron(win, wallColor);
				waddch(win, 'w');
			}
			else if(A[y][x] == ' ') {
				wattron(win, floorColor);
				waddch(win, ' ');
			}
		}
	}
	wrefresh(win);
}




int main() {

	//// START

	initscr();
	start_color();
	noecho();
	curs_set(0);

	//INPUT
	WINDOW *inputW = newwin(1, 1, 0, 0);
	keypad(inputW, true);

	//COLORI
	//muro
	init_color(COLOR_CYAN, 258, 156, 57);
	init_pair(1, COLOR_WHITE, COLOR_CYAN);	//1=marrone
	wallColor = COLOR_PAIR(1);
	//pavimento
	init_color(COLOR_GREEN, 121, 238, 47);
	init_pair(2, COLOR_WHITE, COLOR_GREEN);	//2=verde foresta
	floorColor = COLOR_PAIR(2);

	//LIVELLO
	WINDOW *level = newwin(HEIGHT, WIDTH, 0, 1);
	fillWindow(level, floorColor);
	wattron(level, wallColor);
	wborder(level, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wrefresh(level);
	//array
	char levelArr[HEIGHT][WIDTH];
	emptyArray(levelArr, WIDTH, HEIGHT);
	wallArray(levelArr, WIDTH, HEIGHT);

	//GIOCATORE
	char playerLeft[5] = {'|','/','|','\\','<'};
	char playerRight[5] = {'|','\\','>','/','|'};
	char playerCh[3] = {playerLeft[0], '0', playerRight[0]};
	int playerX = 50, playerY = 20;

	printArray(level, levelArr, WIDTH, HEIGHT);

	//// UPDATE
	while(true) {

	}


	endwin();

}