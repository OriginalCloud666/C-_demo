#pragma once // ����ͷ�ļ��ظ�����
#pragma comment(lib,"winmm.lib") // ���ؾ�̬��

#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

auto const width = 640; // ���ڿ��
auto const height = 720; // ���ڸ߶�
auto const pic_width = 640; // ͼƬ���
auto const pic_height = 905; // ͼƬ�߶�
auto const fps_count = 8; // ֡�ʼ�����ʱ��

//-------�ɶ�ˮ����ģ��--------//

class Model {
private:
	DWORD* img_ptr1; // ԭλͼ�ڴ�ָ��
	DWORD* img_ptr2; // �����λͼ�ڴ�ָ��
	short* buf; // ��ǰ����
	short* buf2; // ��һ�̲���
	int flag; // ״̬��ʶ
public:
	explicit Model();
	~Model();
	void load(IMAGE& src_img); // ������Դ
	void bgm(); // ��������
	void init(IMAGE& src_img, IMAGE& dest_img); // ��ʼ������
	void nextFrame(const int damp); // ������һ�̲���
	void renderRipple(); // ����λͼ
	void disturb(const int x, const int y, const int stonesize, const int stoneweight); // ���Ͷʯͷ
	float getFps(); // ����֡��FPS
	void renderFun(IMAGE& dest_img); // ������Ⱦ
	void frameFun(); // �����߼�
};