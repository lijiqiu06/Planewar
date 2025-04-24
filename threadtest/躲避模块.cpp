#define _CRT_SECURE_NO_WARNINGS
#include"game.h"

//Ԥ���ϰ���
std::vector<POS> Predictpos(const std::vector<POS>& barriers, const std::vector<Vec2d>& barrierVec)
{
	std::vector<POS>futurepos;
	for (size_t i = 0; i < barriers.size(); ++i) 
	{
		const auto& barrier = barriers[i];
		const auto& barriervec = barrierVec[i];
		// ʹ�� barrier �� barrierVec
	
		double x = barrier.x + barriervec.dx;
		double y = barrier.y + barriervec.dy;
		POS temp = { x,y };
		futurepos.push_back(temp);
	}
	return futurepos;
}
double calculateObstacleDensity(const POS& current, const std::vector<POS>& barriers, double radius) {
	int count = 0;
	for (const auto& barrier : barriers) {
		if (calculateDistance(current.x, current.y, barrier.x, barrier.y) < radius) {
			count++;
		}
	}
	return static_cast<double>(count) / (3.14 * radius * radius); // ��λ����ϰ�������
}
// ����Ŀ��������
Vec2d calculateAttraction(const POS& current, const POS& goal,const PotentialField&pof) {
    Vec2d force;
    force.dx = (goal.x - current.x) * pof.goal_gain;
    force.dy = (goal.y - current.y) * pof.goal_gain;
    return force;
}

// ���������ϰ���ĳ���
Vec2d calculateRepulsion(const POS& current, const std::vector<POS>& barriers, double radius, const PotentialField& pof,PLANE *enemy) {
    Vec2d total_force = { 0, 0 };
    for (const auto& barrier : barriers) {
        double dist = calculateDistance(current.x, current.y, barrier.x, barrier.y)-PLANESIZE-PLANESIZE/2;
        if (dist < radius) {
			double strength = pof.obstacle_gain * (1.0 / (dist + 1e-5) - 1.0 / radius) / (dist * dist);
			
            Vec2d force;
            force.dx = (current.x - barrier.x) * strength;
            force.dy = (current.y - barrier.y) * strength;
            total_force.dx += force.dx;
            total_force.dy += force.dy;
        }
    }
	PLANE* p = enemy;
	while (p != NULL)
	{
		if (p == enemy) { // ��������
			p = p->next;
			continue;
		}
		double dist = calculateDistance(current.x, current.y, p->planepos.x, p->planepos.y) - PLANESIZE ;
		if (dist < PLANESIZE/5) {
			double strength = fabs(pof.plane_gain * (1.0 / (dist + 1e-5)/* - 1.0 / (PLANESIZE * 1.5)*/) / (dist * dist));

			Vec2d force;
			force.dx = (current.x - p->planepos.x) * strength;
			force.dy = (current.y - p->planepos.y) * strength;
			total_force.dx += force.dx;
			total_force.dy += force.dy;
		}
		p = p->next;
	}
    return total_force;
}

