#include <stdlib.h>
#include <stdio.h>

#include "graphics.h"
#include "controls.h"
#include "game.h"

AIRCRAFT a = {0, 0, 20, 20, 2, 4, 0, 1, MAX_LIVES, YELLOW};

BULLET bullets[MAX_BULLETS];
AIRCRAFT aircrafts[MAX_ENEMIES];

int realBullets = 0;

char buffer[41];

int main() {	
	printf("char:%d\n", sizeof(char));
	printf("short:%d\n", sizeof(short));
	printf("int:%d\n", sizeof(int));
	printf("char_ptr:%d\n", sizeof(char*));
	printf("short_ptr:%d\n", sizeof(short*));
	printf("int_ptr:%d\n", sizeof(int*));
			
	drawBG();
    
    int oldRow = a.x;
    int oldCol = a.y;
   
    int seed = a.score;
    
    srand(seed);
  
  	populateDummyBullets();
	
	populateDummyEnemies();

	//start screen
    drawImage3(0, 0, 160, 240, space_wars);
    sprintf(buffer, "Press ENTER to start!");
	drawString(150, 60, buffer, WHITE);
   	while(!KEY_DOWN_NOW(BUTTON_START));
    drawBG();
    
    while(1) {

    	if(KEY_DOWN_NOW(BUTTON_UP)) {
		 	a.x--;
			if (a.x < 0) {
				a.x = 0;
			}
		}
		if(KEY_DOWN_NOW(BUTTON_DOWN)) {
			a.x++;
			if (a.x > SCREENHEIGHT) {
				a.x = SCREENHEIGHT;
			}
		}
		if(KEY_DOWN_NOW(BUTTON_LEFT)) {
			a.y--;
			if (a.y < 0) {
				a.y = 0;
			}
		}
		if(KEY_DOWN_NOW(BUTTON_RIGHT)) {
			a.y++;
			if (a.y > SCREENWIDTH) {
				a.y = SCREENWIDTH;
			}
		}
		if(KEY_DOWN_NOW(BUTTON_A)) {
			realBullets = 1;
			BULLET *curr;
			for (int bul = 0; bul < MAX_BULLETS; bul++) {
				curr = bullets + bul;
				if (!curr->exists) {
					BULLET b = {a.x + 9, a.y + a.gunWidth + a.width, curr->height, curr->width, 1, RED};
					bullets[bul] = b;
				}
			}
		}
		
		waitForVblank();
		
		drawScore();
		
		drawLives();
		
		manageBullets();
		
		drawImage3(oldRow, oldCol, a.height, a.width, background_space);
		drawImage3(oldRow + 9, oldCol + 20, a.gunHeight, a.gunWidth, background_space);
		drawRectangleDMA(a.x, a.y, a.height, a.width, a.color);
		drawRectangleDMA(a.x + 9, a.y + 20, 2, 4, a.color);
		oldCol = a.y;
		oldRow = a.x;
		
		waitForVblank();
		manageEnemies();

		if (a.lives <= 0) {
			drawImage3(0, 0, 160, 240, gameover);
			sprintf(buffer, "Score: %d", a.score);
			drawString(135, 80, buffer, WHITE);
			sprintf(buffer, "Press ENTER to play again!");
			drawString(145, 40, buffer, WHITE);
			while (!KEY_DOWN_NOW(BUTTON_START));
			if (KEY_DOWN_NOW(BUTTON_START)) {
				a.lives = 3;
				a.score = 0;
				a.x = 0;
				a.y = 0;
				populateDummyBullets();
				populateDummyEnemies();
				drawImage3(0, 0, 160, 240, background_space);
			}
		}
    }
	
}

void populateDummyBullets() {
	for (int i = 0; i < MAX_BULLETS; i++) {
    	BULLET dummy = {0, 0, 2, 5, 0, RED};
    	bullets[i] = dummy;
    }
}

void populateDummyEnemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
    	AIRCRAFT enemy = {rand()%SCREENHEIGHT, rand()%SCREENWIDTH + 150, 20, 20, 2, 4, 0, 1, 0, OLVGRN};
    	aircrafts[i] = enemy;
    }
}

