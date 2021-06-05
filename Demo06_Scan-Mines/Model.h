#pragma once // 避免头文件重复定义
#pragma comment(lib,"winmm.lib") // 加载静态库

#include <iostream>
#include <vector>
#include <conio.h>
#include <time.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

auto const width = 640; // 窗口宽度
auto const height = 720; // 窗口高度
auto const pic_size = 32; // 图片大小
auto const pic_num = 12; // 图片数量

auto const map_size1 = 9; // 地图大小（小）
auto const map_size2 = 13; // 地图大小（中）
auto const map_size3 = 17; // 地图大小（大）
auto const mine_num1 = 10; // 地雷数量（少）
auto const mine_num2 = 26; // 地雷数量（中）
auto const mine_num3 = 47; // 地雷数量（多）

//-------扫雷游戏类模型--------//

class Model {
private:
	int flag; // 状态标识，标记是否胜利
	clock_t start; // 标记程序开始时间
	clock_t finish; // 标记程序结束时间
	double duration; // 记录程序运行时间
	int mine_num; // 地图区域地雷数量
	int row; // 地图区域行数
	int col; // 地图区域列数
	int top; // 地图左上角y坐标
	int left; // 地图左上角x坐标
	int down; // 地图右下角y坐标
	int right; // 地图右下角x坐标
	vector<vector<int>> gameMapVec; // 存放地图信息
public:
	explicit Model();
	~Model() = default;
	clock_t& getStart();
	clock_t& getFinish();
	void load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3, IMAGE& mine, vector<IMAGE>& img, vector<int>& imgIndex); // 加载资源
	void bgm(); // 加载音乐
	void init(const int row, const int col, const int mine_num); // 初始化数据
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // 载入PNG透明图片
	void welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3); // 绘制开始界面
	void draw(IMAGE& bk, IMAGE& mine, vector<IMAGE>& img); // 绘制游戏界面
	void openNull(const int cur_row, const int cur_col); // 打开空格周围格子
	int mouseControl(); // 鼠标消息响应
	void keyDown(); // 键盘消息响应
	void over(IMAGE& bk, IMAGE& mine, vector<IMAGE>& img, const int judgeflag); // 游戏结束判断
};