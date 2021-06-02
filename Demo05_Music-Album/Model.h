#pragma once // ����ͷ�ļ��ظ�����
#pragma comment(lib,"winmm.lib") // ���ؾ�̬��

#include <iostream>
#include <vector>
#include <conio.h>
#include <math.h>
#include <graphics.h>
#include <mmsystem.h>

using namespace std;

//-------���������ģ��--------//

class Model {
private:
	int count; // ����������¼ѭ������
public:
	explicit Model();
	~Model() = default;
	int& getCount(); // ��ȡcount
	void load(IMAGE& bk1, IMAGE& bk2, IMAGE& w1, vector<IMAGE>& img, vector<int>& imgIndex, vector<int>& pic_width, vector<int>& pic_height, const int width, const int height, const int pic_num); // ������Դ
	void bgm(); // ��������
	void drawAlphabk(IMAGE* picture, const int  picture_x, const int picture_y, const int width); // ����PNG͸��ͼƬ
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // ����PNG͸��ͼƬ
	void welcome(IMAGE& bk1, IMAGE& w1); // ���ƿ�ʼ����
	void drawBorder(const int x0, const int y0, const int size, COLORREF C, const int borderwidth); // �������α߿�
	void drawText(const double i, const int x0, const int y0, const int size, const char* s); // ���Ʊ߿��ֶ�
	void drawHeart(IMAGE& bk2, vector<IMAGE>& img, vector<int>& pic_x, vector<int>& pic_y, const int pic_num, const int width, const int x, const int y, const int size, COLORREF C, const char s[][5], const double move, const double interval, const int time); // ��������ͼ��
};