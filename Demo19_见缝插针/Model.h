#pragma once // 避免头文件重复定义
#pragma comment(lib,"winmm.lib") // 加载静态库

#include <iostream>
#include <vector>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

constexpr auto WIDTH = 640; // 窗口宽度
constexpr auto HEIGHT = 720; // 窗口高度
constexpr auto PIC_NUM = 4; // 图片数量
constexpr auto NEEDLE_NUM = 36; // 针的数量
constexpr auto PI = 3.1415926; // 定义π

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
	COLORREF textcolor; // 中文字颜色
};

struct Needle // 针
{
	double x; // 针末端的x坐标
	double y; // 针末端的y坐标
	double len; // 针的长度
	double rotateSpeed; // 所有旋转针的旋转速度
	double angle; // 所有旋转的针的旋转角度
	int flag; // 状态标识，用于判断针为哪个玩家发射
};

struct Ball // 球
{
	double x; // 球的x坐标
	double y; // 球的y坐标
	double r; // 球的半径
};

struct Role // 玩家
{
	double x; // 玩家x坐标
	double y; // 玩家y坐标
	struct Ball ball; // 玩家尾部射出的球
	double step; // 玩家尾部射出的球的速度
	int score; // 玩家的得分
	int count; // 玩家射出的总针数
};

//-------见缝插针游戏类模型--------//

class Model
{
public:
	explicit Model();
	~Model();

	int& getNeedleNum();
	double& getFinaltime();
	struct Role& getRole1();
	struct Role& getRole2();
	struct Button*& getButton1();
	struct Button*& getButton2();
	void load(IMAGE& bk, IMAGE& w1, IMAGE& w2, vector<IMAGE>& img, vector<int>& imgIndex); // 加载资源
	void bgm(); // 加载音乐
	void init(); // 初始化数据
	int timer(time_t sec, int id); // 定时器
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // PNG图片透明实现
	void welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2); // 绘制开始界面
	void draw(IMAGE& bk, vector<IMAGE>& img); // 绘制游戏界面
	struct Button* creatButton(const int x, const int y, const int width, const int height, const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor); // 创建按钮
	void drawButton(struct Button* pButton); // 绘制按钮
	void drawRole1(vector<IMAGE>& img); // 绘制玩家一
	void drawRole2(vector<IMAGE>& img); // 绘制玩家二
	void rotateNeedle(); // 针的旋转
	int mouseInButton(struct Button* pButton, MOUSEMSG* msg); // 判断鼠标是否在按钮内
	int mouseControl(struct Button* pButton1, struct Button* pButton2); // 鼠标消息响应
	void keyDown(IMAGE& bk, vector<IMAGE>& img); // 按键消息响应
	bool over(); // 结束判断

private:
	int needleNum; // 旋转的针的数量
	double finaltime; // 用于记录倒计时时间
	struct Role role1; // 玩家一
	struct Role role2; // 玩家二
	struct Ball ball; // 中部球体
	vector<Needle> needleVec; // 存放所有针
	struct Button* beginGame; // 开始按钮
	struct Button* endGame; // 结束按钮
};
