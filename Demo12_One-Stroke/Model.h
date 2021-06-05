#pragma once // 避免头文件重复定义
#pragma comment(lib,"winmm.lib") // 加载静态库

#include <iostream>
#include <vector>
#include <conio.h>
#include <graphics.h>
#include <mmsystem.h>

using namespace std;

enum Tools {
	BLANK = 0, // 空地用0表示
	WALL = 1, // 墙壁用1表示
	ROLE = 2, // 人物用2表示
	BOARD = 3, // 人走过的地方用3表示
};

enum Direction {
	LEFT = 0, // 左方向
	RIGHT = 1, // 右方向
	UP = 2, // 上方向
	DOWN = 3, // 下方向
};

struct Role { // 角色
	int row; // 角色所在行
	int col; // 角色所在列
	int flag; // 移动标记
};

struct Point { // 点用于记录起始和终止位置
	int row; // 当前点所在行
	int col; // 当前点所在列
};

//-------一笔画游戏类模型--------//

class Model {
private:
	int flag; // 状态标识，判断关卡
	int count; // 计数器
	int top; // 地图左上角y坐标
	int left; // 地图左上角x坐标
	int down; // 地图右下角y坐标
	int right; // 地图右下角x坐标
	struct Role role; // 角色人物
	vector<Point> point; // 点存放位置
	vector<vector<vector<int>>> gameMapVec; // 存放地图及关卡信息
public:
	explicit Model();
	~Model() = default;
	int& getCount(); // 获取count
	int& getFlag(); // 获取flag
	struct Role& getRole(); // 获取role
	vector<Point>& getPoint(); // 获取point
	void load(IMAGE& bk, IMAGE& w1, IMAGE& w2); // 加载资源
	void bgm(); // 加载音乐
	void init(const int width, const int height, const int grid_num, const int grid_size, const int point_num); // 初始化数据
	void pointInit(const int grid_num); // 位置信息点的初始化
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // 载入PNG图片
	void welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2); // 绘制开始界面
	void draw(const int grid_num, const int grid_size); // 绘制游戏界面
	void drawRole(const double x, const double y, const int border_size, const int grid_num); // 角色绘制
	void drawWall(const double x, const double y, const int grid_size); // 墙壁绘制
	void drawLine(struct Point* begin, struct Point* end, const int grid_size); // 画线过程
	void searchRolePos(const int grid_num); // 寻找角色位置
	int roleStop(const int dir); // 移动标准判断
	void moveRole(const int dir); // 角色移动
	void keyDown(); // 按键消息响应
	bool over(const int grid_num); // 结束条件判断
};