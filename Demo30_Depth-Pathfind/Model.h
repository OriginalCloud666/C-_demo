#pragma once // 避免头文件重复定义
#pragma comment(lib,"winmm.lib") // 加载静态库

#include <iostream>
#include <vector>
#include <stack>
#include <conio.h>
#include <string.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

auto const width = 640; // 窗口宽度
auto const height = 720; // 窗口高度
auto const grid_width = 35; // 格子宽度
auto const grid_height = 35; // 格子高度
auto const row = 12; // 行数
auto const col = 12; // 列数

enum Dir { // 方向
	UP = 0,
	RIGHT = 1,
	DOWN = 2,
	LEFT = 3,
};

struct Button { // 按钮
	int x; // 按钮左上角x坐标
	int y; // 按钮左上角y坐标
	int width; // 按钮宽度
	int height; // 按钮高度
	char* text; // 按钮内文字
	int dx; // 文字相对于按钮左上角x坐标的偏移值
	int dy; // 文字相对于按钮左上角y坐标的偏移值
	COLORREF buttoncolor; // 按钮颜色
	COLORREF textcolor; // 按钮中文字颜色
};

struct Point { // 点
	int x;
	int y;
};

struct PathNode { // 记录试探点
	int dir; // 记录当前点的试探方向
	bool isFind; // 记录当前试探点是否走过，0未走过，1走过
};

//-------可视化深度寻路类模型--------//

class Model {
private:
	int top; // 地图左上角y坐标
	int left; // 地图左上角x坐标
	int down; // 地图右下角y坐标
	int right; // 地图右下角x坐标
	vector<vector<int>> gameMapVec; // 存放地图信息
	vector<vector<PathNode>> gameNodeVec; // 存放试探点信息
	struct Point beginPoint; // 起点
	struct Point endPoint; // 终点
	stack<Point> mystack; // 栈
	struct Button* beginGame; // 开始按钮
	struct Button* endGame; // 结束按钮
public:
	explicit Model();
	~Model();
	struct Button*& getButton1();
	struct Button*& getButton2();
	void load(IMAGE& w1, IMAGE& ren, IMAGE& road, IMAGE& wall); // 加载资源
	void bgm(); // 加载音乐
	void init(); // 初始化数据
	void welcome(IMAGE& w1); // 程序开始绘制
	void draw(IMAGE& road, IMAGE& wall, IMAGE& ren); // 程序游戏绘制
	struct Button* creatButton(const int x, const int y, const int width, const int height, const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor); // 创建按钮
	void drawButton(struct Button* pButton); // 绘制按钮
	void depthFinding(IMAGE& road, IMAGE& wall, IMAGE& ren); // 深度寻路算法
	int mouseInButton(struct Button* pButton, MOUSEMSG* msg); // 判断鼠标是否在按钮内
	int mouseControl(struct Button* pButton1, struct Button* pButton2); // 鼠标消息响应
};