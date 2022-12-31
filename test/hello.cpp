#include <iostream>
#include <typeinfo>
#include "curses.h"
#include <ctime>
//#include "timer.hpp"
//#include "cell.hpp"
//#include "animation.hpp"
#include <math.h>
//#include "coordinate.hpp"
//#include "connected_room.hpp"
using namespace std;



#define WIDTH 8
#define HEIGHT 2
#define END_CH KEY_RIGHT

//#define REFRESH_RATE 3000
#define INPUT_RATE 1. / 30 * 1000
const double TICS = (double) CLOCKS_PER_SEC;


/*
double getTime() {
	return clock() / TICS;
}

char getInput(WINDOW *in, WINDOW *wait) {
	wmove(wait, 1, 1);
	wrefresh(wait);
	wtimeout(in, REFRESH_RATE);
	char s[100];
	wgetstr(in, s);
	char c = s[0];

	wtimeout(in, INPUT_RATE);
	c = mvwgetch(in, 1, 1);
	return c;
}*/






int main()
{	
	initscr();

	WINDOW *w = newwin(10,10,10,10);
	waddch(w,'A');
	wgetch(w);


	endwin();



	return 0;
//	Room R = Room();
//	R.generate();
//
//	R.printSize();
//	
//
//	return 0;

/*
	initscr();
	start_color();
	cbreak();

	WINDOW *in = newwin(3,4,1,1);
	box(in, 0, 0);
	wrefresh(in);
	WINDOW *wait = newwin(3,4,1,5);
	box(wait, 0, 0);
	wrefresh(wait);
	keypad(in, true);

	WINDOW *out = newwin(HEIGHT+2,WIDTH+2,5,5);
	box(out, 0, 0);
	wrefresh(out);


	bool running = true;
	char ch = ' ';
	int x1 = 1, y1 = 1, x2 = 1, y2 = 2;
	while(running) {
		ch = getInput(in, wait);

		if(ch == END_CH) running = false;
		else if(ch >= 65 && ch <= 90) {
			mvwaddch(out, y1, x1, ch);
			if(x1 >= WIDTH) x1 = 1;
			else x1++; 
		}
		else if(ch >= 97 && ch <= 122) {
			mvwaddch(out, y2, x2, ch);
			if(x2 >= WIDTH) x2 = 1;
			else x2++; 
		}
		wrefresh(out);
	}

	
	getch();
	endwin();

*/


	//return 0;

	initscr();


	addch(ACS_ULCORNER);
	addch(' ');
	addch(ACS_LLCORNER);
	addch(' ');
	addch(ACS_URCORNER);	
	addch(' ');
	addch(ACS_LRCORNER);
	addch(' ');
	addch(ACS_RTEE);
	addch(' ');
	addch(ACS_LTEE);
	addch(' ');
	addch(ACS_BTEE);
	addch(' ');
	addch(ACS_TTEE);
	addch(' ');
	addch(ACS_HLINE);
	addch(' ');
	addch(ACS_VLINE);
	addch(' ');
	addch(ACS_PLUS);
	addch(' ');
	addch(ACS_S1);
	addch(' ');
	addch(ACS_S9);
	addch(' ');
	addch(ACS_DIAMOND);
	addch(' ');
	addch(ACS_CKBOARD);
	addch(' ');
	addch(ACS_DEGREE);
	addch(' ');
	addch(ACS_PLMINUS);
	addch(' ');
	addch(ACS_BULLET);
	addch(' ');
	addch(ACS_LARROW);
	addch(' ');
	addch(ACS_RARROW);
	addch(' ');
	addch(ACS_DARROW);
	addch(' ');
	addch(ACS_UARROW);
	addch(' ');
	addch(ACS_BOARD);
	addch(' ');
	addch(ACS_LANTERN);
	addch(' ');
	addch(ACS_BLOCK);
	addch(' ');
	/*printw(WACS_LRCORNER);
	printw(WACS_ULCORNER);
	printw(WACS_LLCORNER);
	printw(WACS_URCORNER);
	printw(WACS_PLUS);
	printw(WACS_S1);
	printw(WACS_S9);
	printw(WACS_DIAMOND);
	printw(WACS_CKBOARD);
	printw(WACS_DEGREE);
	printw(WACS_PLMINUS);
	printw(WACS_BULLET);
	printw(WACS_LARROW);
	printw(WACS_RARROW);
	printw(WACS_DARROW);
	printw(WACS_UARROW);
	printw(WACS_BOARD);
	printw(WACS_LANTERN);
	printw(WACS_BLOCK);
*/

	getch();
	endwin();


/*
	initscr();
	start_color();

	init_pair(3, COLOR_MAGENTA, COLOR_WHITE);
	chtype cc = 'a' | A_UNDERLINE | COLOR_PAIR(3);
	addch(cc);
	getch();

	init_pair(1, COLOR_CYAN, COLOR_YELLOW);
	attron(A_BOLD);
	chtype att = COLOR_PAIR(1);
	attron(att);
	printw("CI");
	refresh();
	getch();
	
	attroff(A_BOLD);
	init_pair(2, COLOR_GREEN, COLOR_BLUE);
	attron(COLOR_PAIR(2));
	printw("AO");
	refresh();
	getch();

	attron(COLOR_PAIR(1));
	printw("AO");
	refresh();
	getch();


	move(0, 0);
	attr_t attr;
	short color;
	int a = attr_get(&attr, &color, NULL);
	mvprintw(1, 0, "%d, %d, %d", a, attr, color);

	getch();

	endwin();
	*/
}


