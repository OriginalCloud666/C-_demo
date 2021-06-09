#pragma once // ����ͷ�ļ��ظ�����

#include <iostream>
#include <list>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

struct Bullet { // �ӵ�
	double x; // ���Ͻ�x����
	double y; // ���Ͻ�y����
	double step; // �ӵ��ƶ��ٶ�
};

//-------�ɻ���ģ��--------//

class Plane {
private:
	double x; // ���Ͻ�x����
	double y; // ���Ͻ�y����
	double step; // �����ƶ��ٶ�
	int width; // ͼƬ���
	int height; // ͼƬ�߶�
	int hp; // �ɻ�Ѫ��
	bool flag; // �жϷɻ��Ƿ���
	list<Bullet> bulletLis; // �ɻ������е��ӵ�
public:
	explicit Plane();
	~Plane() = default;
	int& getWidth();
	int& getHeight();
	int& getHp();
	bool& getFlag();
	double& get_x();
	double& get_y();
	double& getStep();
	list<Bullet>& getBullet();
	void set(const double x, const double y, const double step, const int width, const int height, const int hp, const bool flag); // ���÷ɻ�����
	void draw(IMAGE pic_plane[]); // �ɻ���ͼ
	void drawBullet(IMAGE pic_bullet[]); // �ɻ��ӵ���ͼ
	void createBullet(double step); // �����ӵ�
	void moveBullet(); // �ӵ��ƶ�
};