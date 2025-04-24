#define _CRT_SECURE_NO_WARNINGS
#include"game.h"

//预测障碍物
std::vector<POS> Predictpos(const std::vector<POS>& barriers, const std::vector<Vec2d>& barrierVec)
{
	std::vector<POS>futurepos;
	for (size_t i = 0; i < barriers.size(); ++i) 
	{
		const auto& barrier = barriers[i];
		const auto& barriervec = barrierVec[i];
		// 使用 barrier 和 barrierVec
	
		double x = barrier.x + barriervec.dx;
		double y = barrier.y + barriervec.dy;
		POS temp = { x,y };
		futurepos.push_back(temp);
	}
	return futurepos;
}

// 计算目标点的引力
Vec2d calculateAttraction(const POS& current, const POS& goal,const PotentialField&pof) {
    Vec2d force;
    force.dx = (goal.x - current.x) * pof.goal_gain;
    force.dy = (goal.y - current.y) * pof.goal_gain;
    return force;
}

// 计算所有障碍物的斥力
Vec2d calculateRepulsion(const POS& current, const std::vector<POS>& barriers, double radius, const PotentialField& pof, PLANE* enemy) {
    Vec2d total_force = { 0, 0 };
    for (const auto& barrier : barriers) {
        double dist = calculateDistance(current.x, current.y, barrier.x, barrier.y)-PLANESIZE-PLANESIZE/2;
		//printf("%lf      %lf\n", dist,radius);

        if (dist < radius) {
			double strength = pof.obstacle_gain * (1.0 / dist - 1.0 / radius) / (dist * dist);
			//printf("%lf\n", strength);
            Vec2d force;
            force.dx = (current.x - barrier.x) * strength;
            force.dy = (current.y - barrier.y) * strength;
			//printf("%lf\n", (current.x - barrier.x));

            total_force.dx += force.dx;
            total_force.dy += force.dy;
        }
    }
	PLANE* p = enemy;
	while (p != NULL)
	{
		double dist = calculateDistance(current.x, current.y, p->planepos.x, p->planepos.y) - PLANESIZE;
		if (dist < PLANESIZE / 5) {
			double strength = fabs(pof.plane_gain * (1.0 / (dist + 1e-5) /*- 1.0 / (PLANESIZE / 10)*/) / (dist * dist));

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

// 合成总力并限制大小
Vec2d getTotalForce(const POS& current, const POS& goal, const std::vector<POS>& barriers, double radius, const PotentialField& pof, PLANE* enemy) {
    Vec2d attraction = calculateAttraction(current, goal,pof);
    Vec2d repulsion = calculateRepulsion(current, barriers, radius,pof, enemy);
	
    Vec2d total_force;
    total_force.dx = attraction.dx + repulsion.dx;
    total_force.dy = attraction.dy + repulsion.dy;

    // 限制最大力
    double force_mag = sqrt(total_force.dx * total_force.dx + total_force.dy * total_force.dy);
    if (force_mag > pof.max_force) {
        total_force.dx = (total_force.dx / force_mag) * pof.max_force;
        total_force.dy = (total_force.dy / force_mag) * pof.max_force;
    }
    return total_force;
}
// 辅助函数：计算两点之间的距离
double calculateDistance(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}








POS getNextMove(plane* enemy, POS targetPos, const std::vector<POS>& barriers, std::vector<Vec2d> barrierVec, int width, int height, int barrierRadius, const PotentialField& pof) 
{
	if (enemy == nullptr) {
		return { 0, 0 };
	}



	std::vector<POS>futurepos = Predictpos(barriers, barrierVec);
	POS currentPos = enemy->planepos;
	POS nextPos = currentPos;

	Vec2d total_force = getTotalForce(currentPos, targetPos, futurepos, barrierRadius + 50, pof, enemy);
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





	// 确保返回的位置在屏幕范围内
	if (nextPos.x < 0 ) nextPos.x = 0;
	if (nextPos.x > 0 + width) nextPos.x = width;
	if (nextPos.y < 0) nextPos.y = 0;
	if (nextPos.y > height) nextPos.y = height;


	return nextPos;
}
