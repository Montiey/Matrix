//// This version uses ANSI codes instead of ncurses. Moar portabel!


//#include <math.h>
//#include <stdlib.h>
#include <unistd.h>
#include <algorithm>
#include <sys/ioctl.h>

#define LENGTH 24

#define MININC 200	//x1000
#define MAXINC 800
#define SPACING 3

using namespace std;

struct winsize size;


// Most stuff came from https://www.en.wikipedia.org/wiki/ANSI_escape_code/

#define COLORS 8
const int colors[] = {234, 22, 28, 35, 40, 46, 157, 194};

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
	int c = colors[(int)max(0, min(COLORS, i))];
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

int map(long x, long in_min, long in_max, long out_min, long out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
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
		for(int i = 0; i < LENGTH; i++){
			setFadeShade(map(i, 0, LENGTH-1, COLORS-1, 0));
			setPos(wrap(y-i), x);
			printf("%c", chars[wrap(y-i)]);
		}
		setPos(wrap(y-LENGTH), x);
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
	
	for(int x = 0; x < 1000; x++){
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
	
	
	setF(10);
	setB(0);
	return 0;
}

