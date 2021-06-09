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

auto const width = 640; // 窗口宽度
auto const height = 720; // 窗口高度
auto const ball_num = 16; // 小球个数
auto const pi = 3.1415926; // 定义π

struct Ball { // 球
	double x; // 球心x坐标
	double y; // 球心y坐标
	double step; // 球的整体移动速度
	double tempstep; // 记录小球初始时的速度
	int juge; // 小球是否开始移动的状态标识
	int flag; // 小球移动转向的状态标识
	int r; // 球的半径
	COLORREF color; // 小球的颜色
};

//-------旋转花游戏类模型--------//

class Model {
private:
	vector<Ball> balls; // 旋转的小球
public:
	explicit Model() = default;
	~Model() = default;
	void bgm(); // 加载音乐
	int timer(time_t sec, int id); // 定时器
	void init(); // 初始化数据
	void draw(); // 程序界面绘制
	void judeMove(); // 小球移动条件判断
	void ballMove(); // 小球的移动
};