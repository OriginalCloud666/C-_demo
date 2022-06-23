#pragma once // 避免头文件重复定义
#pragma comment(lib,"winmm.lib") // 加载静态库

#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

constexpr auto WIDTH = 640; // 窗口宽度
constexpr auto HEIGHT = 720; // 窗口高度
constexpr auto BRID_SIZE = 60; // 小鸟大小
constexpr auto PILLAR_NUM = 3; // 柱子数量
constexpr auto MOVE_STEP = 0.7; // 柱子及背景移动速度

struct Bird // 小鸟
{
	double x; // 贴柱子左上角x坐标
	double y; // 贴柱子坐上角y坐标
	double speed; // 下坠速度
};

struct Pillar // 柱子
{
	double x; // 贴柱子左上角x坐标
	int height; // 柱子显示的高度
	int flag = 0; // 状态标识，判断柱子是否消失
};

struct BkDown // 背景图下部分地面
{
	double x; // 贴地面图坐上角x坐标
	int flag = 0; // 状态标识，判断地面是否消失
};

struct Button // 按钮
{
	int x; // x坐标
	int y; // y坐标
	int width; // 按钮宽度
	int height; // 按钮高度
	char* text; // 按钮内文字
	int dx; // 文字相对于按钮左上角x坐标的偏移值
	int dy; // 文字相对于按钮左上角y坐标的偏移值
	COLORREF buttoncolor; // 按钮颜色
	COLORREF textcolor; // 按钮中文字颜色
};

//-------飞翔小鸟游戏类模型--------//

class Model
{
public:
	explicit Model();
	~Model();

	int& getScore();
	struct Button*& getButton1();
	struct Button*& getButton2();
	void load(IMAGE& bk, IMAGE mybird[][2], IMAGE guid[], IMAGE up[], IMAGE down[], IMAGE start[], IMAGE end[]); // 加载资源
	void bgm(); // 加载音乐
	void init(); // 初始化数据
	void welcome(IMAGE& bk, IMAGE guid[]); // 绘制开始界面
	void draw(IMAGE& bk, IMAGE up[], IMAGE down[], IMAGE mybird[][2]); // 绘制游戏界面
	struct Button* creatButton(const int x, const int y, const int width, const int height, const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor); // 创建按钮
	void drawButton(struct Button* pButton); // 绘制按钮
	void drawGuid(IMAGE guid[], const int x, const int y); // 绘制引导图片
	void drawBird(IMAGE mybird[][2], const int x, const int y); // 绘制小鸟图片
	void drawPillar(IMAGE up[], IMAGE down[], struct Pillar mypillar); // 绘制柱子图片
	void drawBackground(IMAGE& bk, struct BkDown mybkdown); // 绘制地面图片
	void createPillar(const int i); // 产生柱子
	void createBkDown(const int i); // 产生地面
	void moveBird(); // 鸟的移动
	void movePillar(); // 柱子移动
	void moveBkDown(); // 地面移动
	int hitPillar(); // 柱子碰撞
	int mouseControl(struct Button* pButton1, struct Button* pButton2); // 鼠标消息响应
	int mouseInButton(struct Button* pButton, MOUSEMSG* msg); // 判断鼠标是否在按钮中
	void keyDown(); // 按键响应
	int condition1(); // 结束条件1
	int condition2(); // 结束条件2
	bool over(); // 结束判断
	void beginAction(IMAGE& bk, IMAGE guid[], IMAGE start[]); // 开始动画
	void flyAction(); // 飞翔动画
	void hitAction(IMAGE& bk, IMAGE up[], IMAGE down[], IMAGE mybird[][2]); // 撞击动画
	void endAction(IMAGE& bk, IMAGE up[], IMAGE down[], IMAGE mybird[][2], IMAGE end[]); // 结束动画

private:
	int score; // 记录分数
	int cas; // 小鸟动画设置
	double tempcas; // 小鸟动画设置
	struct Bird bird; // 小鸟
	vector<Pillar> pillar; // 柱子
	vector<BkDown> bkdown; // 背景及地面
	struct Button* beginGame; // 开始按钮
	struct Button* endGame; // 结束按钮
};
