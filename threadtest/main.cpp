#define _CRT_SECURE_NO_WARNINGS
#include"game.h"








PLANE myPlane1, myPlane2;
int EnemyCurNum1, EnemyCurNum2;
DWORD startime1, endtime1, startime2, endtime2;
time_t Colstartime1, Colendtime1, Colstartime2, Colendtime2, Teststartime1, Testendtime1, Teststartime2, Testendtime2;
IMAGE Img[10];



extern int backgroundOffset1=0, backgroundOffset2=0;

bool flag=false;
bool Spacekeypressed = false;
bool Enterkeypressed = false;
bool thread1Running = true;
bool thread2Running = true;
int sumtestnum1, colcount1, diecount1, sumtestnum2, colcount2, diecount2,diecountplus1,diecountplus2;
// 互斥锁
CRITICAL_SECTION cs;





DWORD WINAPI GameThread1(LPVOID lpParam) {
    PLANE* enemy1 = NULL;
    BARRIER* barrier1 = NULL;
   
    
    int offsetX = 0;
    PotentialField pof = { 7.087390 ,795323.080374, 96854.064470 ,12.166078 };
    initgame(&enemy1, &barrier1, &myPlane1, &EnemyCurNum1, &startime1,&Colstartime1, offsetX,&backgroundOffset1,&Teststartime1, &sumtestnum1, &colcount1,&diecount1,&diecountplus1);

    while (thread1Running)
    {
        EnterCriticalSection(&cs);
        initdraw(&enemy1, &barrier1, &myPlane1, offsetX, backgroundOffset1);
       
        Move(&enemy1, &barrier1, pof, &myPlane1, &EnemyCurNum1, &startime1,&Colstartime1, &Colendtime1, offsetX, &backgroundOffset1,&Teststartime1, &Testendtime1,&sumtestnum1,&colcount1,&diecount1,&diecountplus1);
        LeaveCriticalSection(&cs);
        Sleep(10);
    }
    DrawoverPage(offsetX, myPlane1.Colnum, myPlane1.dienum, myPlane1.dienum2, sumtestnum1);
    return 0;
}

DWORD WINAPI GameThread2(LPVOID lpParam) {
    PLANE* enemy2 = NULL;
    BARRIER* barrier2 = NULL;
    
    
    int offsetX = WIDTH;
    PotentialField pof = { 7.123674, 340654.848734, 632627.550513,21.383836 };
    initgame(&enemy2, &barrier2, &myPlane2, &EnemyCurNum2, &startime2, &Colstartime2, offsetX ,&backgroundOffset2, &Teststartime2, &sumtestnum2, &colcount2,&diecount2,&diecountplus2);

    while (thread2Running)
    {
        EnterCriticalSection(&cs);
        initdraw(&enemy2, &barrier2, &myPlane2, offsetX,backgroundOffset2);
       
        Move(&enemy2, &barrier2, pof, &myPlane2, &EnemyCurNum2, &startime2, &Colstartime2, &Colendtime2, offsetX, &backgroundOffset2, &Teststartime2, &Testendtime2, &sumtestnum2, &colcount2,&diecount2,&diecountplus2);
        LeaveCriticalSection(&cs);
        Sleep(10);
    }
    DrawoverPage(offsetX, myPlane2.Colnum, myPlane2.dienum, myPlane2.dienum2, sumtestnum2);

    return 0;
}

int main()
{
    // 初始化互斥锁
    InitializeCriticalSection(&cs);


	loadimage(&Img[0], "./res/背景.png", WIDTH, HEIGHT);
	loadimage(&Img[1], "./res/myplane.jpg", PLANESIZE * 1.5, PLANESIZE * 1.5);
	loadimage(&Img[2], "./res/enemyplane.png", PLANESIZE, PLANESIZE);
	loadimage(&Img[3], "./res/子弹8.png", PLANESIZE / 3, PLANESIZE / 3);
	loadimage(&Img[4], "./res/障碍.png", PLANESIZE*2, PLANESIZE*2);
	loadimage(&Img[5], "./res/myplanebk.jpg", PLANESIZE * 1.5, PLANESIZE * 1.5);
	loadimage(&Img[6], "./res/enemyplanebk.jpg", PLANESIZE, PLANESIZE);
    loadimage(&Img[7], "./res/结束页.png", WIDTH, HEIGHT);


	initgraph(WIDTH*2, HEIGHT);
    // 创建线程
    HANDLE hThread1 = CreateThread(NULL, 0, GameThread1, NULL, 0, NULL);
    HANDLE hThread2 = CreateThread(NULL, 0, GameThread2, NULL, 0, NULL);
    // 等待线程结束（这里不会结束）
    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);

    // 删除互斥锁
    DeleteCriticalSection(&cs);

	






	


    getchar();
	closegraph();
	return 0;
}





