#pragma once
#include <stdio.h> 
#include <time.h>
#include<easyx.h>
#include<graphics.h>
#include<stdlib.h>
#include<math.h>
#include<conio.h>
#include<Windows.h>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
//窗口大小
#define WIDTH 600
#define HEIGHT 800
//飞机大小
#define PLANESIZE 50
//飞机速度
#define PLANESPEED 10
//敌机数量
#define ENEMYNUM 500
//敌机产生速度
#define ENEMYPROSPEED 0.3
//敌机飞行速度
#define ENEMYFLYSPEED 5
//子弹数量
#define BULLETNUM 1000
//子弹速度
#define BULLETSPEED 4
//子弹旋转角度
#define BULLETANGLE 10*3.1415926/180
//障碍物数量
#define MAXBARNUM 3
//障碍物移动速度
#define BARRIERSPEED 1
//测试时单位时间/s
#define UTIME 3
//坐标
typedef struct pos
{
	double x;
	double y;

}POS;
//势场法模块
typedef struct v
{
	double dx;
	double dy;
} Vec2d;
typedef struct PoF
{
	double goal_gain;    // 目标引力增益系数
	double obstacle_gain; // 障碍斥力增益系数
	double plane_gain;   // 敌机斥力增益系数
	double max_force;     // 最大合成力限制
} PotentialField;

//飞机结构体
typedef struct plane
{
	POS planepos;
	POS planebullet[BULLETNUM];
	int bulletnum;
	int bulletspeed;
	int health;
	int planespeed;
	int Score;
	long long HighScore;

	struct plane* next;

	POS moveHistory[10];// 记录移动历史
	int Historynum;
	int Historycurnum;
}PLANE;
//障碍结构体
typedef struct barrier
{
	POS barrierpos;
	float dx, dy;
	int radius;
	struct barrier* next;
}BARRIER;

//我的飞机
extern PLANE myPlane;
//敌机当前数量
extern int EnemyCurNum;
//每产生一个敌机的开始和结束时间
extern DWORD startime, endtime;
extern time_t Colstartime, Colendtime, Teststartime, Testendtime;

//图片数组
extern IMAGE Img[10];


//反弹球当前数量
extern int n;
extern PLANE* enemy ;
extern BARRIER* barrier;
extern int backgroundOffset;
extern bool flag;
extern bool Spacekeypressed;
extern int sumtestnum, colcount;
//函数声明
//初始化游戏
void initgame(PLANE** enemy, BARRIER** barrier);
//批量绘制
void initdraw(PLANE** enemy, BARRIER** barrier);
//主体移动
void Move(PLANE** enemy, BARRIER** barrier, const PotentialField& pof);
//初始化敌机
PLANE* initenemy(PLANE* enemy);
//删敌机
void delezeroe(PLANE** enemy);
//碰撞判定
int isintersect(POS a, POS b, int r);
//销毁检测
void destroyplane(PLANE** enemy, BARRIER** barrier);
void destorybullet(PLANE** enemy, BARRIER** barrier);
//生成障碍
BARRIER* createbarrier();
//计算反弹后的矢量速度


// 计算敌机安全移动方向
POS getNextMove(PLANE* enemy, POS targetPos, const std::vector<POS>& barriers, std::vector<Vec2d> barrierVec, int width, int height, int barrierRadius, const PotentialField& pof);
// 辅助函数：计算两点之间的距离
double calculateDistance(double x1, double y1, double x2, double y2);



//障碍物移动
void barrmove(BARRIER** barrier);
void AdjustScore(PLANE* myPlane);
Vec2d calculateAttraction(const POS& current, const POS& goal, const PotentialField& pof);

Vec2d calculateRepulsion(const POS& current, const std::vector<POS>& barriers, double radius, const PotentialField& pof, PLANE* enemy);
Vec2d getTotalForce(const POS& current, const POS& goal, const std::vector<POS>& barriers, double radius, const PotentialField& pof, PLANE* enemy);



