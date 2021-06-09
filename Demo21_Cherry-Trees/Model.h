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
auto const pi = 3.1415926; // 定义π

//-------樱花树游戏类模型--------//

class Model {
private:
	double offsetAngle; // 左右枝干和父枝干偏离的角度
	double shortenRate = 0.65; // 子枝干比父枝干变短的倍数
	int isShowAnimation = 1; // 是否显示树生成的过程动画
public:
	explicit Model();
	~Model() = default;
	void bgm(); // 加载音乐
	void init(); // 初始化数据
	double mapValue(double input, double inputMin, double inputMax, double outputMin, double outputMax); // 映射
	double randBetween(double min, double max); // 随机数
	void brunch(double x_start, double y_start, double length, double angle, double thickness, int generation); // 分支树
	void judeMove(); // 小球移动条件判断
	void ballMove(); // 小球的移动
	void mouseControl(); // 鼠标消息响应
};