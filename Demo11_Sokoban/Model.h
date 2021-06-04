#pragma once // 避免头文件重复定义
#pragma comment(lib,"winmm.lib") // 加载静态库

#include <iostream>
#include <vector>
#include <conio.h>
#include <graphics.h>
#include <mmsystem.h>

using namespace std;

//-------推箱子游戏类模型--------//

class Model {
private:
	int flag; // 状态标识，判断关卡
	int top; // 地图左上角y坐标
	int left; // 地图左上角x坐标
	int down; // 地图右下角y坐标
	int right; // 地图右下角x坐标
	vector<vector<vector<int>>> gameMapVec; // 存放地图及关卡信息
public:
	explicit Model();
	~Model() = default;
	int& getFlag();
	void load(IMAGE& bk, vector<IMAGE>& img, vector<int>& imgIndex, const int pic_num, const int pic_size, const int width, const int height); // 加载资源
	void bgm(); // 加载音乐
	void init(const int width, const int height, const int grid_num, const int pic_size, const int cas); // 初始化数据
	void welcome(IMAGE& bk); // 绘制开始界面
	void draw(IMAGE& bk, vector<IMAGE>& img, const int grid_num, const int pic_size); // 绘制游戏界面
	void changOne(const int i, const int j); // 更改玩家位置
	void keyDown(const int grid_num); // 按键响应
	int over(const int grid_num); // 游戏结束判断
	void action(IMAGE& bk, vector<IMAGE>& img, const int grid_num, const int pic_size, const int width, const int height); // 结束动画
};