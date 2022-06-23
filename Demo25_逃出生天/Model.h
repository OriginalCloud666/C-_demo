#pragma once // 避免头文件重复定义
#pragma comment(lib,"winmm.lib") // 加载静态库

#include <iostream>
#include <vector>
#include <conio.h>
#include <string.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

constexpr auto WIDTH = 1280; // 窗口宽度
constexpr auto HEIGHT = 720; // 窗口高度
constexpr auto GRID_SIZE = 20; // 格子大小
constexpr auto ROW = 24; // 行数
constexpr auto COL = 40; // 列数
constexpr auto MAXCAS = 3; // 关卡数量

enum Tools // 工具
{
	BLANK = 0, // 空地
	WALL = 1, // 墙壁
	ROLE = 2, // 玩家
	FIRE = 3, // 火焰
	DOOR = 4, // 终点
	GHOST = 5, // 幽灵
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
	int x; // x坐标
	int y; // y坐标
	int width; // 宽度
	int height; // 高度
	char* text; // 文字
	int dx; // 按钮内文字相对于按钮的偏移值
	int dy; // 按钮内文字相对于按钮的偏移值
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

//-------逃出升天游戏类模型--------//

class Model
{
public:
	explicit Model();
	~Model();

	int& getCas();
	struct Button*& getButton1();
	struct Button*& getButton2();
	void load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& man, IMAGE& brick, IMAGE& replace, IMAGE fire[], IMAGE door[], IMAGE ghost[], IMAGE character[]); // 加载资源
	void bgm(); // 加载音乐
	void init(); // 初始化数据
	void map1(); // 初始化地图
	void map2(); // 初始化地图
	void map3(); // 初始化地图
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // PNG图片透明实现
	void welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& man); // 绘制开始界面
	void draw(IMAGE& bk, IMAGE& brick, IMAGE fire[], IMAGE door[], IMAGE ghost[], IMAGE character[]); // 绘制游戏界面
	struct Button* creatButton(const int x, const int y, const int width, const int height, const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor); // 创建按钮
	void drawButton(struct Button* pButton); // 绘制按钮
	void drawFire(const int x, const int y, IMAGE fire[]); // 贴火焰图片
	void drawDoor(const int x, const int y, IMAGE door[]); // 贴终点图片
	void drawGhost(const int x, const int y, IMAGE ghost[]); // 贴幽灵图片
	void drawRole(IMAGE character[]); // 贴人物图片
	void searchRole(); // 搜寻人物图片
	void moveRole(IMAGE& bk, IMAGE& brick, IMAGE& replace, IMAGE fire[], IMAGE door[], IMAGE ghost[], IMAGE character[]); // 角色移动
	int mouseInButton(struct Button* pButton, MOUSEMSG* msg); // 判断鼠标是否在按钮内
	int mouseControl(struct Button* pButton1, struct Button* pButton2); // 鼠标消息响应
	void keyDown(); // 按键响应
	int condition1(); // 结束条件1
	int condition2(); // 结束条件2
	bool over(); // 结束条件判断

private:
	int cas;// 用于决定游戏关卡
	int top; // 地图左上角y坐标
	int left; // 地图左上角x坐标
	int down; // 地图右下角y坐标
	int right; // 地图右下角x坐标
	struct Role role; // 角色
	vector<vector<vector<int>>> gameMapVec; // 存放地图信息
	struct Button* beginGame; // 开始按钮
	struct Button* endGame; // 结束按钮
};
