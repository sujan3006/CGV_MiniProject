#include <stdlib.h>
#include <vector>
#include <deque>
#include <windows.h>
#include <GL/glut.h>
#include <iostream>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

static bool replay = false;
static bool over = true;
static float squareSize = 50.0;
static float xIncrement = 0;
static float yIncrement = 0;
static int rotation = 0;
float* monster1 = new float[3] {10.5, 8.5, 1.0};
float* monster2 = new float[3] {13.5, 1.5, 2.0};
float* monster3 = new float[3] {4.5, 6.5, 3.0};
float* monster4 = new float[3] {2.5, 13.5, 4.0};
static vector<int> border = { 0, 0, 15, 1, 15, 15, 14, 1, 0, 14, 15, 15, 1, 14, 0, 0 };


static vector<int> obstaclesTop = { 2, 2, 3, 6, 3, 6, 4, 5, 4, 2, 5, 4, 5, 3, 6, 5, 6, 1, 9, 2, 7, 2, 8, 5, 9, 5, 10, 3, 10, 4, 11, 2, 11, 5, 12, 6, 12, 6, 13, 2 };
static vector<int> obstaclesMiddle = { 2, 9, 3, 7, 3, 7, 4, 8, 4, 9, 5, 11, 5, 6, 6, 10, 6, 10, 7, 8, 7, 8, 8, 9, 6, 7, 7, 6, 8, 6, 9, 7, 10, 6, 9, 10, 9, 10, 8, 8, 11, 9, 10, 11, 11, 8, 12, 7, 12, 7, 13, 9 };
static vector<int> obstaclesBottom = { 2, 10, 3, 13, 3, 13, 4, 12, 5, 12, 6, 13, 6, 13, 7, 11, 8, 11, 9, 13, 9, 13, 10, 12, 11, 12, 12, 13, 12, 13, 13, 10 };
static deque<float> food = { 1.5, 1.5, 1.5, 2.5, 1.5, 3.5, 1.5, 4.5, 1.5, 5.5, 1.5, 6.5, 1.5, 7.5, 1.5, 8.5, 1.5, 9.5, 1.5, 10.5, 1.5, 11.5, 1.5, 12.5, 1.5, 13.5, 2.5, 1.5, 2.5, 6.5, 2.5, 9.5, 2.5, 13.5, 3.5, 1.5, 3.5, 2.5, 3.5, 3.5, 3.5, 4.5, 3.5, 6.5, 3.5, 8.5, 3.5, 9.5, 3.5, 10.5, 3.5, 11.5, 3.5, 13.5, 4.5, 1.5, 4.5, 4.5, 4.5, 5.5, 4.5, 6.5, 4.5, 7.5, 4.5, 8.5, 4.5, 11.5, 4.5, 12.5, 4.5, 13.5, 5.5, 1.5, 5.5, 2.5, 5.5, 5.5, 5.5, 10.5, 5.5, 11.5, 5.5, 13.5, 6.5, 2.5, 6.5, 3.5, 6.5, 4.5, 6.5, 5.5, 6.5, 7.5, 6.5, 10.5, 6.5, 13.5, 7.5, 5.5, 7.5, 6.5, 7.5, 7.5, 7.5, 9.5, 7.5, 10.5, 7.5, 11.5, 7.5, 12.5, 7.5, 13.5, 8.5, 2.5, 8.5, 3.5, 8.5, 4.5, 8.5, 5.5, 8.5, 7.5, 8.5, 10.5, 8.5, 13.5, 9.5, 1.5, 9.5, 2.5, 9.5, 5.5, 9.5, 10.5, 9.5, 11.5, 9.5, 13.5, 10.5, 1.5, 10.5, 4.5, 10.5, 5.5, 10.5, 6.5, 10.5, 7.5, 10.5, 8.5, 10.5, 11.5, 10.5, 12.5, 10.5, 13.5, 11.5, 1.5, 11.5, 2.5, 11.5, 3.5, 11.5, 4.5, 11.5, 5.5, 11.5, 6.5, 11.5, 8.5, 11.5, 9.5, 11.5, 10.5, 11.5, 11.5, 11.5, 13.5, 12.5, 1.5, 12.5, 6.5, 12.5, 9.5, 12.5, 13.5, 13.5, 1.5, 13.5, 2.5, 13.5, 3.5, 13.5, 4.5, 13.5, 5.5, 13.5, 6.5, 13.5, 7.5, 13.5, 8.5, 13.5, 9.5, 13.5, 10.5, 13.5, 11.5, 13.5, 12.5, 13.5, 13.5 };
static vector<vector<bool>> bitmap; // 2d image of which squares are blocked and which are clear for pacman to move in
bool* keyStates = new bool[256];
int points = 0;


