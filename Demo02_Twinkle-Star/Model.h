#pragma once // 避免头文件重复定义
#pragma comment(lib,"winmm.lib") // 加载静态库

#include <iostream>
#include <vector>
#include <time.h>
#include <conio.h>
#include <graphics.h>
#include <mmsystem.h>

using namespace std;

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
	void load(IMAGE& bk, const int width, const int height); // 加载资源
	void bgm(); // 加载音乐
	struct Star initStar(const int height); // 初始化星星
	void moveStar(const int i, const int weidth, const int height); // 移动星星
	void init(const int star_num, const int width, const int height); // 初始化数据
	void draw(IMAGE& bk); // 绘制游戏界面
};