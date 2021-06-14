#pragma once // ����ͷ�ļ��ظ�����
#pragma comment(lib,"winmm.lib") // ���ؾ�̬��

#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

auto const width = 640; // ���ڿ��
auto const height = 720; // ���ڸ߶�
auto const pic_size = 80; // ͼƬ��С
auto const pic_num = 4; // ͼƬ����
auto const line_num = 7; // �߶�����
auto const car_num = 3; // ��������
auto const row = (height / pic_size); // ��
auto const col = (width / pic_size); // ��

struct Button { // ��ť
	int x; // ���Ͻ�x����
	int y; // ���Ͻ�y����
	int width; // ��ť���
	int height; // ��ť�߶�
	char* text; // ��ť������
	int dx; // ��������ڰ�ť���Ͻ�x�����ƫ��ֵ
	int dy; // ��������ڰ�ť���Ͻ�y�����ƫ��ֵ
	COLORREF buttoncolor; // ��ť��ɫ
	COLORREF textcolor; // ��ť��������ɫ
};

struct Role { // ��ɫ
	double x; // ��ɫx����
	double y; // ��ɫy����
	double step; // ��ɫÿ��λ��
};

struct Car { // ����
	double x; // ���ӵ�x����
	double y; // ���ӵ�y����
	double step; // ����ÿ�ε�λ��
	int kind; // ���ӵ�����
	int road; // �������ڵ�·
};

struct Line { // �߶�
	double FL_x; // ����߶�x����
	double FL_y; // ����߶�y����
	double SL_x; // �ұ��߶�x����
	double SL_y; // �ұ��߶�y����
	double step; // �߶�ÿ��λ����
	int width; // �߶εĿ��
	int height; // �߶εĸ߶�
};

//-------����������Ϸ��ģ��--------//

class Model {
private:
	vector<Car> cars; // ��
	vector<Line> lines; // ��
	struct Role role; // ��ɫ
	struct Button* beginGame; // ��ʼ��ť
	struct Button* endGame; // ������ť
public:
	explicit Model();
	~Model();
	struct Button*& getButton1();
	struct Button*& getButton2();
	void load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& track, vector<IMAGE>& img, vector<int>& imgIndex); // ������Դ
	void bgm(); // ��������
	void init(); // ��ʼ������
	double distance(const double x1, const double y1, const double x2, const double y2); // ����������
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // PNGͼƬ͸��ʵ��
	void welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& track, vector<IMAGE>& img); // ���ƿ�ʼ����
	void draw(IMAGE& bk, IMAGE& track, vector<IMAGE>& img); // ������Ϸ����
	struct Button* creatButton(const int x, const int y, const int width, const int height, const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor); // ������ť
	void drawLayout(IMAGE& bk, IMAGE& track); // ��������
	void drawButton(struct Button* pButton); // ���ư�ť
	void drawLine(); // ��������
	void moveLine(); // �����ƶ�
	void moveCar(); // �����ƶ�
	int mouseInButton(struct Button* pButton, MOUSEMSG* msg); // �ж�����Ƿ��ڰ�ť��
	int mouseControl(struct Button* pButton1, struct Button* pButton2); // �����Ϣ��Ӧ
	void keyDown(); // ������Ϣ��Ӧ
	bool over(); // ���������ж�
};