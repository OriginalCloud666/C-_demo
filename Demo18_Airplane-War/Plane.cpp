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

// 飞机贴图
void Plane::draw(IMAGE pic_plane[]) {
	// 源码图：SRCPAINT格式去贴图
	putimage(this->x, this->y, pic_plane, SRCPAINT);
	// 掩码图：SRCAND格式去贴图
	putimage(this->x, this->y, pic_plane + 1, SRCAND);
}

// 飞机子弹贴图
void Plane::drawBullet(IMAGE pic_bullet[]) {
	for (list<Bullet>::iterator iter = this->bulletLis.begin(); iter != bulletLis.end(); iter++) {
		// 源码图：SRCPAINT格式去贴图
		putimage(iter->x, iter->y, pic_bullet, SRCPAINT);
		// 掩码图：SRCAND格式去贴图
		putimage(iter->x, iter->y, pic_bullet + 1, SRCAND);
	}
}

// 设置飞机属性
void Plane::set(const double x, const double y, const double step, const int width, const int height, const int hp, const bool flag) {
	this->x = x;
	this->y = y;
	this->step = step;
	this->width = width;
	this->height = height;
	this->hp = hp;
	this->flag = flag;
}

// 创建子弹
void Plane::createBullet(double step) {
	Bullet newbullet;
	newbullet.x = x + 39.8;
	newbullet.y = y - 20;
	newbullet.step = step;
	this->bulletLis.push_back(newbullet);
}

// 移动子弹
void Plane::moveBullet() {
	for (list<Bullet>::iterator iter = this->bulletLis.begin(); iter != bulletLis.end(); iter++) {
		iter->y -= iter->step;
		if (iter->y <= -15) {
			this->bulletLis.pop_front();
		}
	}
}