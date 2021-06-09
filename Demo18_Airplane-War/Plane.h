#pragma once // 避免头文件重复定义

#include <iostream>
#include <list>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

struct Bullet { // 子弹
	double x; // 左上角x坐标
	double y; // 左上角y坐标
	double step; // 子弹移动速度
};

//-------飞机类模型--------//

class Plane {
private:
	double x; // 左上角x坐标
	double y; // 左上角y坐标
	double step; // 设置移动速度
	int width; // 图片宽度
	int height; // 图片高度
	int hp; // 飞机血量
	bool flag; // 判断飞机是否存活
	list<Bullet> bulletLis; // 飞机所带有的子弹
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
	void set(const double x, const double y, const double step, const int width, const int height, const int hp, const bool flag); // 设置飞机属性
	void draw(IMAGE pic_plane[]); // 飞机贴图
	void drawBullet(IMAGE pic_bullet[]); // 飞机子弹贴图
	void createBullet(double step); // 创建子弹
	void moveBullet(); // 子弹移动
};