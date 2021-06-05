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

auto const width = 1280; // ���ڿ��
auto const height = 720; // ���ڸ߶�
auto const map_width = width * 4; // ��ͼ���
auto const map_height = height * 4; // ��ͼ�߶�
auto const food_num = 480; // ʳ������
auto const bullet_num = 100; // �ӵ�����
auto const derived_num = 16; // ��������
auto const pi = 3.1415926; // ��

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
	void updatePos(); // ������Ӱ��
	void setMap(IMAGE& map); // ���õ�ͼ
	double distance(const double x1, const double y1, const double x2, const double y2); // ����������
	void load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3); // ������Դ
	void bgm(); // ��������
	void init(); // ��ʼ������
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // ����PNG͸��ͼƬ
	void welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3); // ���ƿ�ʼ����
	void draw(IMAGE& map); // ������Ϸ����
	void creatFood(const int i); // ����ʳ��
	void eatFood(struct Role* role_, const int k); // �Ե�ʳ��
	void creatBullet(struct Role* role_, const double radian); // �����ӵ�
	void moveBullet(); // �ƶ��ӵ�
	void creatRole(struct Role* role_, const double radian); // ��������
	void moveRole(); // �ƶ�����
	void mixRole(struct Role* role_); // �ںϷ���
	void actRole(); // ��ɫ�
	void keyDown1(); // ������Ӧ���첽��
	void keyDown2(); // ������Ӧ��ͬ����
	void mouseControl(); // ������
};