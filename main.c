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

	char *rawFirst[] = {"Random", "Multiple comics", "Specific comic", "Newest comic"};
	char *rawSecond[] = {"Change directory", "Same directory", "Cancel/Exit"};
	List firstList = initList(4, rawFirst);
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

	char *multiple2 = malloc(25), *multiple1 = malloc(25); // For multiple comics
	char directory[100]; // For custom directory

	bool directoryInit = false, multipleInit = false; // Flags for window inits

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
			if(secondChoice != 0) {
				currentList++;
				continue;
			}
			if(!directoryInit) {
				wclear(w);
				// Free the menus
				for(int i = 0; i < firstList.itemsSize; i++)
					free(firstList.items[i]);
				for(int i = 0; i < secondList.itemsSize; i++)
					free(secondList.items[i]);
				free(firstList.items);
				free(secondList.items);
			}
			box(w, 0, 0);
			mvwprintw(w, 1, 1, "Type a directory: ");
			directoryInit = true;
			int newAppend = wgetch(w);
			int dirLen = strlen(directory);
			if(newAppend == 10) 
				if(firstChoice != 1)
					break;
				else
					currentList++;
			else if(newAppend != KEY_BACKSPACE) {
				directory[dirLen] = newAppend;
				directory[dirLen+1] = '\0';
			} else
				directory[dirLen-1] = '\0';
			wclear(w);
			wrefresh(w);
			mvwprintw(w, 1, 19, directory);
		} else if(currentList == 3) {
			if(firstChoice != 1)
				break;
			if(!multipleInit)
				wclear(w);
			box(w, 0, 0);
			mvwprintw(w, 1, 1, "Type starting comic:");
			multipleInit = true;
			int newAppend = wgetch(w);
			int multiple1Len = strlen(multiple1);
			if(newAppend == 10) {
				currentList++;
				multipleInit = false;
			} else if(newAppend != KEY_BACKSPACE) {
				multiple1[multiple1Len] = newAppend;
				multiple1[multiple1Len+1] = '\0';
			} else
				multiple1[multiple1Len-1] = '\0';
			wclear(w);
			wrefresh(w);
			mvwprintw(w, 1, 21, multiple1);
		} else if(currentList == 4) {
			if(!multipleInit)
				wclear(w);
			box(w, 0, 0);
			mvwprintw(w, 1, 1, "Type end comic:");
			multipleInit = true;
			int newAppend = wgetch(w);
			int multiple2Len = strlen(multiple2);
			if(newAppend == 10) {
				break;
			} else if(newAppend != KEY_BACKSPACE) {
				multiple2[multiple2Len] = newAppend;
				multiple2[multiple2Len+1] = '\0';
			} else
				multiple2[multiple2Len-1] = '\0';
			wclear(w);
			wrefresh(w);
			mvwprintw(w, 1, 16, multiple2);
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

	char commandToRun[40];

	switch(secondChoice) {
	case(0):
		strcpy(commandToRun, "python3 script/main.py -d ");
		strcat(commandToRun, directory);
		break;
	case(1):
		strcpy(commandToRun, "python3 script/main.py");
		break;
	default:
		break;
	}

	switch(firstChoice) {
	case(0):
		strcat(commandToRun, " -r");
		break;
	case(1):
		break;
	}

	system(commandToRun);

	free(multiple1);
	free(multiple2);

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