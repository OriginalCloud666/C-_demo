#pragma once // 避免头文件重复定义
#pragma comment(lib,"winmm.lib") // 加载静态库

#include <iostream>
#include <vector>
#include <conio.h>
#include <time.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

auto const width = 640; // 窗口宽度
auto const height = 720; // 窗口高度
auto const grid_num = 4; // 行列格子数量
auto const grid_length = 60; // 格子边长
auto const photo_num = 12; // 图片数量

//-------2048游戏类模型--------//

class Model {
private:
	int score; // 记录得分
	int top; // 地图左上角y坐标
	int left; // 地图左上角x坐标
	int down; // 地图右下角y坐标
	int right; // 地图右下角x坐标
	vector<vector<int>> gameMapVec; // 存放地图信息
public:
	explicit Model();
	~Model() = default;
	int& getScore(); // 获取score
	void load(IMAGE& bk, vector<IMAGE>& img, vector<int>& imgIndex); // 加载资源
	void bgm(); // 加载音乐
	void init(); // 初始化数据
	void welcome(IMAGE& bk); // 绘制开始界面
	void draw(IMAGE& bk, vector<IMAGE>& img, vector<int>& imgIndex); // 绘制游戏界面
	bool condition1(); // 结束条件1
	bool condition2(); // 结束条件2
	bool over(); // 结束条件判断
	int randIntNum(); // 产生整形随机数
	int moveLeft(); // 向左移动
	int moveRight(); // 向右移动
	int moveUp(); // 向上移动
	int moveDown(); // 向下移动
	void keyDown(); // 键盘消息响应
};