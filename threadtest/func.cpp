#define _CRT_SECURE_NO_WARNINGS

#include"game.h"







void initgame(PLANE** enemy, BARRIER** barrier, PLANE* myPlane, int* EnemyCurNum, DWORD* startime, time_t* Colstartime,int offsetX, int* backgroundOffset,time_t *Teststartime,int* sumtestnum,int* colcount,int*diecount,int* diecountplus)
{
	//Clear();
	srand((unsigned)time(NULL));
	myPlane->planepos = {(double)offsetX+(WIDTH / 2 - PLANESIZE / 2),HEIGHT - PLANESIZE / 2 };
	myPlane->bulletnum = 0;
	myPlane->bulletspeed = 3;
	myPlane->health = 66;
	myPlane->Score = 0;
	EnemyCurNum = 0;
	*startime = GetTickCount();
	*Colstartime = time(NULL);
	*Teststartime= time(NULL);
	*backgroundOffset = 0;
	*sumtestnum =0;
	*colcount = 0;
	*diecount = 0;
	*diecountplus = 0;
	BARRIER* b = *barrier;
	while (b != NULL)
	{
		BARRIER* temp = b;
		b = b->next;
		free(temp);
	}
	*barrier = NULL;
	
	*barrier = createbarrier(offsetX);

	
	
	////清空列表
	PLANE* p = *enemy;
	while (p != NULL)
	{
		PLANE* temp = p;
		p = p->next;
		free(temp);
	}
	*enemy = NULL;
	
	/*BARRIER* pba = *barrier;
	while (pba != NULL)
	{
		BARRIER* temp = pba;
		pba = pba->next;
		free(temp);
	}
	*barrier = NULL;*/
}

void initdraw(PLANE** enemy, BARRIER** barrier, PLANE* myPlane, int offsetX , int backgroundOffset)
{




	BeginBatchDraw();
	
	static double bulletangle = 0;
	bulletangle += BULLETANGLE;
	// 绘制背景
	putimage(offsetX, backgroundOffset - HEIGHT, &Img[0]);
	putimage(offsetX, backgroundOffset, &Img[0]);
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 3);
	line(WIDTH,0, WIDTH, HEIGHT);
	putimage(myPlane->planepos.x - PLANESIZE * 1.5 / 2, myPlane->planepos.y - PLANESIZE * 1.5 / 2, &Img[5], NOTSRCERASE);
	putimage(myPlane->planepos.x - PLANESIZE * 1.5 / 2, myPlane->planepos.y - PLANESIZE * 1.5 / 2, &Img[1], SRCINVERT);
	// 绘制链表中的敌机
	PLANE* p1 = *enemy;
	while (p1 != NULL)
	{
		/*putimage(p1->planepos.x - PLANESIZE / 2, p1->planepos.y - PLANESIZE / 2, &Img[6], NOTSRCERASE);

		putimage(p1->planepos.x - PLANESIZE / 2, p1->planepos.y - PLANESIZE / 2, &Img[2], SRCINVERT);*/
		double angle = -atan2(myPlane->planepos.y - p1->planepos.y, myPlane->planepos.x - p1->planepos.x);
		angle += 3.14159 / 2;
		IMAGE rotatedImg;
		//rotateimage(&rotatedImg, &Img[6], angle);
		//putimage(p1->planepos.x - PLANESIZE / 2, p1->planepos.y - PLANESIZE / 2, &rotatedImg, NOTSRCERASE);
		rotateimage(&rotatedImg, &Img[2], angle);
		putimage(p1->planepos.x - PLANESIZE / 2, p1->planepos.y - PLANESIZE / 2, &rotatedImg, SRCINVERT);
		p1 = p1->next;
	}
	
	
	//绘制链表中的障碍物
	BARRIER* p2 = *barrier;
	while (p2 != NULL)
	{
		putimage(p2->barrierpos.x - PLANESIZE, p2->barrierpos.y - PLANESIZE, &Img[4], SRCINVERT);
		p2 = p2->next;
	}
	int i = 0;
	IMAGE rotateimg[2];
	for (i = 0; i < myPlane->bulletnum; i++)
	{

		rotateimage(&rotateimg[0], &Img[3], bulletangle);




		putimage(myPlane->planebullet[i].x, myPlane->planebullet[i].y, &rotateimg[0], SRCINVERT);
	}
	//printf("%d\n", myPlane->bulletnum);
	/*for (i = 0; i < myPlane->bulletnum; i++)
	{

		printf("%d===================\n",offsetX);
	}*/
	RECT rect1 = { offsetX,PLANESIZE,offsetX+WIDTH,PLANESIZE * 3 };

	RECT rect3 = { myPlane->planepos.x - (PLANESIZE * 1.5 / 2),myPlane->planepos.y + (PLANESIZE * 1.5 / 2),myPlane->planepos.x + (PLANESIZE * 1.5 / 2),myPlane->planepos.y + (PLANESIZE * 1.5 / 2) + PLANESIZE };

	setbkmode(TRANSPARENT);
	char str1[100];

	char str3[100];

	sprintf(str1, "score:%lld", myPlane->Score);

	sprintf(str3, "health:%d", myPlane->health);

	settextcolor(BLACK);
	drawtext(str1, &rect1, DT_TOP | DT_CENTER);
	settextcolor(RED);
	drawtext(str3, &rect3, DT_TOP | DT_CENTER);
	//setbkmode(TRANSPARENT);
	if (flag)
	{

		RECT rect2 = { offsetX, 0, offsetX+WIDTH, PLANESIZE };

		drawtext("暂停中", &rect2, DT_TOP | DT_LEFT);
	}
	EndBatchDraw();

}

