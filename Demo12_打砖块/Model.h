#pragma once // 避免头文件重复定义
#pragma comment(lib,"winmm.lib") // 加载静态库

#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>
#include <conio.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

constexpr auto WIDTH = 640; // 窗口宽度
constexpr auto HEIGHT = 720; // 窗口高度
constexpr auto BRICK_WIDTH = 80; // 砖块宽度
constexpr auto BRICK_HEIGHT = 30; // 砖块高度
constexpr auto BRICK_ROW = (HEIGHT / BRICK_HEIGHT - 16); // 砖块行数
constexpr auto BRICK_COL = (WIDTH / BRICK_WIDTH); // 砖块列数
constexpr auto PI = 3.1415926; // π

struct Board // 木板
{
	double x; // x坐标
	double y; // y坐标
	double step; // 移动速度
	int width; // 宽度
	int height; // 高度
};

struct Ball // 球
{
	double x; // x坐标
	double y; // y坐标
	double dx; // x轴速度分量
	double dy; // y轴速度分量
	double step; // 移动速度
	double radian; // 移动的方向角度
	double r; // 半径大小
	int flag = 0; // 状态标识，用于判断小球是否发射
};

//-------打砖块游戏类模型--------//

class Model
{
public:
	explicit Model();
	~Model() = default;

	struct Ball& getBall();
	double distance(const double x1, const double y1, const double x2, const double y2); // 求两点间距离
	void load(IMAGE& bk); // 加载资源
	void bgm(); // 加载音乐
	void init(); // 初始化数据
	void welcome(IMAGE& bk); // 绘制开始界面
	void draw(); // 绘制游戏界面
	void keyDown1(); // 按键响应（异步）
	void keyDown2(); // 按键响应（同步）
	void ballMove(); // 小球移动碰撞
	void ballCrash(); // 小球与砖块碰撞
	bool over(); // 结束条件判断

private:
	int top; // 地图左上角y坐标
	int left; // 地图左上角x坐标
	int down; // 地图右下角y坐标
	int right; // 地图右下角x坐标
	struct Board board; // 木板
	struct Ball ball; // 小球
	vector<vector<int>> gameMapVec; // 存放地图信息
};
