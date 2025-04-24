#define _CRT_SECURE_NO_WARNINGS
#include"game.h"








PLANE myPlane;
int EnemyCurNum;
DWORD startime, endtime;
time_t Colstartime, Colendtime,Teststartime,Testendtime;
IMAGE Img[10];
PLANE* enemy = NULL;
BARRIER* barrier = NULL;
int n;
int backgroundOffset;
bool flag = false;
bool Spacekeypressed = false;
int sumtestnum, colcount;






int main()
{



	loadimage(&Img[0], "./res/±³¾°.png", WIDTH, HEIGHT);
	loadimage(&Img[1], "./res/myplane.jpg", PLANESIZE * 1.5, PLANESIZE * 1.5);
	loadimage(&Img[2], "./res/enemyplane.png", PLANESIZE, PLANESIZE);
	loadimage(&Img[3], "./res/×Óµ¯8.png", PLANESIZE / 3, PLANESIZE / 3);
	loadimage(&Img[4], "./res/ÕÏ°­.png", PLANESIZE * 2, PLANESIZE * 2);
	loadimage(&Img[5], "./res/myplanebk.jpg", PLANESIZE * 1.5, PLANESIZE * 1.5);
	loadimage(&Img[6], "./res/enemyplanebk.jpg", PLANESIZE, PLANESIZE);


	initgraph(WIDTH, HEIGHT);


	/*PLANE* enemy = NULL;
	BARRIER* barrier = NULL;*/

	PotentialField pof = { 7.123674, 340654.848734, 632627.550513,21.383836 };
	initgame(&enemy, &barrier);






	while (1)
	{
		initdraw(&enemy, &barrier);

		Move(&enemy, &barrier, pof);
		Sleep(10);
	}



	closegraph();
	return 0;
}





