#pragma once // 避免头文件重复定义
#pragma comment(lib,"winmm.lib") // 加载静态库

#include <iostream>
#include <vector>
#include <conio.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

auto const width = 640; // 窗口宽度
auto const height = 720; // 窗口高度
auto const pic_num = 10; // 图片数量
auto const pic_size = 50; // 图片大小
auto const grid_num = 8; // 地图行列

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
	void load(IMAGE& bk, vector<IMAGE>& img, vector<int>& imgIndex); // 加载资源
	void bgm(); // 加载音乐
	void init(); // 初始化数据
	void welcome(IMAGE& bk); // 绘制开始界面
	void draw(IMAGE& bk, vector<IMAGE>& img); // 绘制游戏界面
	void changOne(const int i, const int j); // 更改玩家位置
	void keyDown(); // 按键响应
	int over(); // 游戏结束判断
	void action(IMAGE& bk, vector<IMAGE>& img); // 结束动画
};