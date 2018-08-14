#include <unistd.h>
#include <string>
#include <algorithm>
#include <iostream>
#include <stdio.h>

#define MININC 100	// 1/1000
#define MAXINC 1000
#define SPACING 4	//increase to save CP
//comment out to save CPU
#define HEADSPIN	//Spins the head char every loop, even if not moving down that iteration.
#define RESIZE_CHECK_INTERVAL 20
using namespace std;

// Most color stuff came from https://www.en.wikipedia.org/wiki/ANSI_escape_code/

const char charTable[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '~', '!', '@', '#', '$', '%', '^', '&', '*', '-', '+', '=', '\\', '/', '<', '>', '?', '[', ']', '{', '}', '(', ')'};

char * chars;
float * increments;
float * sums;
int * ys;
int rows;
int cols;
int loopr = 0;

////////////////

#if defined(unix) || defined(__unix__) || defined (__unix) || defined (__APPLE__)
#include <sys/ioctl.h>
#define OS "MACOS"
const int colors[] = {194, 194, 157, 157, 46, 46, 40, 40, 35, 35, 28, 28, 22, 22, 234, 234};
#define HEADCOLOR 15
#define BGCOLOR 0
struct winsize size;

bool getSize(){
	int oldr = rows;
	int oldc = cols;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    rows = size.ws_row;
    cols = size.ws_col;
	
	return (oldr != rows || oldc != cols);
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

void clear(){
	setB(BGCOLOR);
	printf("\x1B[2J");
}

void waitCustom(long millis){
    usleep(1000 * millis);
}
#endif

////////////////

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#define OS "WIN32"
const int colors[] = {10, 10, 10, 10, 10, 10, 2, 2, 2, 2, 2, 8, 8, 8, 8, 8};
#define HEADCOLOR 15
#define BGCOLOR 0
HWND console = GetConsoleWindow();
RECT r;

bool getSize(){
    RECT old = r;
    GetWindowRect(console, &r);
    rows = (r.bottom - r.top) / 16 - 3;	//approximate the max number of characters given the size of the window
    cols = (r.right - r.left) / 8 - 5;
	
	return (old.bottom != r.bottom || old.top != r.top || old.right != r.right || old.left != r.left);
}

void setF(int i){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i);
}
void setB(int i){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i);
}

void setPos(int y, int x){
	COORD pos = {(short)x, (short)y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void clear(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BGCOLOR);
	system("CLS");
}

void waitCustom(long millis){
    Sleep(millis);
}
#endif

const int numColors = sizeof(colors) / sizeof(colors[0]);

////////////////

char randChar(){
	return charTable[rand() % sizeof(charTable)];
}

void setFadeShade(int &i){
	setF(colors[(int)max(0, min(numColors, i))]);
}

int wrap(int n){
	if(n >= 0){
		return n % rows;
	}else{
		return rows - ((-n) % rows);
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
		printf("%c", chars[wrap(y-i) + x * rows]);
	}
	setPos(wrap(y-numColors), x);
	printf(" ");
	y = (y+1) % rows;
	
	#ifndef HEADSPIN
	setF(HEADCOLOR);
	setPos(y, x);
	printf("%c", chars[y + x*rows]);
	#endif
}

void resize(){
    free(chars);
    free(increments);
    free(sums);
    free(ys);
    
	chars = (char *) calloc(rows * cols, sizeof(char));
	increments = (float *) calloc(cols, sizeof(float));
	sums = (float *) calloc(cols, sizeof(float));
	ys = (int *) calloc(cols, sizeof(int));
    
	for(int i = 0; i < rows * cols; i++){
		chars[i] = randChar();
	}
	
	for(int i = 0; i < cols; i++){
		increments[i] = ((rand() % (MAXINC - MININC)) + MININC) / 1000.0;
		sums[i] = 0.0;
		ys[i] = rand() % rows;
	}
}

////////////////

int main(int argc, char* argv[]){

	getSize();
	resize();
	
    /*cout << "OS: " << OS;
    cout << rows << "R x " << cols << "C";
    fflush(stdout);
    wait(3000);*/
	
	clear();
	
	for(;;){
		for(int i = 0; i < cols; i++){
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
		
		if(loopr >= RESIZE_CHECK_INTERVAL){
			if(getSize()){
				resize();
				clear();
			}
			loopr = 0;
		}
		loopr++;
		
		waitCustom(40);
		
	}
	
	cout << (int)loopr;
	
	return 0;
}

