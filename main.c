#include <ncurses.h>

int main() {
	initscr(); // Starts curses
	raw();
	keypad(stdscr, TRUE);
	bool init = true;
	noecho();
	while(true) {
		int ch = getch();
		if(init == true) {
			printw("XKCD GRABBER TERMINAL APPLICATION");
			init = false;
		}
		if(ch == KEY_F(1))
			break;
		refresh();
	}
	endwin(); // End curses mode

	return 0;
}
