#include <string>
#include <iostream>
#include <fstream>
#include "openGA.hpp"
#include"game.h"
using std::string;
using std::cout;
using std::endl;
PLANE myPlane;
int EnemyCurNum;
DWORD startime, endtime;
time_t Colstartime, Colendtime, Teststartime, Testendtime;
IMAGE Img[10];
PLANE* enemy = NULL;
BARRIER* barrier = NULL;
int n;
int backgroundOffset;
bool flag = false;
bool Spacekeypressed = false;
int sumtestnum, colcount, diecount,diecount2;
bool gamerunning = true;
struct MySolution
{
    double goal_gain;
    double obstacle_gain;
    double max_force;
	double plane_gain;
    string to_string() const
    {
        return
            string("{")
            + "goal_gain:" + std::to_string(goal_gain)
            + ", obstacle_gain:" + std::to_string(obstacle_gain)
			+ ", plane_gain:" + std::to_string(plane_gain)
            + ", max_force:" + std::to_string(max_force)
            + "}";
    }
};

struct MyMiddleCost
{
    // This is where the results of simulation
    // is stored but not yet finalized.
    double objective1;
};

typedef EA::Genetic<MySolution, MyMiddleCost> GA_Type;
typedef EA::GenerationType<MySolution, MyMiddleCost> Generation_Type;

void init_genes(MySolution& p, const std::function<double(void)>& rnd01)
{
    // rnd01() gives a random number in 0~1
    p.goal_gain = 0.1 + 9.9 * rnd01();
    int n = rand() % 4;
   
    p.obstacle_gain = 999 + 999000 * rnd01();
    p.plane_gain= 999 + 999000 * rnd01();
   
    
    p.max_force = 1.0 + 49.0 * rnd01();
}

// 假设这是游戏模拟函数，需要根据实际情况修改
// 这里只是一个示例，实际需要调用你的游戏逻辑
// 统计到达目标数和碰撞数


double simulate_game(const MySolution& p, PLANE** enemy, BARRIER** barrier)
{
    PotentialField pof = { p.goal_gain, p.obstacle_gain,p.plane_gain, p.max_force };
    
   

    initgame(enemy, barrier);
	time_t starttime = time(NULL);
        while (gamerunning)
        {
            
            initdraw(enemy, barrier);
            Move(enemy, barrier, pof);
			time_t endtime = time(NULL);
            if(endtime-starttime>=30)
			{
				gamerunning = false;
			}   
			Sleep(1);
        }
	printf("colcount:%d\n", colcount);
	printf("diecount:%d\n", diecount);
	printf("diecount2:%d\n", diecount2);
	printf("%lf %lf %lf %lf\n", pof.goal_gain, pof.obstacle_gain,pof.plane_gain, pof.max_force);
    gamerunning = true;
    // 适应度
    return -1.0*(1* colcount-0.01 * diecount-0.05*diecount2 ) / 30;
}

bool eval_solution(
    const MySolution& p,
    MyMiddleCost& c)
{
    c.objective1 = simulate_game(p, &enemy, &barrier);
    if (colcount > 0) colcount = 0;
	if (diecount > 0) diecount = 0;
	if (diecount2 > 0) diecount2 = 0;
    return true; // solution is accepted
}

MySolution mutate(
    const MySolution& X_base,
    const std::function<double(void)>& rnd01,
    double shrink_scale)
{
    MySolution X_new;
    const double mu = 0.2 * shrink_scale; // mutation radius (adjustable)
    bool in_range;
    do {
        in_range = true;
        X_new = X_base;
        X_new.goal_gain += mu * (rnd01() - rnd01());
        in_range = in_range && (X_new.goal_gain >= 0.1 && X_new.goal_gain < 10.0);
        X_new.obstacle_gain += mu * (rnd01() - rnd01());
        /*double log_obstacle = log(X_base.obstacle_gain);
        log_obstacle += mu*0.05 * (rnd01() - rnd01());
        X_new.obstacle_gain = exp(log_obstacle);*/
        in_range = in_range && (X_new.obstacle_gain >= 10.0 && X_new.obstacle_gain < 999999.0);

        X_new.plane_gain += mu * (rnd01() - rnd01());

        /*double log_plane = log(X_base.plane_gain);
        log_plane += mu*0.05 * (rnd01() - rnd01());
        X_new.plane_gain = exp(log_plane);*/
        in_range = in_range && (X_new.plane_gain >= 10.0 && X_new.plane_gain < 999999.0);

        X_new.max_force += mu * (rnd01() - rnd01());
        in_range = in_range && (X_new.max_force >= 1.0 && X_new.max_force < 50.0);
    } while (!in_range);
    return X_new;
}