void init(void){

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);

	for (int i = 0; i < 256; i++){
		keyStates[i] = false;
	}

	bitmap.push_back({ true, true, true, true, true, true, true, true, true, true, true, true, true, true, true });
	bitmap.push_back({ true, false, false, false, false, false, false, false, false, false, false, false, false, false, true });
	bitmap.push_back({ true, false, true, true, true, true, false, true, true, false, true, true, true, false, true });
	bitmap.push_back({ true, false, false, false, false, true, false, true, false, false, false, false, true, false, true});
	bitmap.push_back({ true, false, true, true, false, false, false, false, false, true, true, false, false, false, true});
	bitmap.push_back({ true, false, false, true, true, false, true, true, true, true, false, false, true, false, true});
	bitmap.push_back({ true, true, false, false, false, false, true, false, true, true, false, true, true, false, true});
	bitmap.push_back({ true, true, true, true, true, false, false, false, true, false, false, false, false, false, true});
	bitmap.push_back({ true, true, false, false, false, false, true, false, true, true, false, true, true, false, true });
	bitmap.push_back({ true, false, false, true, true, false, true, true, true, true, false, false, true, false, true });
	bitmap.push_back({ true, false, true, true, false, false, false, false, false, true, true, false, false, false, true });
	bitmap.push_back({ true, false, false, false, false, true, false, true, false, false, false, false, true, false, true });
	bitmap.push_back({ true, false, true, true, true, true, false, true, true, false, true, true, true, false, true });
	bitmap.push_back({ true, false, false, false, false, false, false, false, false, false, false, false, false, false, true });
	bitmap.push_back({ true, true, true, true, true, true, true, true, true, true, true, true, true, true, true });
}


void drawLaberynth(){
	glColor3f(1.0, 1.0, 1.0);

	for (int i = 0; i < border.size(); i = i + 4){
		glRectf(border.at(i) * squareSize, border.at(i + 1)*squareSize, border.at(i + 2)*squareSize, border.at(i + 3)*squareSize);
	}


	for (int j = 0; j < obstaclesBottom.size(); j = j + 4){
		glRectf(obstaclesBottom.at(j) * squareSize, obstaclesBottom.at(j + 1)*squareSize, obstaclesBottom.at(j + 2)*squareSize, obstaclesBottom.at(j + 3)*squareSize);
	}
	for (int k = 0; k < obstaclesMiddle.size(); k = k + 4){
		glRectf(obstaclesMiddle.at(k) * squareSize, obstaclesMiddle.at(k + 1)*squareSize, obstaclesMiddle.at(k + 2)*squareSize, obstaclesMiddle.at(k + 3)*squareSize);
	}
	for (int p = 0; p < obstaclesTop.size(); p = p + 4){
		glRectf(obstaclesTop.at(p) * squareSize, obstaclesTop.at(p + 1)*squareSize, obstaclesTop.at(p + 2)*squareSize, obstaclesTop.at(p + 3)*squareSize);
	}
}


bool foodEaten(int x, int y, float pacmanX, float pacmanY){
	if (x >= pacmanX - 16.0 *cos(359 * M_PI / 180.0) && x <= pacmanX + 16.0*cos(359 * M_PI / 180.0)){
		if (y >= pacmanY - 16.0*cos(359 * M_PI / 180.0) && y <= pacmanY + 16.0*cos(359 * M_PI / 180.0)){
			return true;
		}
	}
	return false;
}


void drawFood(float pacmanX, float pacmanY){
	deque<float> temp;

	for (int i = 0; i < food.size(); i = i + 2){
		if (!foodEaten(food.at(i)*squareSize, food.at(i + 1)*squareSize, pacmanX, pacmanY)){
			temp.push_back(food.at(i));
			temp.push_back(food.at(i + 1));
		}
		else {
			points++;
		}
	}
	food.swap(temp);
	glPointSize(5.0);
	glBegin(GL_POINTS);
	glColor3f(1.0, 1.0, 1.0);

	for (int j = 0; j < food.size(); j = j + 2){
		glVertex2f(food.at(j)*squareSize, food.at(j + 1)*squareSize);
	}
	glEnd();
}