// �ϳ����������ƴ�С
Vec2d getTotalForce(const POS& current, const POS& goal, const std::vector<POS>& barriers, double radius, const PotentialField& pof,PLANE*enemy) {
    Vec2d attraction = calculateAttraction(current, goal,pof);
    Vec2d repulsion = calculateRepulsion(current, barriers, radius,pof,enemy);
    Vec2d total_force;
    total_force.dx = attraction.dx + repulsion.dx;
    total_force.dy = attraction.dy + repulsion.dy;

    // ���������
    double force_mag = sqrt(total_force.dx * total_force.dx + total_force.dy * total_force.dy);
    if (force_mag > pof.max_force) {
        total_force.dx = (total_force.dx / force_mag) * pof.max_force;
        total_force.dy = (total_force.dy / force_mag) * pof.max_force;
    }
    return total_force;
}
// ������������������֮��ľ���
double calculateDistance(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// �������������λ���Ƿ�ȫ������ײ���ϰ��
bool isSafePosition(const pos& position, const std::vector<pos>& barriers, int barrierRadius) {
	for (const auto& barrier : barriers) {
		if (calculateDistance(position.x, position.y, barrier.x, barrier.y) < barrierRadius + 10) { // ���Ӱ�ȫ����
			return false;
		}
	}
	return true;
}
bool isSafePosition2(const pos& position, plane* enemy, int Radius)
{
	PLANE* p = enemy;
	while (p->next != NULL)
	{
		if (calculateDistance(position.x, position.y, p->planepos.x, p->planepos.y) <= 10)
		{
			p = p->next;
			continue;
		}
		if (calculateDistance(position.x, position.y, p->planepos.x, p->planepos.y) < Radius+20) { // ���Ӱ�ȫ����
			return false;
		}
		p = p->next;
	}
	return true;
}

// �����������ҵ�������ϰ���
pos findNearestBarrier(const pos& position, const std::vector<pos>& barriers) {
	pos nearest = { 0, 0 };
	double minDist = 1e9;
	for (const auto& barrier : barriers) {
		double dist = calculateDistance(position.x, position.y, barrier.x, barrier.y);
		if (dist < minDist) {
			minDist = dist;
			nearest = barrier;
		}
	}
	return nearest;
}

// ���λ���Ƿ����ƶ���ʷ��
bool isInHistory(const POS& position, const PLANE* enemy) {
	for (int i = 0;i < enemy->Historynum;i++)
	{
		if (enemy->moveHistory[i].x == position.x && enemy->moveHistory[i].y == position.y) {
			return true;
		}
	}
	return false;
}


// ��Ҫ��Ѱ·����
POS getNextMove2(plane* enemy, POS targetPos, const std::vector<POS>& barriers, std::vector<Vec2d> barrierVec, int width, int height, int barrierRadius, const PotentialField& pof, int offsetX) {
	if (enemy == nullptr) {
		return { 0, 0 };
	}
	/*for (const auto& barrier : barriers) 
	{
		printf("%lf,%lf\n",barrier.x, barrier.y);
		break;
	}*/
	
	std::vector<POS>futurepos = Predictpos(barriers, barrierVec);
	POS currentPos = enemy->planepos;
	POS nextPos = currentPos;
	/*for (const auto& futureposc : futurepos)
	{
		printf("%lf,%lf\n", futureposc.x, futureposc.y);
		break;
	}*/
	// ���㵽Ŀ��ķ�������
	Vec2d total_force = getTotalForce(currentPos, targetPos,  futurepos, barrierRadius + 10,pof,enemy);
	double dx = total_force.dx;
	double dy = total_force.dy;

	

	// ��̬Ȩ�ص���(�ϰ�������)
	double density = calculateObstacleDensity(currentPos, futurepos, 100.0);
	double weight_gain = 1.0 + density * 2.0;

	// Ѱ��������ϰ���
	pos nearestBarrier = findNearestBarrier(currentPos,  futurepos);
	double barrierDist = calculateDistance(currentPos.x, currentPos.y, nearestBarrier.x, nearestBarrier.y);

	// ����ӽ��ϰ�������������
	if (barrierDist < barrierRadius + 150) // ���ӱ��ϴ�������
	{
		double obstacleDx = currentPos.x - nearestBarrier.x;
		double obstacleDy = currentPos.y - nearestBarrier.y;
		double obstacleDistance = sqrt(obstacleDx * obstacleDx + obstacleDy * obstacleDy);

		if (obstacleDistance > 0) {
			// ��׼����������
			obstacleDx /= obstacleDistance;
			obstacleDy /= obstacleDistance;

			// ���ݾ����������Ȩ��
			double weight = (barrierRadius + 100 - barrierDist) / (barrierRadius + 100);
			weight = weight * 2.0*weight_gain; // ʹȨ�ر仯����ƽ��
			if (weight > 1.0)weight = 1.0;
			// �ϲ�Ŀ�귽��ͱ��Ϸ���
			dx = dx * (1 - weight) + obstacleDx * weight;
			dy = dy * (1 - weight) + obstacleDy * weight;

			// ���±�׼���ϲ���ķ�������
			double newDist = sqrt(dx * dx + dy * dy);
			if (newDist > 0) {
				dx /= newDist;
				dy /= newDist;
			}
		}
	}

	// ���������ܵ��ƶ�����
	const int NUM_DIRECTIONS = 32; // ���ӷ��������Ի�ø�ƽ�����ƶ�
	double bestDistance = 1e9;
	bool foundSafeMove = false;

	for (int i = 0; i < NUM_DIRECTIONS; i++) {
		double angle = (2 * 3.14159 * i / NUM_DIRECTIONS) + atan2(dy, dx);
		double testDx = cos(angle);
		double testDy = sin(angle);

		POS testPos = {
			currentPos.x + testDx * enemy->planespeed,
			currentPos.y + testDy * enemy->planespeed
		};

		// ȷ��λ������Ļ��Χ��
		if (testPos.x < offsetX || testPos.x > offsetX+width || testPos.y < 0 || testPos.y > height) {
			continue;
		}

		// �����������Ƿ�ȫ
		if (isSafePosition(testPos, futurepos, barrierRadius) /*&& isSafePosition2(testPos, enemy, PLANESIZE)*/ && !isInHistory(testPos, enemy))
		{
			double testDistance = calculateDistance(testPos.x, testPos.y, targetPos.x, targetPos.y);
			if (!foundSafeMove || testDistance < bestDistance) {
				bestDistance = testDistance;
				nextPos = testPos;
				foundSafeMove = true;
			}
		}
	}



	// ȷ�����ص�λ������Ļ��Χ��
	if (nextPos.x < offsetX) nextPos.x = 0;
	if (nextPos.x > offsetX+width) nextPos.x = width;
	if (nextPos.y < 0) nextPos.y = 0;
	if (nextPos.y > height) nextPos.y = height;

	// �����ƶ���ʷ
	enemy->moveHistory[enemy->Historycurnum] = nextPos;
	enemy->Historycurnum = (enemy->Historycurnum + 1) % 10;
	if (enemy->Historynum < 10)
	{
		(enemy->Historynum)++;
	}
	return nextPos;
}

POS getNextMove1(plane* enemy, POS targetPos, const std::vector<POS>& barriers, std::vector<Vec2d> barrierVec, int width, int height, int barrierRadius, const PotentialField& pof, int offsetX) {
	if (enemy == nullptr) {
		return { 0, 0 };
	}



	std::vector<POS>futurepos = Predictpos(barriers, barrierVec);
	POS currentPos = enemy->planepos;
	POS nextPos = currentPos;
	
	Vec2d total_force = getTotalForce(currentPos, targetPos, futurepos, barrierRadius + 10, pof,enemy);
	double dx = total_force.dx;
	double dy = total_force.dy;


	




	
		double angle = atan2(dy, dx);
		double testDx = cos(angle);
		double testDy = sin(angle);

		POS testPos = {
			currentPos.x + testDx * enemy->planespeed,
			currentPos.y + testDy * enemy->planespeed
		};

		
		nextPos = testPos;
		
	



	// ȷ�����ص�λ������Ļ��Χ��
	if (nextPos.x < offsetX) nextPos.x = offsetX;
	if (nextPos.x > offsetX+width) nextPos.x = offsetX+width;
	if (nextPos.y < 0) nextPos.y = 0;
	if (nextPos.y > height) nextPos.y = height;

	
	return nextPos;
}




POS getNextMove3(plane* enemy, POS targetPos, const std::vector<POS>& barriers, std::vector<Vec2d> barrierVec, int width, int height, int barrierRadius, const PotentialField& pof, int offsetX) {
	if (enemy == nullptr) {
		return { 0, 0 };
	}
	

	//std::vector<POS>futurepos = Predictpos(barriers, barrierVec);
	POS currentPos = enemy->planepos;
	POS nextPos = currentPos;

	// ���㵽Ŀ��ķ�������
	double dx = targetPos.x - currentPos.x;
	double dy = targetPos.y - currentPos.y;
	double distance = calculateDistance(currentPos.x, currentPos.y, targetPos.x, targetPos.y);

	if (distance < 1) {
		return currentPos;
	}

	// ��׼����������
	dx /= distance;
	dy /= distance;





	// ���������ܵ��ƶ�����
	const int NUM_DIRECTIONS = 32; // ���ӷ��������Ի�ø�ƽ�����ƶ�
	double bestDistance = 1e9;
	bool foundSafeMove = false;

	for (int i = 0; i < NUM_DIRECTIONS; i++) {
		double angle = (2 * 3.14159 * i / NUM_DIRECTIONS) + atan2(dy, dx);
		double testDx = cos(angle);
		double testDy = sin(angle);

		POS testPos = {
			currentPos.x + testDx * enemy->planespeed,
			currentPos.y + testDy * enemy->planespeed
		};

		// ȷ��λ������Ļ��Χ��
		if (testPos.x < offsetX + 15 || testPos.x > offsetX + width || testPos.y < 0 || testPos.y > height) {
			continue;
		}

		// �����������Ƿ�ȫ
		if (isSafePosition(testPos, barriers, barrierRadius) &&isSafePosition2(testPos, enemy, PLANESIZE) /*&& !isInHistory(testPos, enemy)*/)
		{
			double testDistance = calculateDistance(testPos.x, testPos.y, targetPos.x, targetPos.y);
			if (!foundSafeMove || testDistance < bestDistance) {
				bestDistance = testDistance;
				nextPos = testPos;
				foundSafeMove = true;
			}
		}
	}



	// ȷ�����ص�λ������Ļ��Χ��
	if (nextPos.x < offsetX) nextPos.x = 0;
	if (nextPos.x > offsetX + width) nextPos.x = width;
	if (nextPos.y < 0) nextPos.y = 0;
	if (nextPos.y > height) nextPos.y = height;

	// �����ƶ���ʷ
	enemy->moveHistory[enemy->Historycurnum] = nextPos;
	enemy->Historycurnum = (enemy->Historycurnum + 1) % 10;
	if (enemy->Historynum < 10)
	{
		(enemy->Historynum)++;
	}
	return nextPos;
}




















