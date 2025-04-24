#pragma once
#include <stdio.h> 
#include <time.h>
#include<easyx.h>
#include<graphics.h>
#include<stdlib.h>
#include<math.h>
#include<conio.h>
#include<Windows.h>
#pragma comment(lib,"Winmm.lib")
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
#define ENEMYPROSPEED 0.01
//敌机飞行速度
#define ENEMYFLYSPEED 5
//子弹数量
#define BULLETNUM 1000
//子弹速度
#define BULLETSPEED 4
//子弹旋转角度
#define BULLETANGLE 10*3.1415926/180
//障碍物数量
#define MAXBARNUM 6	
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


	struct plane* next;

	POS moveHistory[10];// 记录移动历史
	int Historynum;
	int Historycurnum;
	int Colnum[2000];
	int dienum[2000];
	int dienum2[2000];
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
extern PLANE myPlane1, myPlane2;
//敌机当前数量
extern int EnemyCurNum1, EnemyCurNum2;
//每产生一个敌机的开始和结束时间
extern DWORD startime1, endtime1, startime2, endtime2;
extern time_t Colstartime1, Colendtime1, Colstartime2, Colendtime2, Teststartime1, Testendtime1, Teststartime2, Testendtime2;

//图片数组
extern IMAGE Img[10];




extern int backgroundOffset1, backgroundOffset2;
extern bool flag;
extern bool Spacekeypressed;
extern bool Enterkeypressed;
extern int sumtestnum1, colcount1,diecount1, sumtestnum2, colcount2, diecount2, diecountplus1, diecountplus2;
extern bool thread1Running ;
extern bool thread2Running ;
// 互斥锁
extern CRITICAL_SECTION cs;
//函数声明
//初始化游戏
void initgame(PLANE** enemy, BARRIER** barrier,PLANE* myPlane, int* EnemyCurNum, DWORD* startime, time_t* Colstartime,int offsetX, int* backgroundOffset,time_t* Teststartime, int* sumtestnum, int* colcount, int* diecount, int* diecountplus);
//批量绘制
void initdraw(PLANE** enemy, BARRIER** barrier，, PLANE* myPlane, int offsetX,int backgroundOffset);
//主体移动
void Move(PLANE** enemy, BARRIER** barrier, const PotentialField& pof, PLANE* myPlane, int* EnemyCurNum, DWORD* startime, time_t* Colstartime, time_t* Colendtime, int offsetX, int* backgroundOffset, time_t* Teststartime, time_t* Testendtime, int* sumtestnum, int* colcount, int* diecount, int* diecountplus);
//初始化敌机
PLANE* initenemy(PLANE* enemy, int* EnemyCurNum, DWORD* startime, DWORD* endtime, int offsetX);
//删敌机
void delezeroe(PLANE** enemy,int* n);

//碰撞判定
int isintersect(POS a, POS b, int r);
//销毁检测
void destroyplane(PLANE** enemy, BARRIER** barrier, PLANE* myPlane, time_t* Colstartime, time_t* Colendtime, int* EnemyCurNum, int offsetX, int* backgroundOffset, time_t* Teststartime, int* sumtestnum, int* colcount, int* diecount, int* diecountplus);
void destorybullet(PLANE** enemy, BARRIER** barrier, PLANE* myPlane, int* EnemyCurNum, int offsetX, int* diecount, int* diecountplus);
//生成障碍
BARRIER* createbarrier(int offsetX);
//计算反弹后的矢量速度
void reflectVector(float* dx, float* dy, float nx, float ny);

// 计算敌机安全移动方向
POS getNextMove1(PLANE* enemy, POS targetPos, const std::vector<POS>& barriers, std::vector<Vec2d> barrierVec, int width, int height, int barrierRadius, const PotentialField& pof, int offsetX);
POS getNextMove2(PLANE* enemy, POS targetPos, const std::vector<POS>& barriers, std::vector<Vec2d> barrierVec, int width, int height, int barrierRadius, const PotentialField& pof, int offsetX);
// 辅助函数：计算两点之间的距离
double calculateDistance(double x1, double y1, double x2, double y2);
// 辅助函数：检查位置是否安全（不会撞到障碍物）
bool isSafePosition(const POS& position, const std::vector<POS>& barriers, int barrierRadius);
// 辅助函数：找到最近的障碍物
pos findNearestBarrier(const POS& position, const std::vector<POS>& barriers);
// 检查位置是否在移动历史中
bool isInHistory(const POS& position, const PLANE* enemy);
//障碍物移动
void barrmove(BARRIER** barrier, int offsetX);
// 计算目标点的引力
Vec2d calculateAttraction(const POS& current, const POS& goal, const PotentialField& pof);
// 计算所有障碍物的斥力
Vec2d calculateRepulsion(const POS& current, const std::vector<POS>& barriers, double radius, const PotentialField& pof, PLANE* enemy);
// 合成总力并限制大小
Vec2d getTotalForce(const POS& current, const POS& goal, const std::vector<POS>& barriers, double radius, const PotentialField& pof, PLANE* enemy);
//预测障碍物
std::vector<POS> Predictpos(const std::vector<POS>& barriers, const std::vector<Vec2d>& barrierVec);
//记录数据
void Record(int sumtestnum, int colcount);
//清数据
void Clear();
void DrawoverPage(int offsetX, int data[200], int data2[200], int data3[200], int dataSize);
POS getNextMove3(plane* enemy, POS targetPos, const std::vector<POS>& barriers, std::vector<Vec2d> barrierVec, int width, int height, int barrierRadius, const PotentialField& pof, int offsetX);
bool isSafePosition2(const pos& position, plane* enemy, int Radius);


