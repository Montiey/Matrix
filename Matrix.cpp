#include <unistd.h>
#include <string>
#include <algorithm>
#include <iostream>
#include <stdio.h>



#ifdef _WIN32
#include <winioctl.h>
#endif
#ifdef __APPLE__
#include <sys/ioctl.h>
#endif

#define MININC 50	// 1/1000
#define MAXINC 1000

#define CYCLETIME 50 // milliseconds - increase to save CPU

#define SPACING 3	//increase to save CPU

//comment out to save CPU
//#define HEADSPIN	//Spins the head char every loop, even if not moving down that iteration.

using namespace std;

// Most color stuff came from https://www.en.wikipedia.org/wiki/ANSI_escape_code/

const int colors[] = {194, 194, 157, 157, 46, 46, 40, 40, 35, 35, 35, 28, 28, 28, 22, 22, 22, 22, 234, 234, 234, 234};	//not including head color

const int numColors = sizeof(colors) / sizeof(colors[0]);

#define HEADCOLOR 15
#define BGCOLOR 0

char * chars;
float * increments;
float * sums;
int * ys;

struct winsize size;

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

void setFadeShade(int &i){
	setF(colors[(int)max(0, min(numColors, i))]);
}

void clear(){
	setB(BGCOLOR);
	printf("\x1B[2J");
}

int wrap(int n){
	if(n >= 0){
		return n % size.ws_row;
	}else{
		return size.ws_row - ((-n) % size.ws_row);
	}
}

void printHead(int &y, int &x){
	setF(HEADCOLOR);
	setPos(y, x);
	printf("%c", randChar());
}

void print(int &y, int &x){
	for(int i = 0; i < numColors; i++){
		setFadeShade(i);
		setPos(wrap(y-i), x);
		printf("%c", chars[wrap(y-i) + x * size.ws_row]);
	}
	setPos(wrap(y-numColors), x);
	printf(" ");
	y = (y+1) % size.ws_row;
	
	#ifndef HEADSPIN
	setF(HEADCOLOR);
	setPos(y, x);
	printf("%c", chars[y + x*size.ws_row]);
	#endif
}

int main(int argc, char* argv[]){
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	
	clear();
	
	chars = (char *) calloc(size.ws_row * size.ws_col, sizeof(char));
	increments = (float *) calloc(size.ws_col, sizeof(float));
	sums = (float *) calloc(size.ws_col, sizeof(float));
	ys = (int *) calloc(size.ws_col, sizeof(int));
	
	for(int i = 0; i < size.ws_row * size.ws_col; i++){
		chars[i] = randChar();
	}
	
	for(int i = 0; i < size.ws_col; i++){
		increments[i] = ((rand() % (MAXINC - MININC)) + MININC) / 1000.0;
		sums[i] = 0.0;
		ys[i] = rand() % size.ws_row;
	}
	
	clear();
	
	for(;;){
		for(int i = 0; i < size.ws_col; i++){
			if(i % SPACING == 0){
				sums[i] += increments[i];
				if(sums[i] >= 1){
					sums[i] = 0;
					print(ys[i], i);
				}
				#ifdef HEADSPIN
				printHead(ys[i], i);
				#endif
			}
		}
		fflush(stdout);
		usleep(1000*CYCLETIME);
	}
	
	return 0;
}

