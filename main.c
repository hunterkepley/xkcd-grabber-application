#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
	int itemsSize;
	char **items;
} List;

void displayList(List, int, WINDOW*);

List initList(int, char**);

void exitProgram(WINDOW *w);

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

	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);

	w = newwin(6, xMax-12, yMax-8, 5); // Create a new window
	box(w, 0, 0); // sets default borders for the window

	char title[] = "XKCD Comic Grabber";
	mvprintw(yMax/3,(xMax-strlen(title))/2, "%s",title);
	refresh();
	keypad(w, TRUE); // enable keyboard input for the window

	curs_set(0);

	char directory[100];

	bool directoryInit = false;

	// get input
	while(1) {
		printw("XKCD Comic Grabber");
		
		int choice = 0;

		if(currentList == 0) {
			displayList(firstList, hl, w);
			choice = wgetch(w);
			switch(choice) {
			case KEY_UP:
				hl--;
				hl = (hl < 0) ? firstList.itemsSize-1 : hl;
				break;
			case KEY_DOWN:
				hl++;
				hl = (hl > firstList.itemsSize-1) ? 0 : hl;
				break;
			}
		} else if(currentList == 1) {
			displayList(secondList, hl, w);
			choice = wgetch(w);
			switch(choice) {
			case KEY_UP:
				hl--;
				hl = (hl < 0) ? secondList.itemsSize-1 : hl;
				break;
			case KEY_DOWN:
				hl++;
				hl = (hl > secondList.itemsSize-1) ? 0 : hl;
				break;
			}
		} else if(currentList == 2) {
			if(!directoryInit)
				wclear(w);
				box(w, 0, 0);
				wrefresh(w);
				mvwprintw(w, 1, 1, "Type a directory: ");
			directoryInit = true;
			int newAppend = wgetch(w);
			int dirLen = strlen(directory);
			if(newAppend == 10) 
				break;
			else if(newAppend != KEY_BACKSPACE) {
				directory[dirLen] = newAppend;
				directory[dirLen+1] = '\0';
			} else
				directory[dirLen-1] = '\0';
			wclear(w);
			box(w, 0, 0);
			wrefresh(w);
			mvwprintw(w, 1, 19, directory);
		}

		if(choice == 10) {
			currentList++;
			wclear(w);
			box(w, 0, 0); // sets default borders for the window
			wrefresh(w);
			if(currentList == 1)
				firstChoice = hl;
			if(currentList == 2)
				secondChoice = hl;
			hl = 0;
		} else if(choice == 'q' || choice == 'q')  // Quit
			break;
		
	}

	exitProgram(w);

	printf("%s\n", directory);

	system(directory);

	for(int i = 0; i < firstList.itemsSize; i++)
		free(firstList.items[i]);
	for(int i = 0; i < secondList.itemsSize; i++)
		free(secondList.items[i]);
	free(firstList.items);
	free(secondList.items);

	return 0;
}

void exitProgram(WINDOW *w) {
	delwin(w);
	endwin();
}

void displayList(List list, int hl, WINDOW *w) {
	for(int i = 0; i < list.itemsSize; i++) {
		if(i == hl)
			wattron(w, A_STANDOUT);
		mvwprintw(w, i+1, 1, list.items[i]);
		wattroff(w, A_STANDOUT);
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