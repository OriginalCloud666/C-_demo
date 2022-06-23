#pragma once // 避免头文件重复定义
#pragma comment(lib,"winmm.lib") // 加载静态库

#include <iostream>
#include <vector>
#include <conio.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

constexpr auto WIDTH = 640; // 窗口宽度
constexpr auto HEIGHT = 480; // 窗口高度
constexpr auto GRID_NUM = 4; // 行列格子数量
constexpr auto GRID_LENGTH = 30; // 格子边长

//-------涂格子游戏类模型--------//

class Model
{
public:
	explicit Model();
	~Model() = default;

	void load(IMAGE& bk); // 加载资源
	void bgm(); // 加载音乐
	void init(); // 初始化数据
	void welcome(IMAGE& bk); // 绘制开始界面
	void draw(IMAGE& bk); // 绘制游戏界面
	void mouseControl(); // 鼠标消息响应
	int over(); // 结束条件判断

private:
	int top; // 地图左上角y坐标
	int left; // 地图左上角x坐标
	int down; // 地图右下角y坐标
	int right; // 地图右下角x坐标
	vector<vector<int>> gameMapVec; // 存放地图信息
};
