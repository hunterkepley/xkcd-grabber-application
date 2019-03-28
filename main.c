#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int itemsSize;
	char **items;
} List;

void displayList(List list, int *hl, WINDOW *w);

List initList(int itemsSize, char **rawList);

int main() {
	WINDOW *w;
	initscr(); // Starts curses
	keypad(stdscr, TRUE);
	noecho();

	char *rawFirst[] = {"Random", "Multiple comics", "Specific comic"};
	char *rawSecond[] = {"Change directory", "Same directory", "Cancel/Exit"};
	List firstList = initList(3, rawFirst);
	List secondList = initList(3, rawSecond);

	int hl = 0;

	int currentList = 0; // 0 = first list, 1 = second list

	int firstChoice = -1, secondChoice = -1;
	int *finalChoice = &firstChoice;

	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);

	w = newwin(6, xMax-12, yMax-8, 5); // Create a new window
	box(w, 0, 0); // sets default borders for the window

	char title[] = "XKCD Comic Grabber";
	mvprintw(yMax/3,(xMax-strlen(title))/2, "%s",title);
	refresh();
	wrefresh(w); // update the terminal screen

	keypad(w, TRUE); // enable keyboard input for the window

	curs_set(0);

	// get input
	while(1) {
		printw("XKCD Comic Grabber");

		if(currentList == 0)
			displayList(firstList, &hl, w);
		else if(currentList == 1)
			displayList(secondList, &hl, w);
		else {
			break;
		}
		
		int choice = wgetch(w);

		if(choice == 10) {
			currentList++;
			hl = 0;
		} else if(choice == 'q')  // Quit
			break;
		
	}
	delwin(w); // Delete window
	endwin(); // End curses mode

	for(int i = 0; i < firstList.itemsSize; i++)
		free(firstList.items[i]);
	for(int i = 0; i < secondList.itemsSize; i++)
		free(secondList.items[i]);
	free(firstList.items);
	free(secondList.items);

	return 0;
}

void displayList(List list, int *hl, WINDOW *w) {
	for(int i = 0; i < list.itemsSize; i++) {
			if(i == *hl)
				wattron(w, A_STANDOUT);
			mvwprintw(w, i+1, 1, list.items[i]);
			wattroff(w, A_STANDOUT);
	}
	int choice = wgetch(w);
	switch(choice) {
		case KEY_UP:
			*hl--;
			*hl = (*hl < 0) ? list.itemsSize : *hl;
			break;
		case KEY_DOWN:
			*hl++;
			*hl = (*hl > list.itemsSize-1) ? 0 : *hl;
			break;
	}
}

List initList(int itemsSize, char **rawList) {
	char **list;
	list = malloc(itemsSize * sizeof(char*));
	for(int i = 0; i < itemsSize; i++) {
		list[i] = malloc(sizeof(rawList[i]));
		strcpy(list[i], rawList[i]);
	}
	List firstList = {itemsSize, list};
	return firstList;
}