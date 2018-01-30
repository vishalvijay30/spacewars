#define MAX_BULLETS 100
#define MAX_ENEMIES 3
#define MAX_LIVES 3

typedef struct aircraft {
	int x;
	int y;
	int height;
	int width;
	int gunHeight;
	int gunWidth;
	int score;
	int alive;
	int lives;
	u16 color;
} AIRCRAFT;

typedef struct bullet {
	int x;
	int y;
	int height;
	int width;
	int exists;
	u16 color;
} BULLET;

//prototypes
void populateDummyBullets();
void populateDummyEnemies();
void manageBullets();
void manageEnemies();
void drawScore();
void drawLives();
