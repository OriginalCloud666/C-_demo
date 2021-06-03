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
	void load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3, IMAGE& w4, const int width, const int height); // ������Դ
	void bgm(); // ��������
	void init(const int bull_num, const int balloon_num, const int width, const int height); // ��ʼ������
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // ����PNG͸��ͼƬ
	void welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3, IMAGE& w4); // ���ƿ�ʼ����
	void draw(IMAGE& bk, const int bull_num, const int balloon_num, const int height); // ������Ϸ����
	void creatBullet(const int bull_num); // �����ӵ�
	void speedBullet(const int bull_num); // �����ӵ�
	void fillBullet(const int bull_num, const int width, const int height); // ����ӵ�
	void moveBullet(const int bull_num); // �ӵ��ƶ�
	void creatBalloon(const int i, const int width, const int height); // ��������
	void crashBalloon(const int bull_num, const int ball_num, const int width, const int height); // ������ײ
	void moveBalloon(const int ball_num, const int width, const int height); // �����ƶ�
	void mouseControl(const int bull_num, const int width, const int height); // �����Ϣ��Ӧ
	void keyDown(const int bull_num, const int width, const int height); // ������Ϣ��Ӧ��ͬ����
	void keyDown2(const double barrel_speed); // ������Ϣ��Ӧ���첽��
};