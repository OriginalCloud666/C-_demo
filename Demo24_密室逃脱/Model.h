#pragma once // 避免头文件重复定义
#pragma comment(lib,"winmm.lib") // 加载静态库

#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include <math.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

constexpr auto WIDTH = 1280; // 窗口宽度
constexpr auto HEIGHT = 720; // 窗口高度
constexpr auto GRID_SIZE = 20; // 格子大小
constexpr auto ENEMY_NUM = 24; // 敌军数量
constexpr auto ROW = (HEIGHT / GRID_SIZE); // 行
constexpr auto COL = (WIDTH / GRID_SIZE); // 列
constexpr auto PI = 3.1415926; // 定义π

enum Tools // 工具
{
	BLANK = 0, // 空地
	WALL = 1, // 墙壁
	DOOR = 2, // 终点
};

enum Dir // 方向
{
	NONE = 0, // 静止状态
	UP = 1, // 上方向
	DOWN = 2, // 下方向
	LEFT = 3, // 左方向
	RIGHT = 4, // 右方向
};

struct Button // 按钮
{
	int x; // 左上角x坐标
	int y; // 左上角y坐标
	int width; // 宽度
	int height; // 高度
	char* text; // 文字
	int dx; // 文字相对于按钮左上角x坐标的偏移值
	int dy; // 文字相对于按钮左上角y坐标的偏移值
	COLORREF buttoncolor; // 颜色
	COLORREF textcolor; // 文字颜色
};

struct Role // 玩家
{
	double x; // 玩家x坐标
	double y; // 玩家y坐标
	double step; // 玩家移动速度
	int dir; // 玩家移动方向
};

struct Enemy // 敌人
{
	double x; // 敌人x坐标
	double y; // 敌人y坐标
	double step; // 敌人移动速度
	double tempstep; // 记录敌人初始移动速度
	double steprange; // 记录速度变化幅度
	int flag; // 敌人移动转向的状态标识
};

//-------密室逃脱游戏类模型--------//

class Model
{
public:
	explicit Model();
	~Model();

	struct Button*& getButton1();
	struct Button*& getButton2();
	void load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE enem[], IMAGE play[]); // 加载资源
	void bgm(); // 加载音乐
	void init(); // 初始化数据
	void initWall(); // 初始化墙壁
	double distance(const double x1, const double y1, const double x2, const double y2); // 求两点间距离
	void welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2); // 绘制开始界面
	void draw(IMAGE play[], IMAGE enem[]); // 绘制游戏界面
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // PNG图片透明实现
	struct Button* creatButton(const int x, const int y, const int width, const int height, const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor); // 创建按钮
	void drawButton(struct Button* pButton); // 绘制按钮
	void drawRole(struct Role* role, IMAGE play[]); // 绘制人物
	void drawEnemy(struct Enemy* enemy, IMAGE enem[]); // 绘制敌人
	void moveRole(); // 角色移动
	void moveEnemy(); // 敌人移动
	int mouseInButton(struct Button* pButton, MOUSEMSG* msg); // 判断鼠标是否在按钮内
	int mouseControl(struct Button* pButton1, struct Button* pButton2); // 鼠标消息响应
	void keyDown(); // 按键消息响应
	int condition1(); // 结束条件1
	int condition2(); // 结束条件2
	bool over(); // 结束判断

private:
	int top; // 地图左上角y坐标
	int left; // 地图左上角x坐标
	int down; // 地图右下角y坐标
	int right; // 地图右下角x坐标
	vector<vector<int>> gameMapVec; // 存放地图信息
	struct Role role; // 角色
	vector<Enemy> enemys; // 敌人
	struct Button* beginGame; // 开始按钮
	struct Button* endGame; // 结束按钮
};
