#pragma once // 避免头文件重复定义
#pragma comment(lib,"winmm.lib") // 加载静态库

#include <iostream>
#include <vector>
#include <conio.h>
#include <time.h>
#include <string.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

auto const width = 640; // 窗口宽度
auto const height = 720; // 窗口高度
auto const grid_num = 15; // 格子数量
auto const grid_size = 40; // 格子大小

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

struct Curse {
	int row; // 光标所在行数
	int col; // 光标所在列数
};

//-------五子棋大战游戏类模型--------//

class Model {
private:
	int flag; // 状态标识，用于判断黑方或白方胜利
	int top; // 地图左上角y坐标
	int left; // 地图左上角x坐标
	int down; // 地图右下角y坐标
	int right; // 地图右下角x坐标
	vector<vector<int>> gameMapVec; // 存放地图信息
	struct Curse curse1; // 玩家1光标
	struct Curse curse2; // 玩家2光标
	struct Button* beginGame; // 开始按钮
	struct Button* endGame; // 结束按钮
public:
	explicit Model();
	~Model();
	int& getFlag();
	struct Button*& getButton1();
	struct Button*& getButton2();
	void load(IMAGE& bk, IMAGE& w1, IMAGE& w2); // 加载资源
	void bgm(); // 加载音乐
	void init(); // 初始化数据
	bool timer(const int ms, const int id); // 定时器
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // PNG图片透明实现
	void welcome(IMAGE& w1, IMAGE& w2); // 程序开始绘制
	void draw(); // 程序游戏绘制
	struct Button* creatButton(const int x, const int y, const int width, const int height, const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor); // 创建按钮
	void drawButton(struct Button* pButton); // 绘制按钮
	int mouseInButton(struct Button* pButton, MOUSEMSG* msg); // 判断鼠标是否在按钮内
	int mouseControl(struct Button* pButton1, struct Button* pButton2); // 鼠标消息响应
	void keyDown(); // 按键消息响应
	int condition1(); // 结束条件1
	int condition2(); // 结束条件2
	bool over(); // 结束判断
};