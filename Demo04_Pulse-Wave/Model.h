#pragma once // 避免头文件重复定义
#pragma comment(lib,"winmm.lib") // 加载静态库

#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <graphics.h>
#include <mmsystem.h>

using namespace std;

//-------律动水波类模型--------//

class Model {
private:
	DWORD* img_ptr1; // 原位图内存指针
	DWORD* img_ptr2; // 处理后位图内存指针
	short* buf; // 当前波幅
	short* buf2; // 下一刻波幅
	int flag; // 状态标识
public:
	explicit Model() = delete;
	explicit Model(const int pic_width, const int pic_height, const int width);
	~Model();
	void load(IMAGE& src_img, const int pic_width, const int pic_height); // 加载资源
	void bgm(); // 加载音乐
	void init(IMAGE& src_img, IMAGE& dest_img, const int pic_width, const int pic_height); // 初始化数据
	void nextFrame(const int damp, const int pic_width, const int pic_height, const int width); // 计算下一刻波幅
	void renderRipple(const int pic_width, const int pic_height); // 处理位图
	void disturb(const int x, const int y, const int stonesize, const int stoneweight, const int pic_width, const int pic_height); // 鼠标投石头
	float getFps(const int fps_count); // 计算帧率FPS
	void renderFun(IMAGE& dest_img, const int pic_width, const int pic_height, const int fps_count); // 画面渲染
	void frameFun(const int pic_width, const int pic_height, const int width, const int height); // 画面逻辑
};