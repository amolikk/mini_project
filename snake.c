#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "snake.h"



#ifdef _WIN32
	
	#define UP_ARROW 72
	#define LEFT_ARROW 75
	#define RIGHT_ARROW 77
	#define DOWN_ARROW 80
	
	#define ENTER_KEY 13
	
	const char SNAKE_HEAD = (char)177;
	const char SNAKE_BODY = (char)178;
	const char WALL = (char)219;	
	const char FOOD = (char)254;
	const char BLANK = ' ';
#else
	
	#include <stdlib.h>
	#include <termios.h>
	#include <unistd.h>
	#include <fcntl.h>
	
	 
	#define UP_ARROW (char)'A' 
	#define LEFT_ARROW (char)'D'
	#define RIGHT_ARROW (char)'C'
	#define DOWN_ARROW (char)'B'

	#define ENTER_KEY 10
	
	const char SNAKE_HEAD = 'X';
	const char SNAKE_BODY = '#';
	const char WALL = '#';	
	const char FOOD = '*';
	const char BLANK = ' ';

	
	

void gotoxy(int x,int y) {
	printf("%c[%d;%df",0x1B,y,x);
}
int kbhit(void);
char getch();
void clrscr();
char waitForAnyKey(void);
int getGameSpeed(void);
void pauseMenu(void);
int checkKeysPressed(int direction);
int collisionSnake (int x, int y, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int detect);
int generateFood(int foodXY[], int width, int height, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength);
void moveSnakeArray(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int direction);
void move(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int direction);
int eatFood(int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[]);
int collisionDetection(int snakeXY[][SNAKE_ARRAY_SIZE], int consoleWidth, int consoleHeight, int snakeLength );
void refreshInfoBar(int score, int speed) {
	gotoxy(5,23);
	printf("Score: %d", score);
	
	gotoxy(5,24);
	printf("Speed: %d", speed);

	gotoxy(52,23);
	printf("Coder: Amoli Kotalwar");

}
void createHighScores(void);
int getLowestScore();
void inputScore(int score);
void displayHighScores(void);
void youWinScreen(void) {
	int x = 6, y = 7;
	gotoxy(x,y++);
	printf("'##:::'##::'#######::'##::::'##::::'##:::::'##:'####:'##::: ##:'####:");
	gotoxy(x,y++);
	printf(". ##:'##::'##.... ##: ##:::: ##:::: ##:'##: ##:. ##:: ###:: ##: ####:");
	gotoxy(x,y++);
	printf(":. ####::: ##:::: ##: ##:::: ##:::: ##: ##: ##:: ##:: ####: ##: ####:");
	gotoxy(x,y++);
	printf("::. ##:::: ##:::: ##: ##:::: ##:::: ##: ##: ##:: ##:: ## ## ##:: ##::");
	gotoxy(x,y++);
	printf("::: ##:::: ##:::: ##: ##:::: ##:::: ##: ##: ##:: ##:: ##. ####::..:::");
	gotoxy(x,y++);
	printf("::: ##:::: ##:::: ##: ##:::: ##:::: ##: ##: ##:: ##:: ##:. ###:'####:");
	gotoxy(x,y++);
	printf("::: ##::::. #######::. #######:::::. ###. ###::'####: ##::. ##: ####:");
	gotoxy(x,y++);
	printf(":::..::::::.......::::.......:::::::...::...:::....::..::::..::....::");
	gotoxy(x,y++);	
	
	waitForAnyKey();
	clrscr(); /*clear the console*/
	return;
}
void gameOverScreen(void) {
	int x = 17, y = 3;
	
	
	
	gotoxy(x,y++);
	printf(":'######::::::'###::::'##::::'##:'########:\n");
	gotoxy(x,y++);
	printf("'##... ##::::'## ##::: ###::'###: ##.....::\n");
	gotoxy(x,y++);
	printf(" ##:::..::::'##:. ##:: ####'####: ##:::::::\n");
	gotoxy(x,y++);
	printf(" ##::'####:'##:::. ##: ## ### ##: ######:::\n");
	gotoxy(x,y++);
	printf(" ##::: ##:: #########: ##. #: ##: ##...::::\n");
	gotoxy(x,y++);
	printf(" ##::: ##:: ##.... ##: ##:.:: ##: ##:::::::\n");
	gotoxy(x,y++);
	printf(". ######::: ##:::: ##: ##:::: ##: ########:\n");
	gotoxy(x,y++);
	printf(":......::::..:::::..::..:::::..::........::\n");
	gotoxy(x,y++);
	printf(":'#######::'##::::'##:'########:'########::'####:\n");
	gotoxy(x,y++);
	printf("'##.... ##: ##:::: ##: ##.....:: ##.... ##: ####:\n");
	gotoxy(x,y++);
	printf(" ##:::: ##: ##:::: ##: ##::::::: ##:::: ##: ####:\n");
	gotoxy(x,y++);
	printf(" ##:::: ##: ##:::: ##: ######::: ########::: ##::\n");
	gotoxy(x,y++);
	printf(" ##:::: ##:. ##:: ##:: ##...:::: ##.. ##::::..:::\n");
	gotoxy(x,y++);
	printf(" ##:::: ##::. ## ##::: ##::::::: ##::. ##::'####:\n");
	gotoxy(x,y++);
	printf(". #######::::. ###:::: ########: ##:::. ##: ####:\n");
	gotoxy(x,y++);
	printf(":.......::::::...:::::........::..:::::..::....::\n");
		waitForAnyKey();
	clrscr(); /*clear the console*/
	return;
}
void startGame( int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[], int consoleWidth, int consoleHeight, int snakeLength, int direction, int score, int speed);
void loadEnviroment(int consoleWidth, int consoleHeight) {
	int i;
	int x = 1, y = 1;
	int rectangleHeight = consoleHeight - 4;
	clrscr(); /*clear the console*/
	
	gotoxy(x,y); /*Top left corner*/
	
	for (; y < rectangleHeight; y++)
	{
		gotoxy(x, y);  
		printf("%c",WALL);
		
		gotoxy(consoleWidth, y); 
		printf("%c",WALL);
	}
	
	y = 1;
	for (; x < consoleWidth + 1; x++)
	{
		gotoxy(x, y);  
		printf("%c",WALL);
		
		gotoxy(x, rectangleHeight); 
		printf("%c",WALL);
	}
	x = (consoleWidth+1)/3;
	for (y = (rectangleHeight) / 3; y < (2 * rectangleHeight) / 3; y++)
	{
		gotoxy(x, y);  
		printf("%c",WALL);
		
		gotoxy((consoleWidth) / 3, y); 
		printf("%c",WALL);
	}
	y = (rectangleHeight) / 3;
	
	for (x = (consoleWidth + 1) / 3; x < (2 * consoleWidth + 1) / 3; x++)
	{
		gotoxy(x, y);  
		printf("%c",WALL);
		
		gotoxy(x, (rectangleHeight) / 3); 
		printf("%c",WALL);
	}
	return;
}
void loadSnake(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength) {
	int i;
	for (i = 0; i < snakeLength; i++)
	{
		gotoxy(snakeXY[0][i], snakeXY[1][i]);
		printf("%c", SNAKE_BODY); 
	}
	
	return;
}
void prepairSnakeArray(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength) {
	int i, x;
	int snakeX = snakeXY[0][0];
	int snakeY = snakeXY[1][0];
	for(i = 1; i <= snakeLength; i++)
	{
		snakeXY[0][i] = snakeX + i;
		snakeXY[1][i] = snakeY;
	}
	
	return;
}
void loadGame(void) {
	int snakeXY[2][SNAKE_ARRAY_SIZE];   /*two dimensional array*/
	int snakeLength = 4;		    /*initial lenght*/
	int direction = LEFT_ARROW;	    /*initial direction*/
	int foodXY[] = {5,5};
	int score = 0;
	int consoleWidth = 80;
	int consoleHeight = 25;
	int speed = getGameSpeed();
	snakeXY[0][0] = 40;     /*starting position of the snake*/
	snakeXY[1][0] = 10;
	
	loadEnviroment(consoleWidth, consoleHeight); /*borders*/
	prepairSnakeArray(snakeXY, snakeLength);
	loadSnake(snakeXY, snakeLength);
	generateFood( foodXY, consoleWidth, consoleHeight, snakeXY, snakeLength);
	refreshInfoBar(score, speed); /*Bottom info bar. Score, Level etc*/
	startGame(snakeXY, foodXY, consoleWidth, consoleHeight, snakeLength, direction, score, speed);

	return;
}
int menuSelector(int x, int y, int yStart);
void welcomeArt(void) {
	clrscr(); 
	
	printf("\n");	
	printf("\t\t    _________         _________ 			\n");	
	printf("\t\t   /         \\       /         \\ 			\n");	
	printf("\t\t  /  /~~~~~\\  \\     /  /~~~~~\\  \\ 			\n");	
	printf("\t\t  |  |     |  |     |  |     |  | 			\n");		
	printf("\t\t  |  |     |  |     |  |     |  | 			\n");
	printf("\t\t  |  |     |  |     |  |     |  |         /	\n");
	printf("\t\t  |  |     |  |     |  |     |  |       //	\n");
	printf("\t\t (o  o)    \\  \\_____/  /     \\  \\_____/ / 	\n");
	printf("\t\t  \\__/      \\         /       \\        / 	\n");
	printf("\t\t    |        ~~~~~~~~~         ~~~~~~~~ 		\n");
	printf("\t\t    ^											\n");
	printf("\t		Welcome To The Snake Game!			\n");
	printf("\t			    Press Any Key To Continue...	\n");			
	printf("\n");
	
	waitForAnyKey();
	return;
}

}
void controls(void);
void exitYN(void) {
	char pressed;
	gotoxy(9,8);
	printf("Are you sure you want to exit(Y/N)\n");
	if (pressed == 'y')
	{
		clrscr(); /*clear the console*/
		exit(1);
	}
	return;
}
int mainMenu(void);


