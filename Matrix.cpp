#include <ncurses.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#define FADE 12
#define SPACING 2
const float yRate = 400;//random rate at which to move to the next line (x/1000)

using namespace std;

int ySize = 0;
int xSize = 0;

int currentCol = 0;

char randChar(){
	return rand() % 93 + 33;
}

////////

void printOne(int col, int a[]){
	
	
	if(rand()%1000 < yRate){
		a[col]++;
		attron(COLOR_PAIR(2));
		mvwprintw(stdscr, a[col]-1, col, "%c", randChar());
		attroff(COLOR_PAIR(2));
	}
	
	if(a[col] >= FADE){
		mvwprintw(stdscr, a[col] - FADE, col, "%c", ' ');
	} else{
		mvwprintw(stdscr, ySize - (FADE - a[col]), col, "%c", ' ');
	}
	
	if(a[col] > ySize){
		a[col] = 0;
	}
	
	
	
	attron(COLOR_PAIR(1));
	mvwprintw(stdscr, a[col], col, "%c", randChar());
	attroff(COLOR_PAIR(1));
}

////////

int main(int argc, char* argv[]){
	
	getch();
	
	initscr();
	noecho();
	
	start_color();
	
	getmaxyx(stdscr, ySize, xSize);
	
	int * arr = (int *) calloc(xSize, sizeof(int));
	
	if(!can_change_color()) return -1;
	if(!has_colors()) return -1;
	
	init_color(COLOR_WHITE, 1000, 1000, 1000);
	init_color(COLOR_GREEN, 1000, 1000, 0);
	
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	
	for(int i = 0; i < xSize; i++){
		if(i%SPACING == 0){
			arr[i] = rand()%ySize;
		}
	}
	
	while(1){
		for(int i = 0; i < xSize; i++){
			if(i%SPACING == 0){
				printOne(i, arr);
			}
		}
		refresh();
		usleep(1000 * 100);
	}
	
	getch();
	endwin();
	
	
	
	
	return 0;
}