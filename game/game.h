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
#define ENEMYPROSPEED 0.3
//�л������ٶ�
#define ENEMYFLYSPEED 5
//�ӵ�����
#define BULLETNUM 1000
//�ӵ��ٶ�
#define BULLETSPEED 4
//�ӵ���ת�Ƕ�
#define BULLETANGLE 10*3.1415926/180
//�ϰ�������
#define MAXBARNUM 3
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
	long long HighScore;

	struct plane* next;

	POS moveHistory[10];// ��¼�ƶ���ʷ
	int Historynum;
	int Historycurnum;
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
extern PLANE myPlane;
//�л���ǰ����
extern int EnemyCurNum;
//ÿ����һ���л��Ŀ�ʼ�ͽ���ʱ��
extern DWORD startime, endtime;
extern time_t Colstartime, Colendtime, Teststartime, Testendtime;

//ͼƬ����
extern IMAGE Img[10];


//������ǰ����
extern int n;
extern PLANE* enemy ;
extern BARRIER* barrier;
extern int backgroundOffset;
extern bool flag;
extern bool Spacekeypressed;
extern int sumtestnum, colcount;
//��������
//��ʼ����Ϸ
void initgame(PLANE** enemy, BARRIER** barrier);
//��������
void initdraw(PLANE** enemy, BARRIER** barrier);
//�����ƶ�
void Move(PLANE** enemy, BARRIER** barrier, const PotentialField& pof);
//��ʼ���л�
PLANE* initenemy(PLANE* enemy);
//ɾ�л�
void delezeroe(PLANE** enemy);
//��ײ�ж�
int isintersect(POS a, POS b, int r);
//���ټ��
void destroyplane(PLANE** enemy, BARRIER** barrier);
void destorybullet(PLANE** enemy, BARRIER** barrier);
//�����ϰ�
BARRIER* createbarrier();
//���㷴�����ʸ���ٶ�


// ����л���ȫ�ƶ�����
POS getNextMove(PLANE* enemy, POS targetPos, const std::vector<POS>& barriers, std::vector<Vec2d> barrierVec, int width, int height, int barrierRadius, const PotentialField& pof);
// ������������������֮��ľ���
double calculateDistance(double x1, double y1, double x2, double y2);



//�ϰ����ƶ�
void barrmove(BARRIER** barrier);
void AdjustScore(PLANE* myPlane);
Vec2d calculateAttraction(const POS& current, const POS& goal, const PotentialField& pof);

Vec2d calculateRepulsion(const POS& current, const std::vector<POS>& barriers, double radius, const PotentialField& pof, PLANE* enemy);
Vec2d getTotalForce(const POS& current, const POS& goal, const std::vector<POS>& barriers, double radius, const PotentialField& pof, PLANE* enemy);