void drawPacman(float positionX, float positionY, float rotation){
	int x, y;
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 0.0);
	for (int k = 0; k < 32; k++){
		x = (float)k / 2.0 * cos((30 + 90*rotation) * M_PI / 180.0) + (positionX*squareSize);
		y = (float)k / 2.0* sin((30 + 90 * rotation) * M_PI / 180.0) + (positionY*squareSize);
		for (int i = 30; i < 330; i++){
			glVertex2f(x, y);
			x = (float)k / 2.0 * cos((i + 90 * rotation) * M_PI / 180.0) + (positionX*squareSize);
			y = (float)k / 2.0* sin((i + 90 * rotation) * M_PI / 180.0) + (positionY*squareSize);
			glVertex2f(x, y);
		}
	}
	glEnd();
}


void drawMonster(float positionX, float positionY, float r, float g, float b){
	int x, y;
	glBegin(GL_LINES);
	glColor3f(r, g, b);

	for (int k = 0; k < 32; k++){
		x = (float)k / 2.0 * cos(360 * M_PI / 180.0) + (positionX*squareSize);
		y = (float)k / 2.0* sin(360 * M_PI / 180.0) + (positionY*squareSize);
		for (int i = 180; i <= 360; i++){
			glVertex2f(x, y);
			x = (float)k / 2.0 * cos(i * M_PI / 180.0) + (positionX*squareSize);
			y = (float)k / 2.0* sin(i * M_PI / 180.0) + (positionY*squareSize);
			glVertex2f(x, y);
		}
	}
	glEnd();

	glRectf((positionX*squareSize) - 17, positionY*squareSize, (positionX*squareSize) + 15, (positionY*squareSize) + 15);
	glBegin(GL_POINTS);
	glColor3f(0, 0.2, 0.4);


	glVertex2f((positionX*squareSize) - 11, (positionY*squareSize) + 14);
	glVertex2f((positionX*squareSize) - 1, (positionY*squareSize) + 14);
	glVertex2f((positionX*squareSize) + 8, (positionY*squareSize) + 14);
	glVertex2f((positionX*squareSize) + 4, (positionY*squareSize) - 3);
	glVertex2f((positionX*squareSize) - 7, (positionY*squareSize) - 3);
	glEnd();
}

//Method to update the position of the monsters randomly
void updateMonster(float* monster, int id){
		//find the current position of the monster
		int x1Quadrant = (int)((monster[0] - (2/squareSize)) - (16.0 *cos(360 * M_PI / 180.0)) / squareSize);
		int x2Quadrant = (int)((monster[0] + (2/squareSize)) + (16.0 *cos(360 * M_PI / 180.0)) / squareSize);
		int y1Quadrant = (int)((monster[1] - (2/squareSize)) - (16.0 *cos(360 * M_PI / 180.0)) / squareSize);
		int y2Quadrant = (int)((monster[1] + (2/squareSize)) + (16.0 *cos(360 * M_PI / 180.0)) / squareSize);
		//move him acording to its direction until he hits an obstacle
		switch ((int)monster[2]){
		case 1:
			if (!bitmap.at(x1Quadrant).at((int)monster[1])){
				monster[0] -= 2 / squareSize;
			}else {
				int current = monster[2];
				do{
					monster[2] =  (rand() % 4) + 1;
				} while (current == (int) monster[2]);
			}
			break;
		case 2:
			if (!bitmap.at(x2Quadrant).at((int)monster[1])){
				monster[0] += 2 / squareSize;
			}
			else {
				int current = monster[2];
				do{
					monster[2] = (rand() % 4) + 1;
				} while (current == (int)monster[2]);
			}
			break;
		case 3:
			if (!bitmap.at((int)monster[0]).at(y1Quadrant)){
				monster[1] -= 2 / squareSize;
			}
			else {
				int current = monster[2];
				do{
					monster[2] = (rand() % 4) + 1;
				} while (current == (int)monster[2]);
			}
			break;
		case 4:
			if (!bitmap.at((int)monster[0]).at(y2Quadrant)){
				monster[1] += 2 / squareSize;
			}
			else {
				int current = monster[2];
				do{
					monster[2] = (rand() % 4) + 1;
				} while (current == (int)monster[2]);
			}
			break;
		default:
			break;
		}
}

//Method to set the pressed key
void keyPressed(unsigned char key, int x, int y){
	keyStates[key] = true;
}

//Method to unset the released key
void keyUp(unsigned char key, int x, int y){
	keyStates[key] = false;
}

