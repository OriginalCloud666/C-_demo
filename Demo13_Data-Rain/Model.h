#pragma once // 避免头文件重复定义
#pragma comment(lib,"winmm.lib") // 加载静态库

#include <iostream>
#include <vector>
#include <conio.h>
#include <time.h>
#include <string>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

auto const width = 1280; // 窗口宽度
auto const height = 720; // 窗口高度
auto const str_width = 15; // 字符串宽度
auto const str_num = 86; // 字符串数目
auto const str_size = 20; // 字符串内字符数

struct Rain { // 数据雨
	char* str; // 字符串大小
	double x; // 字符串的x坐标
	double y; // 字符串的y坐标
	double step; // 字符串每次移动的距离
};

//-------数据雨类模型--------//

class Model {
private:
	vector<Rain> rainVec; // 数据雨
public:
	explicit Model() = default;
	~Model() = default;
	void bgm(); // 加载音乐
	void init(); // 初始化数据
	char createChar(); // 产生字符
	void rainChange(); // 数字雨不断变化
	void rainMove(); // 数字雨移动
	void printChar(); // 输出显示字符
};