#pragma once // 避免头文件重复定义
#pragma comment(lib,"winmm.lib") // 加载静态库

#include <iostream>
#include <vector>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <graphics.h>
#include <mmsystem.h>

using namespace std;

struct Battery { // 炮台
	double x; // 炮管末端横坐标
	double y; // 炮管末端纵坐标
	int len; // 炮管长度
	double cx; // 炮台中心x坐标
	double cy; // 炮台中心y坐标
	double radian; // 炮管与水平面弧度
};

struct Bullet { // 子弹
	double x; // 子弹的x坐标
	double y; // 子弹的y坐标
	double vx; // x轴的子弹速度分量
	double vy; // y轴的子弹速度分量
	double step; // 子弹每次移动的距离
	bool flag; // 状态标识，标识子弹状态
	DWORD color; // 子弹颜色
};

struct Balloon { // 气球
	double x; // 气球的x坐标
	double y; // 气球的y坐标
	double step; // 子弹每次移动的距离
	bool flag; // 状态标识，判断气球是否被击中
	DWORD color; // 气球颜色
};

//-------打气球游戏类模型--------//

class Model {
private:
	int score; // 记录得分
	int speed; // 记录子弹速度能级
	int bullet_num; // 记录子弹数量
	struct Battery bat; // 炮台
	vector<Bullet> bullVec; // 子弹
	vector<Balloon> ballVec; // 气球
public:
	explicit Model();
	~Model() = default;
	void load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3, IMAGE& w4, const int width, const int height); // 加载资源
	void bgm(); // 加载音乐
	void init(const int bull_num, const int balloon_num, const int width, const int height); // 初始化数据
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // 载入PNG透明图片
	void welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3, IMAGE& w4); // 绘制开始界面
	void draw(IMAGE& bk, const int bull_num, const int balloon_num, const int height); // 绘制游戏界面
	void creatBullet(const int bull_num); // 产生子弹
	void speedBullet(const int bull_num); // 加速子弹
	void fillBullet(const int bull_num, const int width, const int height); // 填充子弹
	void moveBullet(const int bull_num); // 子弹移动
	void creatBalloon(const int i, const int width, const int height); // 产生气球
	void crashBalloon(const int bull_num, const int ball_num, const int width, const int height); // 气球碰撞
	void moveBalloon(const int ball_num, const int width, const int height); // 气球移动
	void mouseControl(const int bull_num, const int width, const int height); // 鼠标消息响应
	void keyDown(const int bull_num, const int width, const int height); // 键盘消息响应（同步）
	void keyDown2(const double barrel_speed); // 键盘消息响应（异步）
};