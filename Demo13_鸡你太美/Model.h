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
constexpr auto GRID_NUM = 8; // 地图行列
constexpr auto PIC_NUM = 6; // 图片数量
constexpr auto PIC_SIZE = 50; // 图片大小

struct Button // 按钮
{
	int x; // x坐标
	int y; // y坐标
	int width; // 按钮宽度
	int height; // 按钮高度
	COLORREF color1; // 内层边框颜色
	COLORREF color2; // 外层边框颜色
};

//-------鸡你太美游戏类模型--------//

class Model
{
public:
	explicit Model();
	~Model();

	int& getFlag();
	struct Button*& getBeginGame();
	void load(IMAGE& bk, IMAGE& button, vector<IMAGE>& img, vector<int>& imgIndex); // 加载资源
	void bgm(); // 加载音乐
	void init(); // 初始化数据
	void welcome(IMAGE& bk); // 绘制开始界面
	void draw(IMAGE& bk, IMAGE& button, vector<IMAGE>& img); // 绘制游戏界面
	struct Button* creatButton(int x, int y, int width, int height, COLORREF color1, COLORREF color2); // 创建按钮
	void drawButton(struct Button* pButton, IMAGE& button); // 绘制按钮
	int search_i(); // 找到人物横坐标
	int search_j(); // 找到人物纵坐标
	void keyDown(); // 按键响应（同步）
	int mouseInButton(struct Button* pButton, MOUSEMSG* msg); // 判断鼠标是否在按钮内
	void mouseControl(struct Button* pButton); // 鼠标消息响应
	bool over(); // 结束条件判断

private:
	int flag; // 状态标识，判断关卡
	int top; // 地图左上角y坐标
	int left; // 地图左上角x坐标
	int down; // 地图右下角y坐标
	int right; // 地图右下角x坐标
	struct Button* beginGame; // 开始按钮
	vector<vector<vector<int>>> gameMapVec; // 存放地图及关卡信息
};