//Method to reset all the variable necessaries to start the game again
void resetGame(){
	over = false;
	xIncrement = 0;
	yIncrement = 0;
	rotation = 0;
	monster1 = new float[3] {10.5, 8.5, 1.0};
	monster2 = new float[3] {13.5, 1.5, 2.0};
	monster3 = new float[3] {4.5, 6.5, 3.0};
	monster4 = new float[3] {2.5, 13.5, 4.0};
	points = 0;
	for (int i = 0; i < 256; i++){
		keyStates[i] = false;
	}
	food = { 1.5, 1.5, 1.5, 2.5, 1.5, 3.5, 1.5, 4.5, 1.5, 5.5, 1.5, 6.5, 1.5, 7.5, 1.5, 8.5, 1.5, 9.5, 1.5, 10.5, 1.5, 11.5, 1.5, 12.5, 1.5, 13.5, 2.5, 1.5, 2.5, 6.5, 2.5, 9.5, 2.5, 13.5, 3.5, 1.5, 3.5, 2.5, 3.5, 3.5, 3.5, 4.5, 3.5, 6.5, 3.5, 8.5, 3.5, 9.5, 3.5, 10.5, 3.5, 11.5, 3.5, 13.5, 4.5, 1.5, 4.5, 4.5, 4.5, 5.5, 4.5, 6.5, 4.5, 7.5, 4.5, 8.5, 4.5, 11.5, 4.5, 12.5, 4.5, 13.5, 5.5, 1.5, 5.5, 2.5, 5.5, 5.5, 5.5, 10.5, 5.5, 11.5, 5.5, 13.5, 6.5, 2.5, 6.5, 3.5, 6.5, 4.5, 6.5, 5.5, 6.5, 7.5, 6.5, 10.5, 6.5, 13.5, 7.5, 5.5, 7.5, 6.5, 7.5, 7.5, 7.5, 9.5, 7.5, 10.5, 7.5, 11.5, 7.5, 12.5, 7.5, 13.5, 8.5, 2.5, 8.5, 3.5, 8.5, 4.5, 8.5, 5.5, 8.5, 7.5, 8.5, 10.5, 8.5, 13.5, 9.5, 1.5, 9.5, 2.5, 9.5, 5.5, 9.5, 10.5, 9.5, 11.5, 9.5, 13.5, 10.5, 1.5, 10.5, 4.5, 10.5, 5.5, 10.5, 6.5, 10.5, 7.5, 10.5, 8.5, 10.5, 11.5, 10.5, 12.5, 10.5, 13.5, 11.5, 1.5, 11.5, 2.5, 11.5, 3.5, 11.5, 4.5, 11.5, 5.5, 11.5, 6.5, 11.5, 8.5, 11.5, 9.5, 11.5, 10.5, 11.5, 11.5, 11.5, 13.5, 12.5, 1.5, 12.5, 6.5, 12.5, 9.5, 12.5, 13.5, 13.5, 1.5, 13.5, 2.5, 13.5, 3.5, 13.5, 4.5, 13.5, 5.5, 13.5, 6.5, 13.5, 7.5, 13.5, 8.5, 13.5, 9.5, 13.5, 10.5, 13.5, 11.5, 13.5, 12.5, 13.5, 13.5 };
}

//Method to update the movement of the pacman according to the movement keys pressed
void keyOperations(){
	//get current position
	float  x = (1.5 + xIncrement) * squareSize;
	float y = (1.5 + yIncrement) * squareSize;
	//update according to keys pressed
	if (keyStates['a']){
		x -= 2;
		int x1Quadrant = (int)((x - 16.0 *cos(360 * M_PI / 180.0)) / squareSize);
		if (!bitmap.at(x1Quadrant).at((int)y/squareSize)){
			xIncrement -= 2 / squareSize;
			rotation = 2;
		}
	}
	if (keyStates['d']){
		x += 2;
		int x2Quadrant = (int)((x + 16.0 *cos(360 * M_PI / 180.0)) / squareSize);
		if (!bitmap.at(x2Quadrant).at((int)y / squareSize)){
			xIncrement += 2 / squareSize;
			rotation = 0;
		}
	}
	if (keyStates['w']){
		y -= 2;
		int y1Quadrant = (int)((y - 16.0 *cos(360 * M_PI / 180.0)) / squareSize);
		if (!bitmap.at((int)x/squareSize).at(y1Quadrant)){
			yIncrement -= 2 / squareSize;
			rotation = 3;
		}
	}
	if (keyStates['s']){
		y += 2;
		int y2Quadrant = (int)((y + 16.0 *cos(360 * M_PI / 180.0)) / squareSize);
		if (!bitmap.at((int)x / squareSize).at(y2Quadrant)){
			yIncrement += 2 / squareSize;
			rotation = 1;
		}
	}
	if (keyStates[' ']){
		if (!replay && over){
			resetGame();
			replay = true;
		}
		else if (replay && over){
			replay = false;
		}
	}
}

