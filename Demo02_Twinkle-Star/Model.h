#pragma once // 避免头文件重复定义
#pragma comment(lib,"winmm.lib") // 加载静态库

#include <iostream>
#include <vector>
#include <time.h>
#include <conio.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

auto const width = 640; // 窗口宽度
auto const height = 480; // 窗口高度
auto const star_num = 100; // 星星总数

struct Star {
	double x; // 星星x坐标
	double y; // 星星y坐标
	double radius; // 星星半径
	double step; // 星星速度
	int color; // 星星颜色
};

//-------闪烁星星游戏类模型--------//

class Model {
private:
	vector<Star> startVec; // 存放星星
public:
	explicit Model() = default;
	~Model() = default;
	void load(IMAGE& bk); // 加载资源
	void bgm(); // 加载音乐
	struct Star initStar(); // 初始化星星
	void moveStar(const int i); // 移动星星
	void init(); // 初始化数据
	void draw(IMAGE& bk); // 绘制游戏界面
};