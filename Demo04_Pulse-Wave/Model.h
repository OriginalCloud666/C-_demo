#pragma once // ����ͷ�ļ��ظ�����
#pragma comment(lib,"winmm.lib") // ���ؾ�̬��

#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <graphics.h>
#include <mmsystem.h>

using namespace std;

//-------�ɶ�ˮ����ģ��--------//

class Model {
private:
	DWORD* img_ptr1; // ԭλͼ�ڴ�ָ��
	DWORD* img_ptr2; // �����λͼ�ڴ�ָ��
	short* buf; // ��ǰ����
	short* buf2; // ��һ�̲���
	int flag; // ״̬��ʶ
public:
	explicit Model() = delete;
	explicit Model(const int pic_width, const int pic_height, const int width);
	~Model();
	void load(IMAGE& src_img, const int pic_width, const int pic_height); // ������Դ
	void bgm(); // ��������
	void init(IMAGE& src_img, IMAGE& dest_img, const int pic_width, const int pic_height); // ��ʼ������
	void nextFrame(const int damp, const int pic_width, const int pic_height, const int width); // ������һ�̲���
	void renderRipple(const int pic_width, const int pic_height); // ����λͼ
	void disturb(const int x, const int y, const int stonesize, const int stoneweight, const int pic_width, const int pic_height); // ���Ͷʯͷ
	float getFps(const int fps_count); // ����֡��FPS
	void renderFun(IMAGE& dest_img, const int pic_width, const int pic_height, const int fps_count); // ������Ⱦ
	void frameFun(const int pic_width, const int pic_height, const int width, const int height); // �����߼�
};