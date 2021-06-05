#pragma once // 避免头文件重复定义
#pragma comment(lib,"winmm.lib") // 加载静态库

#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

auto const width = 640; // 窗口宽度
auto const height = 720; // 窗口高度
auto const pic_width = 640; // 图片宽度
auto const pic_height = 905; // 图片高度
auto const fps_count = 8; // 帧率计算间隔时间

//-------律动水波类模型--------//

class Model {
private:
	DWORD* img_ptr1; // 原位图内存指针
	DWORD* img_ptr2; // 处理后位图内存指针
	short* buf; // 当前波幅
	short* buf2; // 下一刻波幅
	int flag; // 状态标识
public:
	explicit Model();
	~Model();
	void load(IMAGE& src_img); // 加载资源
	void bgm(); // 加载音乐
	void init(IMAGE& src_img, IMAGE& dest_img); // 初始化数据
	void nextFrame(const int damp); // 计算下一刻波幅
	void renderRipple(); // 处理位图
	void disturb(const int x, const int y, const int stonesize, const int stoneweight); // 鼠标投石头
	float getFps(); // 计算帧率FPS
	void renderFun(IMAGE& dest_img); // 画面渲染
	void frameFun(); // 画面逻辑
};