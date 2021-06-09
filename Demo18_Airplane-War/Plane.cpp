#include "Plane.h"
using namespace std;

Plane::Plane() :x(0), y(0), step(0), width(0), height(0), hp(0), flag(false) {}

bool& Plane::getFlag() {
	return this->flag;
}

int& Plane::getWidth() {
	return this->width;
}

int& Plane::getHeight() {
	return this->height;
}

int& Plane::getHp() {
	return this->hp;
}

double& Plane::getStep() {
	return this->step;
}

double& Plane::get_x() {
	return this->x;
}

double& Plane::get_y() {
	return this->y;
}

list<Bullet>& Plane::getBullet() {
	return this->bulletLis;
}

// �ɻ���ͼ
void Plane::draw(IMAGE pic_plane[]) {
	// Դ��ͼ��SRCPAINT��ʽȥ��ͼ
	putimage(this->x, this->y, pic_plane, SRCPAINT);
	// ����ͼ��SRCAND��ʽȥ��ͼ
	putimage(this->x, this->y, pic_plane + 1, SRCAND);
}

// �ɻ��ӵ���ͼ
void Plane::drawBullet(IMAGE pic_bullet[]) {
	for (list<Bullet>::iterator iter = this->bulletLis.begin(); iter != bulletLis.end(); iter++) {
		// Դ��ͼ��SRCPAINT��ʽȥ��ͼ
		putimage(iter->x, iter->y, pic_bullet, SRCPAINT);
		// ����ͼ��SRCAND��ʽȥ��ͼ
		putimage(iter->x, iter->y, pic_bullet + 1, SRCAND);
	}
}

// ���÷ɻ�����
void Plane::set(const double x, const double y, const double step, const int width, const int height, const int hp, const bool flag) {
	this->x = x;
	this->y = y;
	this->step = step;
	this->width = width;
	this->height = height;
	this->hp = hp;
	this->flag = flag;
}

// �����ӵ�
void Plane::createBullet(double step) {
	Bullet newbullet;
	newbullet.x = x + 39.8;
	newbullet.y = y - 20;
	newbullet.step = step;
	this->bulletLis.push_back(newbullet);
}

// �ƶ��ӵ�
void Plane::moveBullet() {
	for (list<Bullet>::iterator iter = this->bulletLis.begin(); iter != bulletLis.end(); iter++) {
		iter->y -= iter->step;
		if (iter->y <= -15) {
			this->bulletLis.pop_front();
		}
	}
}