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

struct Food { // 食物
	double x; // 食物x坐标
	double y; // 食物y坐标
	double r; // 食物的半径
	bool flag; // 状态标记，判断食物是否存在
	DWORD color; // 食物颜色
};

struct Bullet { // 子弹
	double x; // 子弹x坐标
	double y; // 子弹y坐标
	double r; // 子弹的半径
	double vx; // 子弹x轴速度分量
	double vy; // 子弹y轴速度分量
	double shootstep; // 子弹射击速度
	double curradian; // 子弹射击方向
	bool flag; // 状态标记，判断子弹是否存在
	DWORD color; // 子弹颜色
};

struct Role { // 角色
	double x; // 角色x坐标
	double y; // 角色y坐标
	double r; // 角色的半径
	double cx; // 炮管末端x坐标
	double cy; // 炮管末端y坐标
	double vx; // 角色x轴速度分量
	double vy; // 角色y轴速度分量
	double radian; // 炮管射击方向
	double shootstep; // 分身发射速度
	double curradian; // 分身发射方向
	double movestep; // 分身移动速度
	bool flag; // 状态标记，判断角色是否存在
	DWORD color; // 角色颜色
};

//-------球球大作战类模型--------//

class Model {
private:
	POINT g_pos; // 摄像机
	vector<Food> foodVec; // 食物
	vector<Bullet> bullVec; // 子弹
	vector<Role> roleVec; // 玩家
public:
	explicit Model() = default;
	~Model() = default;
	void updatePos(const int width, const int height); // 设置摄影机
	void setMap(IMAGE& map); // 设置地图
	double distance(const double x1, const double y1, const double x2, const double y2); // 求两点间距离
	void load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3, const int width, const int height); // 加载资源
	void bgm(); // 加载音乐
	void init(const int map_width, const int map_height, const double pi, const int food_num, const int bullet_num, const int derived_num); // 初始化数据
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // 载入PNG透明图片
	void welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3); // 绘制开始界面
	void draw(IMAGE& map, const int width, const int height, const double pi, const int food_num, const int bullet_num, const int derived_num); // 绘制游戏界面
	void creatFood(const int i, const int map_width, const int map_height); // 产生食物
	void eatFood(struct Role* role_, const int k, const int map_width, const int map_height, const int food_num, const int bullet_num); // 吃掉食物
	void creatBullet(struct Role* role_, const double radian, const int bullet_num); // 产生子弹
	void moveBullet(const int map_width, const int map_height, const int bullet_num); // 移动子弹
	void creatRole(struct Role* role_, const double radian, const int derived_num); // 产生分身
	void moveRole(const int map_width, const int map_height, const int derived_num); // 移动分身
	void mixRole(struct Role* role_, const int derived_num); // 融合分身
	void actRole(const int map_width, const int map_height, const int food_num, const int bullet_num, const int derived_num); // 角色活动
	void keyDown1(const int map_width, const int map_height); // 按键响应（异步）
	void keyDown2(const int bullet_num, const int derived_num); // 按键响应（同步）
	void mouseControl(const int width, const int height, const int bullet_num, const int derived_num); // 鼠标控制
};