#pragma once // ����ͷ�ļ��ظ�����
#pragma comment(lib,"winmm.lib") // ���ؾ�̬��

#include <iostream>
#include <vector>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <graphics.h>
#include <mmsystem.h>

using namespace std;

struct Food { // ʳ��
	double x; // ʳ��x����
	double y; // ʳ��y����
	double r; // ʳ��İ뾶
	bool flag; // ״̬��ǣ��ж�ʳ���Ƿ����
	DWORD color; // ʳ����ɫ
};

struct Bullet { // �ӵ�
	double x; // �ӵ�x����
	double y; // �ӵ�y����
	double r; // �ӵ��İ뾶
	double vx; // �ӵ�x���ٶȷ���
	double vy; // �ӵ�y���ٶȷ���
	double shootstep; // �ӵ�����ٶ�
	double curradian; // �ӵ��������
	bool flag; // ״̬��ǣ��ж��ӵ��Ƿ����
	DWORD color; // �ӵ���ɫ
};

struct Role { // ��ɫ
	double x; // ��ɫx����
	double y; // ��ɫy����
	double r; // ��ɫ�İ뾶
	double cx; // �ڹ�ĩ��x����
	double cy; // �ڹ�ĩ��y����
	double vx; // ��ɫx���ٶȷ���
	double vy; // ��ɫy���ٶȷ���
	double radian; // �ڹ��������
	double shootstep; // �������ٶ�
	double curradian; // �����䷽��
	double movestep; // �����ƶ��ٶ�
	bool flag; // ״̬��ǣ��жϽ�ɫ�Ƿ����
	DWORD color; // ��ɫ��ɫ
};

//-------�������ս��ģ��--------//

class Model {
private:
	POINT g_pos; // �����
	vector<Food> foodVec; // ʳ��
	vector<Bullet> bullVec; // �ӵ�
	vector<Role> roleVec; // ���
public:
	explicit Model() = default;
	~Model() = default;
	void updatePos(const int width, const int height); // ������Ӱ��
	void setMap(IMAGE& map); // ���õ�ͼ
	double distance(const double x1, const double y1, const double x2, const double y2); // ����������
	void load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3, const int width, const int height); // ������Դ
	void bgm(); // ��������
	void init(const int map_width, const int map_height, const double pi, const int food_num, const int bullet_num, const int derived_num); // ��ʼ������
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // ����PNG͸��ͼƬ
	void welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3); // ���ƿ�ʼ����
	void draw(IMAGE& map, const int width, const int height, const double pi, const int food_num, const int bullet_num, const int derived_num); // ������Ϸ����
	void creatFood(const int i, const int map_width, const int map_height); // ����ʳ��
	void eatFood(struct Role* role_, const int k, const int map_width, const int map_height, const int food_num, const int bullet_num); // �Ե�ʳ��
	void creatBullet(struct Role* role_, const double radian, const int bullet_num); // �����ӵ�
	void moveBullet(const int map_width, const int map_height, const int bullet_num); // �ƶ��ӵ�
	void creatRole(struct Role* role_, const double radian, const int derived_num); // ��������
	void moveRole(const int map_width, const int map_height, const int derived_num); // �ƶ�����
	void mixRole(struct Role* role_, const int derived_num); // �ںϷ���
	void actRole(const int map_width, const int map_height, const int food_num, const int bullet_num, const int derived_num); // ��ɫ�
	void keyDown1(const int map_width, const int map_height); // ������Ӧ���첽��
	void keyDown2(const int bullet_num, const int derived_num); // ������Ӧ��ͬ����
	void mouseControl(const int width, const int height, const int bullet_num, const int derived_num); // ������
};