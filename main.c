#include <ncurses.h>
#include <stdlib.h>

int main() {
	WINDOW *w;
	initscr(); // Starts curses
	keypad(stdscr, TRUE);
	noecho();

	char list[2][8] = {"One", "Two"};
	char item[8];

	int hl = 0, choice;

	int finalChoice = -1;

	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);

	w = newwin(6, xMax-12, yMax-8, 5); // Create a new window
	box(w, 0, 0); // sets default borders for the window

	refresh();
	wrefresh(w); // update the terminal screen

	keypad(w, TRUE); // enable keyboard input for the window

	curs_set(0);

	// get input
	while(1) {
		for(int i = 0; i < (sizeof(list)/sizeof(item)); i++) {
			if(i == hl)
				wattron(w, A_STANDOUT);
			mvwprintw(w, i+1, 1, list[i]);
			wattroff(w, A_STANDOUT);
		}
		choice = wgetch(w);
		switch(choice) {
			case KEY_UP:
				hl--;
				hl = (hl < 0) ? (sizeof(list)/sizeof(item)-1) : hl;
				break;
			case KEY_DOWN:
				hl++;
				hl = (hl > (sizeof(list)/sizeof(item))-1) ? 0 : hl;
				break;
		}
		if(choice == 10) {
			finalChoice = hl;
			break;
		} else if(choice == 'q')  // Quit
			break;
		
	}
	delwin(w); // Delete window
	endwin(); // End curses mode

	if(finalChoice == 0) {
		printf("Choice 1 chosen");
	} else if(finalChoice == 1) {
		printf("Choice 2 chosen");
	}

	return 0;
}