void Move(PLANE** enemy, BARRIER** barrier, const PotentialField& pof, PLANE* myPlane, int* EnemyCurNum, DWORD* startime, time_t* Colstartime, time_t* Colendtime, int offsetX,int* backgroundOffset,time_t* Teststartime, time_t* Testendtime, int* sumtestnum, int* colcount, int* diecount,int* diecountplus)
{
	*Testendtime= time(NULL);
	if (*Testendtime- *Teststartime>=UTIME)
	{
		//printf("===============================================\n");
		myPlane->Colnum[*sumtestnum]= *colcount;
		myPlane->dienum[*sumtestnum] = *diecount;
		myPlane->dienum2[*sumtestnum] = *diecountplus;
		//printf("%d %d %d\n", offsetX, *sumtestnum, *colcount);

		(*sumtestnum)++;
		*colcount = 0;
		*diecount = 0;
		*diecountplus = 0;
		*Teststartime = *Testendtime;
	}
	if (GetAsyncKeyState(' ') & 0x8000)
	{
		if (!Spacekeypressed)
		{
			flag = !flag;
			Spacekeypressed = true;
		}
	}
	else
	{
		Spacekeypressed = false;
	}
	if (flag)
	{
		return;
	}
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	{
		if (offsetX == 0) {
			thread1Running = false;
		}
		else {
			thread2Running = false;
		}
		//exit(0);
	}
	
	ExMessage msg;
	if (peekmessage(&msg, EM_MOUSE))
	{
		if (msg.message == WM_LBUTTONDOWN&&offsetX==0)
		{
			if (myPlane->bulletnum + 1 <= BULLETNUM)
			{
				int x = myPlane->planepos.x;
				int y = myPlane->planepos.y;
				myPlane->planebullet[myPlane->bulletnum].x = x - 7.14;
				myPlane->planebullet[myPlane->bulletnum].y = y - PLANESIZE / 4;
				
				/*printf("%lf ", myPlane->planepos.x);
				printf("%lf ", x - 7.14);

				printf("%lf ", myPlane->planebullet[myPlane->bulletnum].x);
				printf("%d\n", offsetX);*/
				myPlane->bulletnum++;
			}
		}
		else if(msg.message == WM_RBUTTONDOWN && offsetX == WIDTH)
		{
			if (myPlane->bulletnum + 1 <= BULLETNUM)
			{
				int x = myPlane->planepos.x;
				int y = myPlane->planepos.y;
				myPlane->planebullet[myPlane->bulletnum].x = x - 7.14;
				myPlane->planebullet[myPlane->bulletnum].y = y - PLANESIZE / 4;

				
				myPlane->bulletnum++;
			}
		}
	}

	*enemy = initenemy(*enemy, EnemyCurNum, startime, &endtime1, offsetX);

	if(offsetX==0)
	{
		if (GetAsyncKeyState('W') & 0x8000 && myPlane->planepos.y > 0)
		{
			myPlane->planepos.y -= PLANESPEED;
		}
		if (GetAsyncKeyState('A') & 0x8000 && myPlane->planepos.x > offsetX)
		{
			myPlane->planepos.x -= PLANESPEED;
		}
		if (GetAsyncKeyState('S') & 0x8000 && myPlane->planepos.y < HEIGHT - PLANESIZE)
		{
			myPlane->planepos.y += PLANESPEED;
		}
		if (GetAsyncKeyState('D') & 0x8000 && myPlane->planepos.x < offsetX + WIDTH - PLANESIZE)
		{
			myPlane->planepos.x += PLANESPEED;
		}
	}
	if (offsetX == WIDTH)
	{
		if (GetAsyncKeyState(VK_UP) & 0x8000 && myPlane->planepos.y > 0)
		{
			myPlane->planepos.y -= PLANESPEED;
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000 && myPlane->planepos.x > offsetX)
		{
			myPlane->planepos.x -= PLANESPEED;
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000 && myPlane->planepos.y < HEIGHT - PLANESIZE)
		{
			myPlane->planepos.y += PLANESPEED;
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && myPlane->planepos.x < offsetX + WIDTH - PLANESIZE)
		{
			myPlane->planepos.x += PLANESPEED;
		}
	}
	

	// 收集所有障碍物位置
	std::vector<POS> barrierPositions;
	std::vector<Vec2d> barrierVec;

	BARRIER* p2 = *barrier;
	while (p2 != NULL) {
		barrierPositions.push_back(p2->barrierpos);
		Vec2d temp = { p2->dx,p2->dy };
		barrierVec.push_back(temp);

		p2 = p2->next;
	}

	// 修改敌机移动逻辑
	PLANE* p = *enemy;
	while (p != NULL) {
		// 获取下一个移动位置
		POS nextPos;
		if(offsetX==0)  nextPos = getNextMove1(p, myPlane->planepos, barrierPositions, barrierVec, offsetX+WIDTH, HEIGHT, PLANESIZE +PLANESIZE/2,pof,offsetX);
		else  nextPos = getNextMove1(p, myPlane->planepos, barrierPositions, barrierVec, offsetX + WIDTH, HEIGHT, PLANESIZE + (PLANESIZE / 2), pof,offsetX);
	
		// 平滑移动
		double dx = nextPos.x - p->planepos.x;
		double dy = nextPos.y - p->planepos.y;
		double distance = sqrt(dx * dx + dy * dy);

		if (distance > 0) {
			double speed = p->planespeed;
			// 确保敌机可以向下移动
			if (p->planepos.y < HEIGHT) {
				p->planepos.x += (dx / distance) * speed;
				p->planepos.y += (dy / distance) * speed;
			}
		}


		p = p->next;
	}
	
	int i = 0;
	for (i = 0;i < myPlane->bulletnum;i++)
	{
		myPlane->planebullet[i].y -= BULLETSPEED;
	}
	destroyplane(enemy, barrier,myPlane, Colstartime, Colendtime,EnemyCurNum,offsetX,backgroundOffset,Teststartime,sumtestnum,colcount,diecount,diecountplus);
	destorybullet(enemy, barrier,myPlane, EnemyCurNum,offsetX,diecount,diecountplus);

	// 更新背景偏移量
	*backgroundOffset += 1;  // 调整这个值可以改变背景移动的速度
	if (*backgroundOffset >= HEIGHT) {
		*backgroundOffset = 0;  // 重置偏移量以实现循环滚动
	}
	//障碍物移动
	barrmove(barrier, offsetX);

}
PLANE* initenemy(PLANE* enemy, int* EnemyCurNum, DWORD* startime, DWORD* endtime,int offsetX)
{
	PLANE* h, * r, * pt;
	h = enemy;
	pt = enemy;
	if (pt != NULL)
	{
		while (pt->next != NULL)
		{
			pt = pt->next;
		}
	}
	r = pt;
	*endtime = GetTickCount();
	double timegap = (*endtime - *startime) * 1.0 / 1000;
	//printf("%lf\n", timegap);
	if (timegap >= ENEMYPROSPEED)
	{
		if (*EnemyCurNum < ENEMYNUM)
		{
			PLANE* p = (PLANE*)malloc(sizeof(PLANE));
			if (p == NULL) {
				return h;
			}
			p->health = 1;
			p->planespeed = ENEMYFLYSPEED * 2;
			double x = offsetX+(rand() % (WIDTH - 75) + PLANESIZE / 2);
			double y = -(PLANESIZE / 2);
			p->planepos.x = x;
			p->planepos.y = y;
			/*POS pos = { 0,0 };
			p->moveHistory.push_back(pos);*/
			p->Historycurnum = 0;
			p->Historynum = 0;
			p->next = NULL;
			
			if (h == NULL)
			{
				h = p;
				r = p;
			}
			else
			{
				r->next = p;
				r = p;
			}
			(*EnemyCurNum)++;
		}
		*startime = *endtime;
	}
	return h;

}
// 删除health <= 0的敌机
void delezeroe(PLANE** enemy, int* n)
{
	PLANE* p = NULL;
	PLANE* r = *enemy;
	while (r != NULL) {
		if (r->health <= 0) {
			PLANE* temp = r;
			if (p == NULL) {
				*enemy = r->next;
			}
			else {
				p->next = r->next;
			}
			r = r->next;
			free(temp);
			(*n)--;
		}
		else {
			p = r;
			r = r->next;
		}
	}
}



