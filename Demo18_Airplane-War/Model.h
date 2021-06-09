#pragma once // 避免头文件重复定义
#pragma comment(lib,"winmm.lib") // 加载静态库

#include "Plane.h"

#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

auto const width = 640; // 窗口宽度
auto const height = 720; // 窗口高度
auto const enemy1_num = 1; // 大型敌机数量
auto const enemy2_num = 8; // 小型敌机数量
auto const bullet_step = 1; // 子弹射击速度

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

//-------飞机大战游戏类模型--------//

class Model {
private:
	int score; // 用于记录最终得分
	int cas; // 用于控制飞机爆炸动画
	Plane role; // 我军飞机
	vector<Plane> enemy1; // 敌军大型飞机
	vector<Plane> enemy2; // 敌军小型飞机
	struct Button* beginGame; // 开始按钮
	struct Button* endGame; // 结束按钮
public:
	explicit Model();
	~Model();
	int& getScore();
	Plane& getRole();
	struct Button*& getButton1();
	struct Button*& getButton2();
	vector<Plane>& getEnemy1();
	vector<Plane>& getEnemy2();
	void load(IMAGE& bk, IMAGE& begin, IMAGE& author, IMAGE plane[][2], IMAGE enemy_big[], IMAGE enemy_small[], IMAGE bullet[]); // 加载资源
	void bgm(); // 加载音乐
	void init(); // 初始化数据
	bool timer(const int ms, const int id); // 定时器
	void welcome(IMAGE& bk, IMAGE& begin, IMAGE& author); // 绘制开始界面
	void draw(IMAGE& bk, IMAGE enemy_big[], IMAGE enemy_small[], IMAGE plane[][2], IMAGE bullet[]); // 绘制游戏界面
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // PNG透明图片
	double distance(const double x1, const double y1, const double x2, const double y2); // 求两点间距离
	struct Button* creatButton(const int x, const int y, const int width, const int height, const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor); // 创建按钮
	void drawButton(struct Button* pButton); // 绘制按钮
	void moveEnemy(); // 敌机移动
	void hitEnemy(vector<Plane>& enemy, const int enemy_num, const int hp); // 敌机碰撞消失
	int mouseInButton(struct Button* pButton, MOUSEMSG* msg); // 判断鼠标是否在按钮中
	int mouseControl(struct Button* pButton1, struct Button* pButton2); // 鼠标消息响应
	void keyDown1(); // 键盘消息响应（异步）
	void keyDown2(); // 键盘消息响应（同步）
	bool condition(vector<Plane>& enemy); // 游戏结束条件
	bool over(); // 结束判断
};