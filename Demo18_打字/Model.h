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

constexpr auto WIDTH = 640; // 窗口宽度
constexpr auto HEIGHT = 720; // 窗口高度
constexpr auto PIC_NUM = 3; // 图片数量
constexpr auto PIC_SIZE = 150; // 图片大小
constexpr auto STR_NUM = 5; // 字符串数量

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

struct Target // 下坠的字符串
{
	double x; // 贴图x坐标
	double y; // 贴图y坐标
	char* str; // 保存字符串
	double dx; // 字符串与图片的偏移坐标
	double dy; // 字符串与图片的偏移坐标
	double step; // 字符串移动速度
	bool flag; // 状态标识，用于判断字符串是否存在
};

struct Userkey // 用户输入的值
{
	int x; // 字符串x坐标
	int y; // 字符串y坐标
	char str[20]; // 保存字符串
};

//-------打字游戏类模型--------//

class Model
{
public:
	explicit Model();
	~Model();

	struct Button*& getButton1();
	struct Button*& getButton2();
	void load(IMAGE& kk, IMAGE& w1, IMAGE& w2, vector<IMAGE>& img); // 加载资源
	void bgm(); // 加载音乐
	void init(); // 初始化数据
	int timer(time_t sec, int id); // 定时器
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // PNG图片透明实现
	void welcome(IMAGE& w1, IMAGE& w2, vector<IMAGE>& img); // 程序开始绘制
	void draw(IMAGE& kk); // 程序游戏绘制
	struct Button* creatButton(const int x, const int y, const int width, const int height, const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor); // 创建按钮
	void drawButton(struct Button* pButton); // 绘制按钮
	void outtextxy_int(int x, int y, const char* format, int num); // 指定位置输出整数
	void outtextxy_double(int x, int y, const  char* format, double num); // 指定位置输出浮点数
	void targetCreat(struct Target words[], int n); // 生成字符串
	void targetMove(); // 字符串下落
	int mystrcmp(const char* s1, const char* s2); // 带空格的字符串比较大小
	void errorStr(); // 计算遗落的字符串
	int mouseInButton(struct Button* pButton, MOUSEMSG* msg); // 判断鼠标是否在按钮内
	int mouseControl(struct Button* pButton1, struct Button* pButton2); // 鼠标消息响应
	void keyDown(); // 按键消息响应
	bool over(); // 结束判断

private:
	int score; // 正确输入的字符数量
	int error; // 遗漏输入的字符数量
	int count; // 总共出现的字符数量
	double rate; // 输入字符的正确率
	struct Userkey userkey; // 用户输入的值
	struct Target target[STR_NUM]; // 下坠的字符串
	struct Button* beginGame; // 开始按钮
	struct Button* endGame; // 结束按钮
};
