#define _CRT_SECURE_NO_WARNINGS
#include"game.h"
BARRIER* createbarrier(int offsetX)
{
	int i = 0;
	int j = 0;
	int sign = 1;
	BARRIER* p, * h = NULL, * r = NULL;

	// 定义障碍物之间的最小距离
	const int MIN_DISTANCE = PLANESIZE * 2;

	while (i < MAXBARNUM && j < 20)
	{
		p = (BARRIER*)malloc(sizeof(BARRIER));
		if (p == NULL) {
			// 内存分配失败，处理错误
			break;
		}

		int validPosition = 0;
		int maxAttempts = 50;  // 最大尝试次数，防止无限循环
		int attempts = 0;

		// 尝试找到一个不会与其他障碍物重叠的位置
		while (!validPosition && attempts < maxAttempts) {
			// 生成随机位置
			p->barrierpos.x = offsetX+(rand() % 400) + 100;
			p->barrierpos.y = (rand() % 500) + 100;
			p->dx = sign * ((double)rand() / RAND_MAX);
			p->dy = sign * ((double)rand() / RAND_MAX);
			p->radius = PLANESIZE;

			validPosition = 1;  // 假设当前位置有效

			// 检查是否与现有障碍物重叠
			BARRIER* temp = h;
			while (temp != NULL) {
				double detx = p->barrierpos.x - temp->barrierpos.x;
				double dety = p->barrierpos.y - temp->barrierpos.y;
				double distance = sqrt(detx * detx + dety * dety);

				// 如果距离小于最小距离，则位置无效
				if (distance < MIN_DISTANCE) {
					validPosition = 0;
					break;
				}
				temp = temp->next;
			}

			attempts++;
			/*printf("%d\n", attempts);*/
		}

		// 如果找不到有效位置，则跳过此障碍物
		if (!validPosition) {
			free(p);
			j++;
			continue;
		}
		else
		{
			sign = -sign;
			/*printf("%lf\n", p->dx);
			printf("---------------\n");*/

		}

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
		i++;

	}

	return h;
}
void barrmove(BARRIER** barrier, int offsetX)
{
	if (*barrier == NULL)
	{
		return;
	}
	BARRIER* b = *barrier;
	while (b != NULL)
	{
		b->barrierpos.x += b->dx* BARRIERSPEED;

		b->barrierpos.y += b->dy* BARRIERSPEED;

		// 边界碰撞（添加边界弹性）
		if (b->barrierpos.x - b->radius <= offsetX) {
			b->dx = fabs(b->dx);
			b->barrierpos.x = offsetX+b->radius;
		}
		if (b->barrierpos.x + b->radius >= offsetX+WIDTH) {
			b->dx = -fabs(b->dx);
			b->barrierpos.x = (offsetX + WIDTH) - b->radius;
		}
		if (b->barrierpos.y - b->radius <= 0) {
			b->dy = fabs(b->dy);
			b->barrierpos.y = b->radius;
		}
		if (b->barrierpos.y + b->radius >= HEIGHT) {
			b->dy = -fabs(b->dy);
			b->barrierpos.y = HEIGHT - b->radius;
		}


		// 圆形障碍物碰撞检测
		if (b->next == NULL)
		{
			break;
		}
		BARRIER* p2 = b->next;
		while (p2 != NULL)
		{
			float dx = b->barrierpos.x - p2->barrierpos.x;
			float dy = b->barrierpos.y - p2->barrierpos.y;
			float distance = sqrt(dx * dx + dy * dy);
			float minDistance = b->radius + p2->radius;

			if (distance < minDistance) {

				// 计算碰撞法线方向
				float nx = dx / distance;
				float ny = dy / distance;

				// 位置修正（防止嵌入）
				float correction = minDistance - distance;
				b->barrierpos.x += nx * correction * 0.5;
				b->barrierpos.y += ny * correction * 0.5;
				p2->barrierpos.x -= nx * correction * 0.5;
				p2->barrierpos.y -= ny * correction * 0.5;

				// 交换速度
				float tempDx = b->dx;
				float tempDy = b->dy;
				b->dx = p2->dx;
				b->dy = p2->dy;
				p2->dx = tempDx;
				p2->dy = tempDy;
			}
			p2 = p2->next;
		}


		b = b->next;
	}
}
