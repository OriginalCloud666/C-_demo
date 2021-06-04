#pragma once // 避免头文件重复定义
#pragma comment(lib,"winmm.lib") // 加载静态库

#include <iostream>
#include <vector>
#include <conio.h>
#include <graphics.h>
#include <mmsystem.h>

using namespace std;

//-------拼图游戏类模型--------//

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
	void load(IMAGE& bk, IMAGE& white, const int width, const int height); // 加载资源
	void bgm(); // 加载音乐
	void init(const int width, const int height, const int grid_num, const int grid_width, const int grid_height); // 初始化数据
	void welcome(IMAGE& bk); // 绘制开始界面
	void draw(IMAGE& bk, IMAGE& white, const int grid_num, const int grid_width, const int grid_height); // 绘制游戏界面
	int searchArray_i(const int grid_num); // 定位白块位置i
	int searchArray_j(const int grid_num); // 定位白块位置j
	void mouseControl(const int grid_num, const int grid_width, const int grid_height); // 鼠标控制
	bool over(const int grid_num); // 游戏结束判断
};
