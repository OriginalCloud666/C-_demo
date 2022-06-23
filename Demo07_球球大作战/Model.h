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

constexpr auto WIDTH = 1280; // 窗口宽度
constexpr auto HEIGHT = 720; // 窗口高度
constexpr auto MAP_WIDTH = WIDTH * 4; // 地图宽度
constexpr auto MAP_HEIGHT = HEIGHT * 4; // 地图高度
constexpr auto FOOD_NUM = 480; // 食物数量
constexpr auto BULLET_NUM = 100; // 子弹数量
constexpr auto DERIVED_NUM = 16; // 分身数量
constexpr auto PI = 3.1415926; // π

struct Food // 食物
{
	double x; // x坐标
	double y; // y坐标
	double r; // 半径
	bool flag; // 状态标记，判断是否存在
	DWORD color; // 颜色
};

struct Bullet // 子弹
{
	double x; // x坐标
	double y; // y坐标
	double r; // 半径
	double vx; // x轴速度分量
	double vy; // y轴速度分量
	double shootstep; // 射击速度
	double curradian; // 射击方向
	bool flag; // 状态标记，判断是否存在
	DWORD color; // 颜色
};

struct Role // 角色
{
	double x; // 角色x坐标
	double y; // 角色y坐标
	double r; // 角色半径
	double cx; // 炮管末端x坐标
	double cy; // 炮管末端y坐标
	double vx; // 角色x轴速度分量
	double vy; // 角色y轴速度分量
	double radian; // 炮管射击方向
	double shootstep; // 分身发射速度
	double curradian; // 分身发射方向
	double movestep; // 分身移动速度
	bool flag; // 状态标记，判断是否存在
	DWORD color; // 角色颜色
};

//-------球球大作战类模型--------//

class Model
{
public:
	explicit Model() = default;
	~Model() = default;

	void updatePos(); // 设置摄影机
	void setMap(IMAGE& map); // 设置地图
	double distance(const double x1, const double y1, const double x2, const double y2); // 求两点间距离
	void load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3); // 加载资源
	void bgm(); // 加载音乐
	void init(); // 初始化数据
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // 载入PNG透明图片
	void welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3); // 绘制开始界面
	void draw(IMAGE& map); // 绘制游戏界面
	void creatFood(const int i); // 产生食物
	void eatFood(struct Role* role_, const int k); // 吃掉食物
	void creatBullet(struct Role* role_, const double radian); // 产生子弹
	void moveBullet(); // 移动子弹
	void creatRole(struct Role* role_, const double radian); // 产生分身
	void moveRole(); // 移动分身
	void mixRole(struct Role* role_); // 融合分身
	void actRole(); // 角色活动
	void keyDown1(); // 按键响应（异步）
	void keyDown2(); // 按键响应（同步）
	void mouseControl(); // 鼠标控制

private:
	POINT g_pos; // 摄像机
	vector<Food> foodVec; // 食物
	vector<Bullet> bullVec; // 子弹
	vector<Role> roleVec; // 玩家
};
