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

auto const width = 640; // ���ڿ��
auto const height = 720; // ���ڸ߶�
auto const bull_num = 50; // �ӵ���Ŀ
auto const balloon_num = 12; // ��������
auto const barrel_speed = 0.01; // �ڹ��ƶ��ٶ�

struct Battery { // ��̨
	double x; // �ڹ�ĩ�˺�����
	double y; // �ڹ�ĩ��������
	int len; // �ڹܳ���
	double cx; // ��̨����x����
	double cy; // ��̨����y����
	double radian; // �ڹ���ˮƽ�满��
};

struct Bullet { // �ӵ�
	double x; // �ӵ���x����
	double y; // �ӵ���y����
	double vx; // x����ӵ��ٶȷ���
	double vy; // y����ӵ��ٶȷ���
	double step; // �ӵ�ÿ���ƶ��ľ���
	bool flag; // ״̬��ʶ����ʶ�ӵ�״̬
	DWORD color; // �ӵ���ɫ
};

struct Balloon { // ����
	double x; // �����x����
	double y; // �����y����
	double step; // �ӵ�ÿ���ƶ��ľ���
	bool flag; // ״̬��ʶ���ж������Ƿ񱻻���
	DWORD color; // ������ɫ
};

//-------��������Ϸ��ģ��--------//

class Model {
private:
	int score; // ��¼�÷�
	int speed; // ��¼�ӵ��ٶ��ܼ�
	int bullet_num; // ��¼�ӵ�����
	struct Battery bat; // ��̨
	vector<Bullet> bullVec; // �ӵ�
	vector<Balloon> ballVec; // ����
public:
	explicit Model();
	~Model() = default;
	void load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3, IMAGE& w4); // ������Դ
	void bgm(); // ��������
	void init(); // ��ʼ������
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // ����PNG͸��ͼƬ
	void welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3, IMAGE& w4); // ���ƿ�ʼ����
	void draw(IMAGE& bk); // ������Ϸ����
	void creatBullet(); // �����ӵ�
	void speedBullet(); // �����ӵ�
	void fillBullet(); // ����ӵ�
	void moveBullet(); // �ӵ��ƶ�
	void creatBalloon(const int i); // ��������
	void crashBalloon(); // ������ײ
	void moveBalloon(); // �����ƶ�
	void mouseControl(); // �����Ϣ��Ӧ
	void keyDown(); // ������Ϣ��Ӧ��ͬ����
	void keyDown2(); // ������Ϣ��Ӧ���첽��
};