MySolution crossover(
    const MySolution& X1,
    const MySolution& X2,
    const std::function<double(void)>& rnd01)
{
    MySolution X_new;
    double r;
    r = rnd01();
    X_new.goal_gain = r * X1.goal_gain + (1.0 - r) * X2.goal_gain;
    r = rnd01();
    X_new.obstacle_gain = r * X1.obstacle_gain + (1.0 - r) * X2.obstacle_gain;
    /*double log_X1o = log(X1.obstacle_gain);
    double log_X2o = log(X2.obstacle_gain);
    X_new.obstacle_gain = exp(r * log_X1o + (1.0 - r) * log_X2o);*/

    X_new.plane_gain = r * X1.plane_gain + (1.0 - r) * X2.plane_gain;

    /*double log_X1p = log(X1.plane_gain);
    double log_X2p = log(X2.plane_gain);
    X_new.plane_gain = exp(r * log_X1p + (1.0 - r) * log_X2p);*/

    r = rnd01();
    X_new.max_force = r * X1.max_force + (1.0 - r) * X2.max_force;
    return X_new;
}

double calculate_SO_total_fitness(const GA_Type::thisChromosomeType& X)
{
    // finalize the cost
    double final_cost = 0.0;
    final_cost += X.middle_costs.objective1;
    return final_cost;
}

std::ofstream output_file;

void SO_report_generation(
    int generation_number,
    const EA::GenerationType<MySolution, MyMiddleCost>& last_generation,
    const MySolution& best_genes)
{
    cout
        << "Generation [" << generation_number << "], "
        << "Best=" << last_generation.best_total_cost << ", "
        << "Average=" << last_generation.average_cost << ", "
        << "Best genes=(" << best_genes.to_string() << ")" << ", "
        << "Exe_time=" << last_generation.exe_time
        << endl;

    output_file
        << generation_number << "\t"
        << last_generation.average_cost << "\t"
        << last_generation.best_total_cost << "\t"
        << best_genes.to_string() << "\n";
}

int main()
{
    loadimage(&Img[0], "./res/背景.png", WIDTH, HEIGHT);
    loadimage(&Img[1], "./res/myplane.jpg", PLANESIZE * 1.5, PLANESIZE * 1.5);
    loadimage(&Img[2], "./res/enemyplane.png", PLANESIZE, PLANESIZE);
    loadimage(&Img[3], "./res/子弹8.png", PLANESIZE / 3, PLANESIZE / 3);
    loadimage(&Img[4], "./res/障碍.png", PLANESIZE * 2, PLANESIZE * 2);
    loadimage(&Img[5], "./res/myplanebk.jpg", PLANESIZE * 1.5, PLANESIZE * 1.5);
    loadimage(&Img[6], "./res/enemyplanebk.jpg", PLANESIZE, PLANESIZE);
    initgraph(WIDTH, HEIGHT);
    
    output_file.open("results.txt");
    output_file << "step" << "\t" << "cost_avg" << "\t" << "cost_best" << "\t" << "solution_best" << "\n";

    EA::Chronometer timer;
    timer.tic();

    GA_Type ga_obj;
    ga_obj.problem_mode = EA::GA_MODE::SOGA;
    ga_obj.multi_threading = false;
    ga_obj.verbose = false;
    ga_obj.population = 10;
    ga_obj.generation_max = 200;
    ga_obj.calculate_SO_total_fitness = calculate_SO_total_fitness;
    ga_obj.init_genes = init_genes;
    ga_obj.eval_solution = eval_solution;
    ga_obj.mutate = mutate;
    ga_obj.crossover = crossover;
    ga_obj.SO_report_generation = SO_report_generation;
    ga_obj.crossover_fraction = 0.7;
    ga_obj.mutation_rate = 0.2;
    ga_obj.best_stall_max = 10;
    ga_obj.elite_count = 1;
    ga_obj.solve();

    cout << "The problem is optimized in " << timer.toc() << " seconds." << endl;

    output_file.close();
    return 0;
}