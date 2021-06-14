#pragma once // 避免头文件重复定义
#pragma comment(lib,"winmm.lib") // 加载静态库

#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

auto const width = 640; // 窗口宽度
auto const height = 720; // 窗口高度
auto const pic_size = 80; // 图片大小
auto const pic_num = 4; // 图片数量
auto const line_num = 7; // 线段数量
auto const car_num = 3; // 车子数量
auto const row = (height / pic_size); // 行
auto const col = (width / pic_size); // 列

struct Button { // 按钮
	int x; // 左上角x坐标
	int y; // 左上角y坐标
	int width; // 按钮宽度
	int height; // 按钮高度
	char* text; // 按钮内文字
	int dx; // 文字相对于按钮左上角x坐标的偏移值
	int dy; // 文字相对于按钮左上角y坐标的偏移值
	COLORREF buttoncolor; // 按钮颜色
	COLORREF textcolor; // 按钮内文字颜色
};

struct Role { // 角色
	double x; // 角色x坐标
	double y; // 角色y坐标
	double step; // 角色每次位移
};

struct Car { // 车子
	double x; // 车子的x坐标
	double y; // 车子的y坐标
	double step; // 车子每次的位移
	int kind; // 车子的种类
	int road; // 车子所在道路
};

struct Line { // 线段
	double FL_x; // 左边线段x坐标
	double FL_y; // 左边线段y坐标
	double SL_x; // 右边线段x坐标
	double SL_y; // 右边线段y坐标
	double step; // 线段每次位移量
	int width; // 线段的宽度
	int height; // 线段的高度
};

//-------赛车竞速游戏类模型--------//

class Model {
private:
	vector<Car> cars; // 车
	vector<Line> lines; // 线
	struct Role role; // 角色
	struct Button* beginGame; // 开始按钮
	struct Button* endGame; // 结束按钮
public:
	explicit Model();
	~Model();
	struct Button*& getButton1();
	struct Button*& getButton2();
	void load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& track, vector<IMAGE>& img, vector<int>& imgIndex); // 加载资源
	void bgm(); // 加载音乐
	void init(); // 初始化数据
	double distance(const double x1, const double y1, const double x2, const double y2); // 求两点间距离
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // PNG图片透明实现
	void welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& track, vector<IMAGE>& img); // 绘制开始界面
	void draw(IMAGE& bk, IMAGE& track, vector<IMAGE>& img); // 绘制游戏界面
	struct Button* creatButton(const int x, const int y, const int width, const int height, const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor); // 创建按钮
	void drawLayout(IMAGE& bk, IMAGE& track); // 绘制赛道
	void drawButton(struct Button* pButton); // 绘制按钮
	void drawLine(); // 绘制线条
	void moveLine(); // 线条移动
	void moveCar(); // 汽车移动
	int mouseInButton(struct Button* pButton, MOUSEMSG* msg); // 判断鼠标是否在按钮内
	int mouseControl(struct Button* pButton1, struct Button* pButton2); // 鼠标消息响应
	void keyDown(); // 按键消息响应
	bool over(); // 结束条件判断
};