void manageBullets() {
	BULLET *bulletsPtr;
	for (int i = 0; i < MAX_BULLETS; i++) {
		bulletsPtr = bullets + i;
		bulletsPtr->y = bulletsPtr->y + 10;
		if (bulletsPtr->y >= SCREENWIDTH) {
			bulletsPtr->exists = 0;
		}
		for (int j = 0; j < MAX_ENEMIES; j++) {
			if (bulletsPtr->exists && bulletsPtr->x >= aircrafts[j].x 
				&& bulletsPtr->x <= (aircrafts[j].x + aircrafts[j].height) && bulletsPtr->y >= aircrafts[j].y 
				&& bulletsPtr->y <= (aircrafts[j].y + aircrafts[j].width)) {
				aircrafts[j].alive = 0;
				drawImage3(aircrafts[j].x, aircrafts[j].y, aircrafts[j].height, aircrafts[j].width, background_space);
				drawImage3(aircrafts[j].x+9, aircrafts[j].y-4, aircrafts[j].gunHeight, 
					aircrafts[j].gunWidth, background_space);
				bulletsPtr->exists = 0;
				a.score++;
			}
		}
	}

	if (realBullets) {
		for (int i = 0; i < MAX_BULLETS; i++) {
			bulletsPtr = bullets + i;
			drawImage3(bulletsPtr->x, ((bulletsPtr->y) - 10), bulletsPtr->height, bulletsPtr->width, background_space);
		}
		
		for (int i = 0; i < MAX_BULLETS; i++) {
			bulletsPtr = bullets + i;
			if (bulletsPtr->exists) {
				drawRectangleDMA(bulletsPtr->x, bulletsPtr->y, bulletsPtr->height, bulletsPtr->width, bulletsPtr->color);
			}
		}
	}
}

void manageEnemies() {
	AIRCRAFT *enemiesPtr;
	
	for (int i = 0; i < MAX_ENEMIES; i++) {
		enemiesPtr = aircrafts + i;
		enemiesPtr->y = enemiesPtr->y - 1;
		if (enemiesPtr-> y >= 220) {
			enemiesPtr->alive = 0;
			drawImage3(enemiesPtr->x, enemiesPtr->y-10, enemiesPtr->height, enemiesPtr->width, background_space);
		}
			
		if (enemiesPtr->y <= 0) {
			if (enemiesPtr->alive) {
				a.lives--;
				drawImage3(enemiesPtr->x, enemiesPtr->y-10, enemiesPtr->height, enemiesPtr->width + 15, background_space);
			}
			enemiesPtr->alive = 0;
			
		}
	}

	for (int i = 0; i < MAX_ENEMIES; i++) {
		enemiesPtr = aircrafts + i;
		if (enemiesPtr->alive) {
			drawImage3(enemiesPtr->x, enemiesPtr->y+1, enemiesPtr->height, enemiesPtr->width, background_space);
			drawImage3(enemiesPtr->x+9, enemiesPtr->y-4, enemiesPtr->gunHeight, enemiesPtr->gunWidth, background_space);
			drawRectangleDMA(enemiesPtr->x, enemiesPtr->y, enemiesPtr->height, enemiesPtr->width, enemiesPtr->color);
			drawRectangleDMA(enemiesPtr->x+9, enemiesPtr->y-4, 2, 4, enemiesPtr->color);	
		}
	}
	for (int i = 0; i < MAX_ENEMIES; i++) {
		enemiesPtr = aircrafts + i;
		if (!enemiesPtr->alive) {
			AIRCRAFT newEnemy = {rand()%SCREENHEIGHT, rand()%SCREENWIDTH + 150, 20, 20, 2, 4, 0, 1, 0,OLVGRN};
			*enemiesPtr = newEnemy; 
		}
	}
}

void drawScore() {
	drawImage3(150, 5, 10, 60, background_space);
	sprintf(buffer, "Score: %d", a.score);
	drawString(150, 5, buffer, WHITE);
}

void drawLives() {	
	drawImage3(150, 180, 10, 60, background_space);
	sprintf(buffer, "Lives: %d", a.lives);
	drawString(150, 180, buffer, WHITE);
}

void waitForVblank() {
	while(SCANLINECOUNTER >= 160);
	while(SCANLINECOUNTER < 160);
}
