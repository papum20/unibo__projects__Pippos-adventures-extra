#include <iostream>
#include <curses.h>
using namespace std;



int main() {

	initscr();
	raw();

	while(true){
		char c = getch();
		printw(".%d %c.", c, c);
		getch();
	}


	getch();
	endwin();
}