//Method to check if the game is over
void gameOver(){
	int pacmanX = (int)(1.5 + xIncrement);
	int pacmanY = (int)(1.5 + yIncrement);
	int monster1X = (int)(monster1[0]);
	int monster1Y = (int)(monster1[1]);
	int monster2X = (int)(monster2[0]);
	int monster2Y = (int)(monster2[1]);
	int monster3X = (int)(monster3[0]);
	int monster3Y = (int)(monster3[1]);
	int monster4X = (int)(monster4[0]);
	int monster4Y = (int)(monster4[1]);
	if (pacmanX == monster1X && pacmanY == monster1Y){
		over = true;
	}
	if (pacmanX == monster2X && pacmanY == monster2Y){
		over = true;
	}
	if (pacmanX == monster3X && pacmanY == monster3Y){
		over = true;
	}
	if (pacmanX == monster4X && pacmanY == monster4Y){
		over = true;
	}
	if (points == 106){
		over = true;
	}
}

//Method to display the results of the game at the ends
void resultsDisplay(){
	if (points == 106){
		//Won
		char* message = "*************************************";
		glRasterPos2f(170, 250);
		while (*message)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
		message = "CONGRATULATIONS, YOU WON! ";
		glColor3f(1, 1, 1);
		glRasterPos2f(200, 300);
		while (*message)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
		message = "*************************************";
		glRasterPos2f(170, 350);
		while (*message)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
		message = "To start or restart the game, press the space key.";
		glRasterPos2f(170, 550);
		while (*message)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *message++);
	}else {
		//Lost
		char* message = "*************************";
		glRasterPos2f(210, 250);
		while (*message)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
		message = "SORRY, YOU LOST ... ";
		glColor3f(1, 1, 1);
		glRasterPos2f(250, 300);
		while (*message)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
		message = "*************************";
		glRasterPos2f(210, 350);
		while (*message)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
		message = "You got: ";
		glRasterPos2f(260, 400);
		while (*message)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
		string result = to_string(points);
		message = (char*)result.c_str();
		glRasterPos2f(350, 400);
		while (*message)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
		message = " points!";
		glRasterPos2f(385, 400);
		while (*message)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
		message = "To start or restart the game, press the space key.";
		glRasterPos2f(170, 550);
		while (*message)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *message++);
	}
}


void welcomeScreen(){
	glClearColor(0, 0.2, 0.4, 1.0);
	char* message = "*************************************";
	glRasterPos2f(150, 200);
	while (*message)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
	message = "PACMAN";
	glColor3f(1, 1, 1);
	glRasterPos2f(330, 250);
	while (*message)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
	message = "*************************************";
	glRasterPos2f(150, 300);
	while (*message)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
	message = "CGV miniproject by Sharath L P(1BY18CS222) and Sujan(1BY18CS172)";
	glRasterPos2f(10, 350);
	while (*message)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
	message = "To control Pacman use A to go left, D to go right, W to go up and S to go down.";
	glRasterPos2f(50, 400);
	while (*message)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *message++);
	message = "To start or restart the game, press the space key.";
	glRasterPos2f(170, 450);
	while (*message)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *message++);
}

//Method to display the screen and its elements
void display(){
	if (points == 1){
		over = false;
	}
	keyOperations();
	glClear(GL_COLOR_BUFFER_BIT);
	gameOver();
	if (replay){
		if (!over){
			drawLaberynth();
			drawFood((1.5 + xIncrement) * squareSize, (1.5 + yIncrement) * squareSize);
			drawPacman(1.5 + xIncrement, 1.5 + yIncrement, rotation);
			updateMonster(monster1, 1);
			updateMonster(monster2, 2);
			updateMonster(monster3, 3);
			updateMonster(monster4, 4);
			drawMonster(monster1[0], monster1[1], 0.0, 1.0, 1.0);
			drawMonster(monster2[0], monster2[1], 1.0, 0.0, 0.0);
			drawMonster(monster3[0], monster3[1], 1.0, 0.0, 0.6);
			drawMonster(monster4[0], monster4[1], 1.0, 0.3, 0.0);
		}
		else {
			resultsDisplay();
		}
	}
	else {
		welcomeScreen();
	}
	glutSwapBuffers();
}

//Methdo to reshape the game is the screen size changes
void reshape(int w, int h){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glOrtho(0, 750, 750, 0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


//Main functions that controls the running of the game
int main(int argc, char** argv){
	//initialize and create the screen
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(750, 750);
	glutInitWindowPosition(500, 50);
	glutCreateWindow("PACMAN");

	//define all the control functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(display);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp);

	//run the game
	init();
	glutMainLoop();
	return 0;
}

