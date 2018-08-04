//// This version uses ANSI codes instead of ncurses. Moar portabel!


//#include <math.h>
//#include <stdlib.h>
#include <unistd.h>
#include <algorithm>
#include <sys/ioctl.h>

#define MININC 200	//1/1000
#define MAXINC 800
#define SPACING 3

using namespace std;

struct winsize size;


// Most stuff came from https://www.en.wikipedia.org/wiki/ANSI_escape_code/

const int colors[] = {194, 194, 157, 157, 46, 46, 40, 40, 35, 35, 35, 28, 28, 28, 22, 22, 22, 22, 234, 234, 234, 234};

const int numColors = sizeof(colors) / sizeof(colors[0]);

#define HEADCOLOR 15
#define BGCOLOR 0

char randChar(){
	return rand() % 93 + 33;
}

void setF(int i){
	printf("\x1B[38;5;%dm", i);
}

void setB(int i){
	printf("\x1B[48;5;%dm", i);
}

void setPos(int y, int x){
	printf("\x1B[%d;%df", y, x);
}

void setFadeShade(int i){
	int c = colors[(int)max(0, min(numColors, i))];
	setF(c);
}

void clear(){
	setB(BGCOLOR);
	printf("\x1B[2J");
}

int wrap(int n){
	if(n >= 0){
		return n % size.ws_row;	//TODO: loop n over the screen size
	} else{
		return size.ws_row - ((-n) % size.ws_row);
	}
}

class Drop{
	public:
	int y;
	int x;
	float increment;
	float sum;
	
	char * chars;
	
	Drop(){
		x = 0;
		increment = ((rand() % (MAXINC - MININC)) + MININC) / 1000.0;
		y = rand() % size.ws_row;
		sum = 0;
		chars = (char *) calloc(size.ws_row, sizeof(char));
		for(int i = 0; i < size.ws_row; i++){
			chars[i] = randChar();
		}
	}
	
	void print(){
		for(int i = 0; i < numColors; i++){
			setFadeShade(i);
			setPos(wrap(y-i), x);
			printf("%c", chars[wrap(y-i)]);
		}
		setPos(wrap(y-numColors), x);
		printf(" ");
		y = (y+1) % size.ws_row;
	}
	
	void printHead(){
		setF(HEADCOLOR);
		setPos(y, x);
		printf("%c", randChar());
	}
};

int main(int argc, char* argv[]){
	ioctl(STDOUT_FILENO,TIOCGWINSZ,&size);
	Drop * arr = new Drop[size.ws_col];
	
	for(int i = 0; i < size.ws_col; i++){
		arr[i].x = i;
	}
	
	clear();
	
	for(int i = 0; i < size.ws_col; i++){	//initial
		if(i % SPACING == 0){
			arr[i].sum += arr[i].increment;
			arr[i].print();
		}
	}
	
	for(;;){
		for(int i = 0; i < size.ws_col; i++){
			if(i % SPACING == 0){
				if(arr[i].sum >= 1){
					arr[i].sum = 0;
					arr[i].print();
				}
				arr[i].sum += arr[i].increment;
				arr[i].printHead();
			}
		}
		fflush(stdout);
		usleep(1000*100);
	}
	
	return 0;
}

