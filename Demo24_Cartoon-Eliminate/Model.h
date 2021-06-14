#pragma once // ����ͷ�ļ��ظ�����
#pragma comment(lib,"winmm.lib") // ���ؾ�̬��

#include <iostream>
#include <vector>
#include <conio.h>
#include <time.h>
#include <string.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

auto const width = 640; // ���ڿ��
auto const height = 720; // ���ڸ߶�
auto const pic_num = 9; // ͼƬ����
auto const pic_size = 60; // ͼƬ��С
auto const grid_num = 10; // ��ͼ����

struct Button { // ��ť
	int x; // ��ť���Ͻ�x����
	int y; // ��ť���Ͻ�y����
	int width; // ��ť���
	int height; // ��ť�߶�
	char* text; // ��ť������
	int dx; // ��������ڰ�ť���Ͻ�x�����ƫ��ֵ
	int dy; // ��������ڰ�ť���Ͻ�y�����ƫ��ֵ
	COLORREF buttoncolor; // ��ť��ɫ
	COLORREF textcolor; // ��ť��������ɫ
};

//-------��ͨ������Ϸ��ģ��--------//

class Model {
private:
	int top; // ��ͼ���Ͻ�y����
	int left; // ��ͼ���Ͻ�x����
	int down; // ��ͼ���½�y����
	int right; // ��ͼ���½�x����
	vector<vector<int>> gameMapVec; // ��ŵ�ͼ���ؿ���Ϣ
	POINT begin; // ��ʼ��������±�
	POINT end; // ������������±�
	bool state; // ״̬��ʶ����¼��ʼ�ͽ�����
	struct Button* beginGame; // ��ʼ��ť
	struct Button* endGame; // ������ť
public:
	explicit Model();
	~Model();
	POINT& getBegin();
	POINT& getEnd();
	vector<vector<int>>& getMapVec();
	struct Button*& getButton1();
	struct Button*& getButton2();
	void load(IMAGE& bk, IMAGE& w1, IMAGE& w2, vector<IMAGE>& img, vector<int>& imgIndex); // ������Դ
	void bgm(); // ��������
	void init(); // ��ʼ������
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // PNGͼƬ͸��ʵ��
	void welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2); // ����ʼ����
	void draw(IMAGE& bk, vector<IMAGE>& img); // ������Ϸ����
	struct Button* creatButton(const int x, const int y, const int width, const int height, const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor); // ������ť
	void drawButton(struct Button* pButton); // ���ư�ť
	void drawFim(const int x, const int y); //���Ʊ߿�
	void drawChoseFim(); // ����ѡ���
	bool isBlock(const int r, const int c); // �ж��Ƿ�հ�
	bool isSeem(); // �ж��Ƿ���ͬ
	bool horizon(POINT begin_, POINT end_); // ˮƽ�����ж�
	bool vertical(POINT begin_, POINT end_); // ��ֱ�����ж�
	bool turn_once(POINT begin_, POINT end_); // һ���յ��ж�
	bool turn_twice(POINT begin_, POINT end_); // �����յ��ж�
	bool reMove(); // ���������ж�
	int mouseInButton(struct Button* pButton, MOUSEMSG* msg); // �ж�����Ƿ��ڰ�ť��
	int mouseControl(struct Button* pButton1, struct Button* pButton2); // �����Ϣ��Ӧ
	void mouseControl2(); // �����Ϣ��Ӧ
	bool over(); // �����ж�
};