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
//���ڴ�С
#define WIDTH 600
#define HEIGHT 800
//�ɻ���С
#define PLANESIZE 50
//�ɻ��ٶ�
#define PLANESPEED 10
//�л�����
#define ENEMYNUM 500
//�л������ٶ�
#define ENEMYPROSPEED 0.01
//�л������ٶ�
#define ENEMYFLYSPEED 5
//�ӵ�����
#define BULLETNUM 1000
//�ӵ��ٶ�
#define BULLETSPEED 4
//�ӵ���ת�Ƕ�
#define BULLETANGLE 10*3.1415926/180
//�ϰ�������
#define MAXBARNUM 6	
//�ϰ����ƶ��ٶ�
#define BARRIERSPEED 1
//����ʱ��λʱ��/s
#define UTIME 3
//����
typedef struct pos
{
	double x;
	double y;

}POS;
//�Ƴ���ģ��
typedef struct v
{
	double dx;
	double dy;
} Vec2d;
typedef struct PoF
{
	double goal_gain;    // Ŀ����������ϵ��
	double obstacle_gain; // �ϰ���������ϵ��
	double plane_gain;   // �л���������ϵ��
	double max_force;     // ���ϳ�������
} PotentialField;

//�ɻ��ṹ��
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

	POS moveHistory[10];// ��¼�ƶ���ʷ
	int Historynum;
	int Historycurnum;
	int Colnum[2000];
	int dienum[2000];
	int dienum2[2000];
}PLANE;
//�ϰ��ṹ��
typedef struct barrier
{
	POS barrierpos;
	float dx, dy;
	int radius;
	struct barrier* next;
}BARRIER;

//�ҵķɻ�
extern PLANE myPlane1, myPlane2;
//�л���ǰ����
extern int EnemyCurNum1, EnemyCurNum2;
//ÿ����һ���л��Ŀ�ʼ�ͽ���ʱ��
extern DWORD startime1, endtime1, startime2, endtime2;
extern time_t Colstartime1, Colendtime1, Colstartime2, Colendtime2, Teststartime1, Testendtime1, Teststartime2, Testendtime2;

//ͼƬ����
extern IMAGE Img[10];




extern int backgroundOffset1, backgroundOffset2;
extern bool flag;
extern bool Spacekeypressed;
extern bool Enterkeypressed;
extern int sumtestnum1, colcount1,diecount1, sumtestnum2, colcount2, diecount2, diecountplus1, diecountplus2;
extern bool thread1Running ;
extern bool thread2Running ;
// ������
extern CRITICAL_SECTION cs;
//��������
//��ʼ����Ϸ
void initgame(PLANE** enemy, BARRIER** barrier,PLANE* myPlane, int* EnemyCurNum, DWORD* startime, time_t* Colstartime,int offsetX, int* backgroundOffset,time_t* Teststartime, int* sumtestnum, int* colcount, int* diecount, int* diecountplus);
//��������
void initdraw(PLANE** enemy, BARRIER** barrier��, PLANE* myPlane, int offsetX,int backgroundOffset);
//�����ƶ�
void Move(PLANE** enemy, BARRIER** barrier, const PotentialField& pof, PLANE* myPlane, int* EnemyCurNum, DWORD* startime, time_t* Colstartime, time_t* Colendtime, int offsetX, int* backgroundOffset, time_t* Teststartime, time_t* Testendtime, int* sumtestnum, int* colcount, int* diecount, int* diecountplus);
//��ʼ���л�
PLANE* initenemy(PLANE* enemy, int* EnemyCurNum, DWORD* startime, DWORD* endtime, int offsetX);
//ɾ�л�
void delezeroe(PLANE** enemy,int* n);

//��ײ�ж�
int isintersect(POS a, POS b, int r);
//���ټ��
void destroyplane(PLANE** enemy, BARRIER** barrier, PLANE* myPlane, time_t* Colstartime, time_t* Colendtime, int* EnemyCurNum, int offsetX, int* backgroundOffset, time_t* Teststartime, int* sumtestnum, int* colcount, int* diecount, int* diecountplus);
void destorybullet(PLANE** enemy, BARRIER** barrier, PLANE* myPlane, int* EnemyCurNum, int offsetX, int* diecount, int* diecountplus);
//�����ϰ�
BARRIER* createbarrier(int offsetX);
//���㷴�����ʸ���ٶ�
void reflectVector(float* dx, float* dy, float nx, float ny);

// ����л���ȫ�ƶ�����
POS getNextMove1(PLANE* enemy, POS targetPos, const std::vector<POS>& barriers, std::vector<Vec2d> barrierVec, int width, int height, int barrierRadius, const PotentialField& pof, int offsetX);
POS getNextMove2(PLANE* enemy, POS targetPos, const std::vector<POS>& barriers, std::vector<Vec2d> barrierVec, int width, int height, int barrierRadius, const PotentialField& pof, int offsetX);
// ������������������֮��ľ���
double calculateDistance(double x1, double y1, double x2, double y2);
// �������������λ���Ƿ�ȫ������ײ���ϰ��
bool isSafePosition(const POS& position, const std::vector<POS>& barriers, int barrierRadius);
// �����������ҵ�������ϰ���
pos findNearestBarrier(const POS& position, const std::vector<POS>& barriers);
// ���λ���Ƿ����ƶ���ʷ��
bool isInHistory(const POS& position, const PLANE* enemy);
//�ϰ����ƶ�
void barrmove(BARRIER** barrier, int offsetX);
// ����Ŀ��������
Vec2d calculateAttraction(const POS& current, const POS& goal, const PotentialField& pof);
// ���������ϰ���ĳ���
Vec2d calculateRepulsion(const POS& current, const std::vector<POS>& barriers, double radius, const PotentialField& pof, PLANE* enemy);
// �ϳ����������ƴ�С
Vec2d getTotalForce(const POS& current, const POS& goal, const std::vector<POS>& barriers, double radius, const PotentialField& pof, PLANE* enemy);
//Ԥ���ϰ���
std::vector<POS> Predictpos(const std::vector<POS>& barriers, const std::vector<Vec2d>& barrierVec);
//��¼����
void Record(int sumtestnum, int colcount);
//������
void Clear();
void DrawoverPage(int offsetX, int data[200], int data2[200], int data3[200], int dataSize);
POS getNextMove3(plane* enemy, POS targetPos, const std::vector<POS>& barriers, std::vector<Vec2d> barrierVec, int width, int height, int barrierRadius, const PotentialField& pof, int offsetX);
bool isSafePosition2(const pos& position, plane* enemy, int Radius);


