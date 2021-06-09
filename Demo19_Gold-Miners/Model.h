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
auto const pic_num = 8; // 图片数量
auto const gold1_num = 3; // 小金子数量
auto const gold2_num = 3; // 大金子数量
auto const stone_num = 3; // 石头数量
auto const sack_num = 4; // 袋子数量
auto const pi = 3.1415926; // 定义π

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

struct Role { // 人物
	int x; // 人物x坐标
	int y; // 人物y坐标
	int width; // 人物图片宽度
	int height; // 人物图片高度
};

struct Mine { // 物品
	double x; // 物品x坐标
	double y; // 物品y坐标
	int width; // 物品宽度
	int height; // 物品高度
	double weight; // 物品重量
	bool flag; // 判断物品是否还存在
	int value; // 物品价值
};

struct Hook { // 钩子
	double start_x; // 钩子起始位置x坐标
	double start_y; // 钩子起始位置y坐标
	double len; // 钩子长度
	double end_x; // 钩子结束位置x坐标
	double end_y; // 钩子结束位置y坐标
	double radian; // 钩子与水平面弧度
	int dir; // 钩子移动方向，左，右
	bool rockflag; // 状态标识，用于判断钩子是否移动
	bool flexflag; // 状态标识，用于判断钩子是否伸缩
};

//-------黄金矿工游戏类模型--------//

class Model {
private:
	int aim; // 记录目标得分
	int coin; // 记录金币得分
	int cas; // 控制人物动画
	double tempcas; // 动画的控制
	struct Role role; // 角色
	struct Hook hook; // 钩子
	vector<Mine> gold1; // 小金子
	vector<Mine> gold2; // 大金子
	vector<Mine> stone; // 石头
	vector<Mine> sack; // 袋子
	struct Button* beginGame; // 开始按钮
	struct Button* endGame; // 结束按钮
public:
	explicit Model();
	~Model();
	struct Button*& getButton1();
	struct Button*& getButton2();
	void load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE rolen[][2], vector<IMAGE>& img, vector<int>& imgIndex); // 加载资源
	void bgm(); // 加载音乐
	void init(); // 初始化数据
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // PNG图片透明实现
	void welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE rolen[][2], vector<IMAGE> img); // 绘制开始界面
	void draw(IMAGE& bk, IMAGE rolen[][2], vector<IMAGE> img); // 绘制游戏界面
	struct Button* creatButton(const int x, const int y, const int width, const int height, const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor); // 创建按钮
	void drawButton(struct Button* pButton); // 绘制按钮
	void drawHook(); // 绘制钩子
	void drawRole(IMAGE rolen[][2]); // 绘制角色
	void drawGold1(vector<IMAGE> img); // 绘制金子
	void drawGold2(vector<IMAGE> img); // 绘制金子
	void drawStone(vector<IMAGE> img); // 绘制石头
	void drawSack(vector<IMAGE> img); // 绘制袋子
	void rockHook(); // 钩子移动
	void grabHook(); // 钩子抓取
	int mouseInButton(struct Button* pButton, MOUSEMSG* msg); // 判断鼠标是否在按钮内
	int mouseControl(struct Button* pButton1, struct Button* pButton2); // 鼠标消息响应
	void keyDown(); // 按键消息响应
	bool over(); // 结束条件判断
};