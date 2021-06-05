#pragma once // 避免头文件重复定义
#pragma comment(lib,"winmm.lib") // 加载静态库

#include <iostream>
#include <vector>
#include <conio.h>
#include <math.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

auto const width = 640; // 窗口宽度
auto const height = 720; // 窗口高度
auto const pic_num = 12; // 图片数量

//-------音乐相册类模型--------//

class Model {
private:
	int count; // 计数器，记录循环次数
public:
	explicit Model();
	~Model() = default;
	int& getCount(); // 获取count
	void load(IMAGE& bk1, IMAGE& bk2, IMAGE& w1, vector<IMAGE>& img, vector<int>& imgIndex, vector<int>& pic_width, vector<int>& pic_height); // 加载资源
	void bgm(); // 加载音乐
	void drawAlphabk(IMAGE* picture, const int  picture_x, const int picture_y); // 载入PNG透明图片
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // 载入PNG透明图片
	void welcome(IMAGE& bk1, IMAGE& w1); // 绘制开始界面
	void drawBorder(const int x0, const int y0, const int size, COLORREF C, const int borderwidth); // 绘制心形边框
	void drawText(const double i, const int x0, const int y0, const int size, const char* s); // 绘制边框字段
	void drawHeart(IMAGE& bk2, vector<IMAGE>& img, vector<int>& pic_x, vector<int>& pic_y, const int x, const int y, const int size, COLORREF C, const char s[][5], const double move, const double interval, const int time); // 绘制心形图案
};