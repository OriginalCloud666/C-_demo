#pragma once // 避免头文件重复定义
#pragma comment(lib,"winmm.lib") // 加载静态库

#include <iostream>
#include <vector>
#include <conio.h>
#include <time.h>
#include <graphics.h>
#include <mmsystem.h>

using namespace std;

//-------2048游戏类模型--------//

class Model {
private:
	int top; // 地图左上角y坐标
	int left; // 地图左上角x坐标
	int down; // 地图右下角y坐标
	int right; // 地图右下角x坐标
	vector<vector<int>> gameMapVec; // 存放地图信息
public:
	explicit Model();
	~Model() = default;
	void load(IMAGE& bk, IMAGE img[], int imgIndex[], const int grid_length, const int width, const int height); // 加载资源
	void bgm(); // 加载音乐
	void init(const int grid_num, const int grid_length, const int width, const int height); // 初始化数据
	void welcome(IMAGE& bk); // 绘制开始界面
	void draw(IMAGE& bk, IMAGE img[], int imgIndex[], const int grid_num, const int grid_length, const int score); // 绘制游戏界面
	bool condition1(const int grid_num); // 结束条件1
	bool condition2(const int grid_num); // 结束条件2
	bool over(const int grid_num); // 结束条件判断
	int randIntNum(const int grid_num); // 产生整形随机数
	int moveLeft(int& score, const int grid_num); // 向左移动
	int moveRight(int& score, const int grid_num); // 向右移动
	int moveUp(int& score, const int grid_num); // 向上移动
	int moveDown(int& score, const int grid_num); // 向下移动
	void keyDown(int& score, const int grid_num); // 键盘消息响应
};
