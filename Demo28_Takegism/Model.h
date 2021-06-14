#pragma once // ����ͷ�ļ��ظ�����
#pragma comment(lib,"winmm.lib") // ���ؾ�̬��

#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include <math.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

auto const width = 1280; // ���ڿ��
auto const height = 720; // ���ڸ߶�
auto const grid_size = 20; // ���Ӵ�С
auto const enemy_num = 24; // �о�����
auto const row = (height / grid_size); // ��
auto const col = (width / grid_size); // ��
auto const pi = 3.1415926; // �����

enum Tools { // ����
	BLANK = 0, // �յ�
	WALL = 1, // ǽ��
	DOOR = 2, // �յ�
};

enum Dir { // ����
	NONE = 0, // ��ֹ״̬
	UP = 1, // �Ϸ���
	DOWN = 2, // �·���
	LEFT = 3, // ����
	RIGHT = 4, // �ҷ���
};

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

struct Role { // ���
	double x; // ���x����
	double y; // ���y����
	double step; // ����ƶ��ٶ�
	int dir; // ����ƶ�����
};

struct Enemy { // ����
	double x; // ����x����
	double y; // ����y����
	double step; // �����ƶ��ٶ�
	double tempstep; // ��¼���˳�ʼ�ƶ��ٶ�
	double steprange; // ��¼�ٶȱ仯����
	int flag; // �����ƶ�ת���״̬��ʶ
};

//-------����������Ϸ��ģ��--------//

class Model {
private:
	int top; // ��ͼ���Ͻ�y����
	int left; // ��ͼ���Ͻ�x����
	int down; // ��ͼ���½�y����
	int right; // ��ͼ���½�x����
	vector<vector<int>> gameMapVec; // ��ŵ�ͼ��Ϣ
	struct Role role; // ��ɫ
	vector<Enemy> enemys; // ����
	struct Button* beginGame; // ��ʼ��ť
	struct Button* endGame; // ������ť
public:
	explicit Model();
	~Model();
	struct Button*& getButton1();
	struct Button*& getButton2();
	void load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE enem[], IMAGE play[]); // ������Դ
	void bgm(); // ��������
	void init(); // ��ʼ������
	void initWall(); // ��ʼ��ǽ��
	double distance(const double x1, const double y1, const double x2, const double y2); // ����������
	void welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2); // ���ƿ�ʼ����
	void draw(IMAGE play[], IMAGE enem[]); // ������Ϸ����
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // PNGͼƬ͸��ʵ��
	struct Button* creatButton(const int x, const int y, const int width, const int height, const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor); // ������ť
	void drawButton(struct Button* pButton); // ���ư�ť
	void drawRole(struct Role* role, IMAGE play[]); // ��������
	void drawEnemy(struct Enemy* enemy, IMAGE enem[]); // ���Ƶ���
	void moveRole(); // ��ɫ�ƶ�
	void moveEnemy(); // �����ƶ�
	int mouseInButton(struct Button* pButton, MOUSEMSG* msg); // �ж�����Ƿ��ڰ�ť��
	int mouseControl(struct Button* pButton1, struct Button* pButton2); // �����Ϣ��Ӧ
	void keyDown(); // ������Ϣ��Ӧ
	int condition1(); // ��������1
	int condition2(); // ��������2
	bool over(); // �����ж�
};