int isintersect(POS a, POS b, int r)
{
	if (fabs(a.x - b.x) <= r && fabs(a.y - b.y) <= r)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
// 销毁飞机
void destroyplane(PLANE** enemy, BARRIER** barrier, PLANE* myPlane, time_t* Colstartime, time_t* Colendtime, int* EnemyCurNum, int offsetX,int* backgroundOffset, time_t* Teststartime, int* sumtestnum, int* colcount, int* diecount, int* diecountplus)
{


	PLANE* p1 = *enemy;
	while (p1 != NULL)
	{
		int	collisionRadius1 = PLANESIZE / 2 + (PLANESIZE * 1.5 / 2) - 20;

		if (isintersect(p1->planepos, myPlane->planepos, collisionRadius1))
		{
			(*colcount)++;
			
			p1->health = 0;
			*Colendtime = time(NULL);
			int timediv = *Colendtime - *Colstartime;
			//printf("%d %d %d\n", offsetX, *Colstartime, *Colendtime);
			if (timediv >= 1)
			{
				//printf("===========================================\n");
				//myPlane->health -= 5;
				*Colstartime = *Colendtime;
			}
			if (myPlane->health <= 0)
			{
				DrawoverPage(offsetX, myPlane->Colnum, myPlane->dienum,myPlane->dienum2, *sumtestnum);
				initgame(enemy, barrier, myPlane, EnemyCurNum, &startime1, Colendtime, offsetX,backgroundOffset,Teststartime,sumtestnum,colcount,diecount,diecountplus);
				break;
				/*if (IDYES == MessageBox(GetHWnd(), "游戏结束，是否要重新开始游戏", "提示", MB_YESNO))
				{
					initgame(enemy, barrier,myPlane, &EnemyCurNum1, &startime1, &Colendtime1, offsetX);
					break;
				}
				else
				{
					exit(0);
				}*/
			}
		}

		// 删除超出屏幕的敌机
		if (p1->planepos.y > HEIGHT)
		{
			p1->health = 0;
		}
		p1 = p1->next;
	}
	int collisionRadius3 = 5 + PLANESIZE * 1.5 / 2;
	
	BARRIER* p2 = *barrier;
	while (p2 != NULL)
	{
		int	collisionRadius2 = PLANESIZE + (PLANESIZE * 1.5 / 2) - 15;

		if (isintersect(p2->barrierpos, myPlane->planepos, collisionRadius2))
		{
			//myPlane->health = 0;
			if (myPlane->health <= 0)
			{
				DrawoverPage(offsetX,myPlane->Colnum,myPlane->dienum, myPlane->dienum2,*sumtestnum);
				initgame(enemy, barrier, myPlane, EnemyCurNum, &startime1, Colstartime, offsetX,backgroundOffset, Teststartime, sumtestnum, colcount,diecount,diecountplus);
				break;
				
			}
		}
		p2 = p2->next;
	}
	delezeroe(enemy, EnemyCurNum);
	

}
// 销毁子弹和敌机
void destorybullet(PLANE** enemy, BARRIER** barrier, PLANE* myPlane, int* EnemyCurNum,int offsetX, int* diecount,int* diecountplus)
{

	PLANE* p1 = *enemy;
	BARRIER* p2 = *barrier;


	while (p1 != NULL)
	{
		int collisionRadius3 = PLANESIZE / 10;
		PLANE* p3 = p1->next;
		while (p3 != NULL)
		{
			if (isintersect(p1->planepos, p3->planepos, collisionRadius3) /*&& p1->planepos.y > PLANESIZE && p3->planepos.y > PLANESIZE*/)
			{
				(*diecountplus)++;
				p1->health = 0;
				p3->health = 0;
				//printf("--------------------------\n");

			}
			p3 = p3->next;
		}
		int	collisionRadius2 = PLANESIZE / 2 + PLANESIZE - 20;
		p2 = *barrier;
		while (p2 != NULL)
		{
			//printf("--------------------------\n");
			if (isintersect(p1->planepos, p2->barrierpos, collisionRadius2))
			{
				//printf("================================\n");
				(*diecount)++;
				p1->health = 0;
				//printf("--------------------------\n");
				break;
			}
			p2 = p2->next;
		}
		p1 = p1->next;
	}

	for (int i = 0; i < myPlane->bulletnum; i++)
	{
		p1 = *enemy;
		while (p1 != NULL && p1->health != 0)
		{
			int	collisionRadius1 = PLANESIZE / 2 + PLANESIZE / 6;


			// 判定是否碰撞
			if (isintersect(p1->planepos, myPlane->planebullet[i], collisionRadius1))
			{
				p1->health = 0;
				myPlane->Score++;
				for (int k = i; k < myPlane->bulletnum - 1; k++)
				{
					myPlane->planebullet[k] = myPlane->planebullet[k + 1];
				}
				myPlane->bulletnum--;
				i--;
				break;
			}
			p1 = p1->next;
		}


		// 删除超出屏幕的子弹
		if (myPlane->planebullet[i].y < 0 || myPlane->planebullet[i].x <offsetX  || myPlane->planebullet[i].x > offsetX+WIDTH)
		{
			for (int k = i; k < myPlane->bulletnum - 1; k++)
			{
				myPlane->planebullet[k] = myPlane->planebullet[k + 1];
			}
			myPlane->bulletnum--;
			i--;
			continue;
		}
		p2 = *barrier;
		int	collisionRadius3 = PLANESIZE + PLANESIZE / 6;
		while (p2 != NULL)
		{
			//printf("--------------------------\n");
			if (isintersect(myPlane->planebullet[i], p2->barrierpos, collisionRadius3))
			{
				for (int k = i; k < myPlane->bulletnum - 1; k++)
				{
					myPlane->planebullet[k] = myPlane->planebullet[k + 1];
				}
				myPlane->bulletnum--;
				i--;
				break;
			}
			p2 = p2->next;
		}

		




	}

	delezeroe(enemy,EnemyCurNum);
	

}
void DrawoverPage(int offsetX,int data1[200],int data2[200],int data3[200], int dataSize)
{
	
	putimage(offsetX, 0, &Img[7]);
	int sum1 = 0;
	int sum2 = 0;
	int sum3 = 0;	
	// 绘制 x 轴
	setlinecolor(WHITE);
	setlinestyle(PS_SOLID, 1);
	line(offsetX+50, HEIGHT - 350, offsetX+WIDTH - 50, HEIGHT - 350);
	// 绘制 x 轴箭头
	line(offsetX + WIDTH - 50, HEIGHT - 350, offsetX + WIDTH - 60, HEIGHT - 345);
	line(offsetX + WIDTH - 50, HEIGHT - 350, offsetX + WIDTH - 60, HEIGHT - 355);

	// 绘制 y 轴
	line(offsetX+50, 50, offsetX+50, HEIGHT - 350);
	// 绘制 y 轴箭头
	line(offsetX+50, 50, offsetX+45, 60);
	line(offsetX+50, 50, offsetX+55, 60);
	char str[10];
	int posy = 0;
	settextstyle(15, 0, "黑体");
	outtextxy(offsetX + 15, 50, "次数");
	outtextxy(offsetX + WIDTH - 50, HEIGHT - 340, "时间/s");
	setfillcolor(RED);
	fillcircle(offsetX+WIDTH-150, 100, 8);
	outtextxy(offsetX + WIDTH - 130,95, "到达目标数量");
	setfillcolor(GREEN);
	fillcircle(offsetX + WIDTH - 150, 130, 8);
	outtextxy(offsetX + WIDTH - 130, 125, "撞到障碍数量");
	setfillcolor(YELLOW);
	fillcircle(offsetX + WIDTH - 150, 160, 8);
	outtextxy(offsetX + WIDTH - 130, 155, "飞机相互碰撞数量");
	for (int i = HEIGHT - 350;i>50;i = i -25 )
	{
		sprintf(str, "%d", posy);
		outtextxy(offsetX + 25,i , str);
		posy += 50;
	}
	int posx = 0;
	settextstyle(10, 0, "黑体");
	for (int i = offsetX + 50;i < offsetX + WIDTH - 60;i = i+20)
	{
		sprintf(str, "%d", posx);
		outtextxy(i, HEIGHT - 330, str);
		posx += 3;
	}
	for (int i = 0; i < dataSize/*&& 50 + offsetX + ((i + 1) * 20)<offsetX+WIDTH-50*/; i++) {
		printf("%d %d\n", offsetX, data1[i]);
		
		setfillcolor(RED);
		fillcircle(50 + offsetX + ((i+1) * 20), HEIGHT-(350 + data1[i]/2), 8);
		sum1 += data1[i];
	}
	settextstyle(30, 0, "黑体");
	outtextxy(offsetX + 30, HEIGHT - 150, "平均到达目标数量:");
	sprintf(str, "%d", sum1/ dataSize);
	outtextxy(offsetX+300, HEIGHT - 150, str);


	for (int i = 0; i < dataSize/* && 50 + offsetX + ((i + 1) * 20) < offsetX + WIDTH - 50*/; i++) {
		//printf("%d %d\n", offsetX, data[i]);

		setfillcolor(GREEN);
		fillcircle(50 + offsetX + ((i + 1) * 20), HEIGHT - (350 + data2[i]/2), 8);
		sum2 += data2[i];
	}
	settextstyle(30, 0, "黑体");
	outtextxy(offsetX + 30, HEIGHT - 100, "平均撞到障碍数量:");

	sprintf(str, "%d", sum2 / dataSize);
	outtextxy(offsetX + 300, HEIGHT - 100, str);


	for (int i = 0; i < dataSize/*&& 50 + offsetX + ((i + 1) * 20)<offsetX+WIDTH-50*/; i++) {
		printf("%d %d\n", offsetX, data1[i]);

		setfillcolor(YELLOW);
		fillcircle(50 + offsetX + ((i + 1) * 20), HEIGHT - (350 + data3[i] / 2), 8);
		sum3 += data3[i];
	}
	settextstyle(30, 0, "黑体");
	outtextxy(offsetX + 30, HEIGHT - 50, "平均相互碰撞数量:");
	sprintf(str, "%d", sum3 / dataSize);
	outtextxy(offsetX + 300, HEIGHT - 50, str);
	outtextxy(offsetX + 350, HEIGHT - 100, "适应度:");
	sprintf(str, "%f", -((1 * sum1 / dataSize) - (0.01 * sum2 / dataSize) - (0.05 * sum3 / dataSize)));
	outtextxy(offsetX + 450, HEIGHT - 100, str);
	while(1)
	{
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			if (!Enterkeypressed)
			{
				exit(0);
				Enterkeypressed = true;
			}
		}
		else
		{
			Enterkeypressed = false;
		}
	}
	
}
//void drawpoint()
//{
//
//}



