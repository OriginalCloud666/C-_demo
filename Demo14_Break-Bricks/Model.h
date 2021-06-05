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

auto const width = 640; // 窗口宽度
auto const height = 720; // 窗口高度
auto const brick_width = 80; // 砖块宽度
auto const brick_height = 30; // 砖块高度
auto const brick_row = (height / brick_height - 16); // 砖块行数
auto const brick_col = (width / brick_width); // 砖块列数
auto const pi = 3.1415926; // π

struct Board { // 木板
	double x; // 木板x坐标
	double y; // 木板y坐标
	double step; // 木板移动速度
	int width; // 木板宽度
	int height; // 木板高度
};

struct Ball { // 球
	double x; // 小球x坐标
	double y; // 小球y坐标
	double dx; // 小球x轴速度分量
	double dy; // 小球y轴速度分量
	double step; // 小球的移动速度
	double radian; // 小球移动的方向角度
	double r; // 小球半径大小
	int flag = 0; // 状态标识，用于判断小球是否发射
};

//-------打砖块游戏类模型--------//

class Model {
private:
	int top; // 地图左上角y坐标
	int left; // 地图左上角x坐标
	int down; // 地图右下角y坐标
	int right; // 地图右下角x坐标
	struct Board board; // 木板
	struct Ball ball; // 小球
	vector<vector<int>> gameMapVec; // 存放地图信息
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
};