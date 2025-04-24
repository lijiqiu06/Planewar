#define _CRT_SECURE_NO_WARNINGS

#include"game.h"







void initgame(PLANE** enemy, BARRIER** barrier)
{
	//Clear();
	srand((unsigned)time(NULL));
	myPlane.planepos = { WIDTH / 2 - PLANESIZE / 2,HEIGHT - PLANESIZE / 2 };
	myPlane.bulletnum = 0;
	myPlane.bulletspeed = 3;
	myPlane.health = 66666;
	myPlane.Score = 0;
	FILE* pf = fopen("./res/score.txt", "rb");
	fread(&(myPlane.HighScore), sizeof(long long), 1, pf);
	fclose(pf);
	EnemyCurNum = 0;
	startime = GetTickCount();
	Colstartime = time(NULL);
	Teststartime= time(NULL);
	
	
	backgroundOffset = 0;
	*barrier = createbarrier();
	n = 0;
	sumtestnum = 1;
	colcount = 0;
	
	////清空列表
	PLANE* p = *enemy;
	while (p != NULL)
	{
		PLANE* temp = p;
		p = p->next;
		free(temp);
	}
	*enemy = NULL;
	
	
}

void initdraw(PLANE** enemy, BARRIER** barrier)
{




	BeginBatchDraw();
	static double bulletangle = 0;
	bulletangle += BULLETANGLE;
	// 绘制背景
	putimage(0, backgroundOffset - HEIGHT, &Img[0]);
	putimage(0, backgroundOffset, &Img[0]);

	putimage(myPlane.planepos.x - PLANESIZE * 1.5 / 2, myPlane.planepos.y - PLANESIZE * 1.5 / 2, &Img[5], NOTSRCERASE);
	putimage(myPlane.planepos.x - PLANESIZE * 1.5 / 2, myPlane.planepos.y - PLANESIZE * 1.5 / 2, &Img[1], SRCINVERT);
	// 绘制链表中的敌机
	PLANE* p1 = *enemy;
	while (p1 != NULL)
	{
		/*putimage(p1->planepos.x - PLANESIZE / 2, p1->planepos.y - PLANESIZE / 2, &Img[6], NOTSRCERASE);

		putimage(p1->planepos.x - PLANESIZE / 2, p1->planepos.y - PLANESIZE / 2, &Img[2], SRCINVERT);*/
		double angle = -atan2(myPlane.planepos.y - p1->planepos.y, myPlane.planepos.x - p1->planepos.x);
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
	for (i = 0; i < myPlane.bulletnum; i++)
	{

		rotateimage(&rotateimg[0], &Img[3], bulletangle);




		putimage(myPlane.planebullet[i].x, myPlane.planebullet[i].y, &rotateimg[0], SRCINVERT);
	}
	RECT rect1 = { 0,PLANESIZE,WIDTH,PLANESIZE * 3 };
	RECT rect2 = { 0,0,WIDTH,PLANESIZE * 3 };
	RECT rect3 = { myPlane.planepos.x - (PLANESIZE),myPlane.planepos.y + (PLANESIZE * 1.5 / 2),myPlane.planepos.x + (PLANESIZE * 1.5 / 2),myPlane.planepos.y + (PLANESIZE) + PLANESIZE };

	setbkmode(TRANSPARENT);
	char str1[100];
	char str2[100];
	char str3[100];

	sprintf(str1, "score:%lld", myPlane.Score);
	sprintf(str2, "highestscore:%lld", myPlane.HighScore);
	sprintf(str3, "health:%d", myPlane.health);

	settextcolor(BLACK);
	drawtext(str1, &rect1, DT_TOP | DT_CENTER);
	drawtext(str2, &rect2, DT_TOP | DT_CENTER);
	settextcolor(RED);
	drawtext(str3, &rect3, DT_TOP | DT_CENTER);
	//setbkmode(TRANSPARENT);
	if (flag)
	{

		RECT rect2 = { 0, 0, WIDTH, PLANESIZE };

		drawtext("暂停中", &rect2, DT_TOP | DT_LEFT);
	}
	EndBatchDraw();

}

void Move(PLANE** enemy, BARRIER** barrier, const PotentialField& pof)
{
	Testendtime= time(NULL);
	if (Testendtime- Teststartime>=UTIME)
	{
		//Record(sumtestnum, colcount);
		sumtestnum++;
		colcount = 0;
		Teststartime = Testendtime;
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
		exit(0);
	}
	
		ExMessage msg;
	if (peekmessage(&msg, EM_MOUSE))
	{
		if (msg.message == WM_LBUTTONDOWN)
		{
			if (myPlane.bulletnum + 1 <= BULLETNUM)
			{
				int x = myPlane.planepos.x;
				int y = myPlane.planepos.y;
				myPlane.planebullet[myPlane.bulletnum].x = x - PLANESIZE / 7;
				myPlane.planebullet[myPlane.bulletnum].y = y - PLANESIZE / 4;
				myPlane.bulletnum++;
			}
		}
	}

	*enemy = initenemy(*enemy);


	if (GetAsyncKeyState('W') & 0x8000 && myPlane.planepos.y > 0)
	{
		myPlane.planepos.y -= PLANESPEED;
	}
	if (GetAsyncKeyState('A') & 0x8000 && myPlane.planepos.x > 0)
	{
		myPlane.planepos.x -= PLANESPEED;
	}
	if (GetAsyncKeyState('S') & 0x8000 && myPlane.planepos.y < HEIGHT - PLANESIZE)
	{
		myPlane.planepos.y += PLANESPEED;
	}
	if (GetAsyncKeyState('D') & 0x8000 && myPlane.planepos.x < WIDTH - PLANESIZE)
	{
		myPlane.planepos.x += PLANESPEED;
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
		POS nextPos = getNextMove(p, myPlane.planepos, barrierPositions, barrierVec, WIDTH, HEIGHT, PLANESIZE + (PLANESIZE / 2),pof);

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
	for (i = 0;i < myPlane.bulletnum;i++)
	{
		myPlane.planebullet[i].y -= BULLETSPEED;
	}
	destroyplane(enemy, barrier);
	destorybullet(enemy, barrier);

	// 更新背景偏移量
	backgroundOffset += 1;  // 调整这个值可以改变背景移动的速度
	if (backgroundOffset >= HEIGHT) {
		backgroundOffset = 0;  // 重置偏移量以实现循环滚动
	}
	//障碍物移动
	barrmove(barrier);
	AdjustScore(&myPlane);
}
PLANE* initenemy(PLANE* enemy)
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
	endtime = GetTickCount();
	double timegap = (endtime - startime) * 1.0 / 1000;
	//printf("%lf\n", timegap);
	if (timegap >= ENEMYPROSPEED)
	{
		if (EnemyCurNum < ENEMYNUM)
		{
			PLANE* p = (PLANE*)malloc(sizeof(PLANE));
			if (p == NULL) {
				return h;
			}
			p->health = 1;
			p->planespeed = ENEMYFLYSPEED * 2;
			double x = rand() % (WIDTH - 75) + PLANESIZE / 2;
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
			EnemyCurNum++;
		}
		startime = endtime;
	}
	return h;

}
// 删除health <= 0的敌机
void delezeroe(PLANE** enemy)
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
			EnemyCurNum--;
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
void destroyplane(PLANE** enemy, BARRIER** barrier)
{


	PLANE* p1 = *enemy;
	while (p1 != NULL)
	{
		int	collisionRadius1 = PLANESIZE / 2 + (PLANESIZE * 1.5 / 2) - 20;

		if (isintersect(p1->planepos, myPlane.planepos, collisionRadius1))
		{
			colcount++;
			p1->health = 0;
			Colendtime = time(NULL);
			int timediv = Colendtime - Colstartime;
			if (timediv >= 1)
			{
				myPlane.health -= 5;
				Colstartime = Colendtime;
			}
			if (myPlane.health <= 0)
			{
				if (IDYES == MessageBox(GetHWnd(), "游戏结束，是否要重新开始游戏", "提示", MB_YESNO))
				{
					initgame(enemy, barrier);
					break;
				}
				else
				{
					exit(0);
				}
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

		if (isintersect(p2->barrierpos, myPlane.planepos, collisionRadius2))
		{
			myPlane.health = 0;
			if (myPlane.health <= 0)
			{
				if (IDYES == MessageBox(GetHWnd(), "游戏结束，是否要重新开始游戏", "提示", MB_YESNO))
				{
					initgame(enemy, barrier);
					break;
				}
				else
				{
					exit(0);
				}
			}
		}
		p2 = p2->next;
	}
	delezeroe(enemy);
	

}
// 销毁子弹和敌机
void destorybullet(PLANE** enemy, BARRIER** barrier)
{

	PLANE* p1 = *enemy;
	BARRIER* p2 = *barrier;
	

	while (p1 != NULL)
	{
		int collisionRadius3 = PLANESIZE / 5;
		PLANE* p3 = p1->next;
		while (p3 != NULL)
		{
			if (isintersect(p1->planepos, p3->planepos, collisionRadius3) && p1->planepos.y > PLANESIZE && p3->planepos.y > PLANESIZE)
			{
				
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
				p1->health = 0;
				//printf("--------------------------\n");
				break;
			}
			p2 = p2->next;
		}
		p1 = p1->next;
	}

	for (int i = 0; i < myPlane.bulletnum; i++)
	{
		p1 = *enemy;
		while (p1 != NULL && p1->health != 0)
		{
			int	collisionRadius1 = PLANESIZE / 2 + PLANESIZE / 6;


			// 判定是否碰撞
			if (isintersect(p1->planepos, myPlane.planebullet[i], collisionRadius1))
			{
				p1->health = 0;
				myPlane.Score++;
				for (int k = i; k < myPlane.bulletnum - 1; k++)
				{
					myPlane.planebullet[k] = myPlane.planebullet[k + 1];
				}
				myPlane.bulletnum--;
				i--;
				break;
			}
			p1 = p1->next;
		}


		// 删除超出屏幕的子弹
		if (myPlane.planebullet[i].y < 0 || myPlane.planebullet[i].x < 0 || myPlane.planebullet[i].x > WIDTH)
		{
			for (int k = i; k < myPlane.bulletnum - 1; k++)
			{
				myPlane.planebullet[k] = myPlane.planebullet[k + 1];
			}
			myPlane.bulletnum--;
			i--;
			continue;
		}
		p2 = *barrier;
		int	collisionRadius3 = PLANESIZE + PLANESIZE / 6;
		while (p2 != NULL)
		{
			//printf("--------------------------\n");
			if (isintersect(myPlane.planebullet[i], p2->barrierpos, collisionRadius3))
			{
				for (int k = i; k < myPlane.bulletnum - 1; k++)
				{
					myPlane.planebullet[k] = myPlane.planebullet[k + 1];
				}
				myPlane.bulletnum--;
				i--;
				break;
			}
			p2 = p2->next;
		}

		




	}

	delezeroe(enemy);
	

}
void AdjustScore(PLANE* myPlane)
{
	if (myPlane->Score > myPlane->HighScore)
	{
		myPlane->HighScore = myPlane->Score;
		FILE* pf = fopen("./res/score.txt", "wb");
		fwrite(&(myPlane->HighScore), sizeof(long long), 1, pf);
		fclose(pf);
		pf = NULL;
	}
}



