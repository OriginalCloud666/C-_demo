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
auto const pic_size = 160; // 图片大小
auto const board_num = 5; // 木板数量
auto const row = (height / pic_size + 1); //行
auto const col = (width / pic_size); // 列

enum DIR { // 方向
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3
};

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

struct Board { // 木板
	double x; // 木板x坐标
	double y; // 木板y坐标
	int width; // 木板宽度
	int height; // 木板高度
	int type; // 木板类型
};

struct Role { // 角色
	double x; // 角色x坐标
	double y; // 角色y坐标
	int width; // 角色宽度
	int height; // 角色高度
	double step; // 角色移动速度
	int dir; // 角色所处方向
	int dirnum[4]; // 方向序列号
	double tempdir[4]; // 控制图片切换速度
	bool flag; // 状态标识，用于判断角色是否在木板上
	bool nail; // 状态标识，用于判短角色是否在带有尖刺的木板上
};

//-------勇者闯关游戏类模型--------//

class Model {
private:
	int count; // 用于记录总共层数
	int score; // 用于记录当前层数
	vector<Board> boards; // 木板
	struct Role role; // 角色
	struct Button* beginGame; // 开始按钮
	struct Button* endGame; // 结束按钮
public:
	explicit Model();
	~Model();
	int& getScore();
	struct Button*& getButton1();
	struct Button*& getButton2();
	void load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& stage1, IMAGE stage2[], IMAGE& apron, IMAGE& nail, IMAGE& hp, IMAGE player[]); // 加载资源
	void bgm(); // 加载音乐
	void init(); // 初始化数据
	bool timer(const int ms, const int id); // 定时器
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // PNG图片透明实现
	void welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& nail, IMAGE& apron); // 绘制开始界面
	void draw(IMAGE& bk, IMAGE& nail, IMAGE& apron, IMAGE& stage1, IMAGE stage2[], IMAGE player[]); // 绘制游戏界面
	struct Button* creatButton(const int x, const int y, const int width, const int height, const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor); // 创建按钮
	void drawButton(struct Button* pButton); // 绘制按钮
	void drawBackgound(IMAGE& bk); // 绘制背景
	void drawNail(IMAGE& nail); // 绘制尖刺
	void drawApron(IMAGE& apron); // 绘制墙壁
	void drawStage02(const int x, const int y, IMAGE stage2[]); // 贴带尖刺木板
	void drawBoard(const int i, IMAGE& stage1, IMAGE stage2[]); // 绘制木板
	void drawRole(IMAGE player[]); // 绘制角色
	void moveBoard(); // 木板移动
	void sumScore(); // 计算层数
	void overAction(IMAGE& bk, IMAGE& nail, IMAGE& apron, IMAGE& stage1, IMAGE stage2[], IMAGE player[]); // 结束动画
	int mouseInButton(struct Button* pButton, MOUSEMSG* msg); // 判断鼠标是否在按钮内
	int mouseControl(struct Button* pButton1, struct Button* pButton2); // 鼠标消息响应
	void keyDown(); // 按键消息响应
	bool over(); // 结束判断
};