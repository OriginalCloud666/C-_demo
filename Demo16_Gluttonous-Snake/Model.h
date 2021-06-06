#pragma once // 避免头文件重复定义
#pragma comment(lib,"winmm.lib") // 加载静态库

#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

auto const width = 640; // 窗口宽度
auto const height = 720; // 窗口高度
auto const grid_width = 16; // 格子宽度
auto const grid_height = 16; // 格子高度
auto const grid_row = (height / grid_height); // 格子行数
auto const grid_col = (width / grid_width); // 格子列数
auto const snake_size = (grid_row * grid_col); // 蛇最大节数

enum Direction {
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3,
};

struct Button { // 按钮
	int x; // 左上角x坐标
	int y; // 左上角y坐标
	int width; // 按钮宽度
	int height; // 按钮高度
	char* text; // 按钮内文字
	int dx; // 文字相对于按钮左上角x坐标的偏移值
	int dy; // 文字相对于按钮左上角y坐标的偏移值
	COLORREF buttoncolor; // 按钮颜色
	COLORREF textcolor; // 按钮中文字颜色
};

struct Point { // 点
	int x; // x坐标
	int y; // y坐标
};

struct Snake { // 蛇
	vector<Point> point; // 蛇最大能有多少节
	int size; // 蛇当前有多少节
	int dir; // 蛇头的方向
};

struct Food { // 食物
	struct Point point; // 食物的x，y坐标
	int flag; // 用来判断食物是否存在
};

//-------贪吃蛇游戏类模型--------//

class Model {
private:
	struct Food food; // 食物
	struct Snake snake; // 角色蛇
	struct Button* beginGame; // 开始按钮
	struct Button* endGame; // 结束按钮
public:
	explicit Model();
	~Model() = default;
	struct Food& getFood();
	struct Snake& getSnake();
	struct Button*& getButton1();
	struct Button*& getButton2();
	void load(IMAGE& bk); // 加载资源
	void bgm(); // 加载音乐
	void init(); // 初始化数据
	void welcome(IMAGE& bk); // 绘制开始界面
	void draw(IMAGE& bk); // 绘制游戏界面
	struct Button* creatButton(const int x, const int y, const int width, const int height, const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor); // 创建按钮
	void drawButton(struct Button* pButton); // 绘制按钮
	int mouseInButton(struct Button* pButton, MOUSEMSG* msg); // 判断鼠标是否在按钮内
	int mouseControl(struct Button* pButton1, struct Button* pButton2); // 鼠标消息响应
	void keyDown(); // 键盘消息响应
	void snackMove(); // 蛇的移动
	void creatFood(); // 产生食物
	void eatFood(); // 食物被吃掉
	int condition1(); // 结束条件1
	int condition2(); // 结束条件2
	bool over(); // 结束条件判断
};