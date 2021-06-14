#pragma once // ����ͷ�ļ��ظ�����
#pragma comment(lib,"winmm.lib") // ���ؾ�̬��

#include <iostream>
#include <vector>
#include <stack>
#include <conio.h>
#include <string.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

auto const width = 640; // ���ڿ��
auto const height = 720; // ���ڸ߶�
auto const grid_width = 35; // ���ӿ��
auto const grid_height = 35; // ���Ӹ߶�
auto const row = 12; // ����
auto const col = 12; // ����

enum Dir { // ����
	UP = 0,
	RIGHT = 1,
	DOWN = 2,
	LEFT = 3,
};

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

struct Point { // ��
	int x;
	int y;
};

struct PathNode { // ��¼��̽��
	int dir; // ��¼��ǰ�����̽����
	bool isFind; // ��¼��ǰ��̽���Ƿ��߹���0δ�߹���1�߹�
};

//-------���ӻ����Ѱ·��ģ��--------//

class Model {
private:
	int top; // ��ͼ���Ͻ�y����
	int left; // ��ͼ���Ͻ�x����
	int down; // ��ͼ���½�y����
	int right; // ��ͼ���½�x����
	vector<vector<int>> gameMapVec; // ��ŵ�ͼ��Ϣ
	vector<vector<PathNode>> gameNodeVec; // �����̽����Ϣ
	struct Point beginPoint; // ���
	struct Point endPoint; // �յ�
	stack<Point> mystack; // ջ
	struct Button* beginGame; // ��ʼ��ť
	struct Button* endGame; // ������ť
public:
	explicit Model();
	~Model();
	struct Button*& getButton1();
	struct Button*& getButton2();
	void load(IMAGE& w1, IMAGE& ren, IMAGE& road, IMAGE& wall); // ������Դ
	void bgm(); // ��������
	void init(); // ��ʼ������
	void welcome(IMAGE& w1); // ����ʼ����
	void draw(IMAGE& road, IMAGE& wall, IMAGE& ren); // ������Ϸ����
	struct Button* creatButton(const int x, const int y, const int width, const int height, const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor); // ������ť
	void drawButton(struct Button* pButton); // ���ư�ť
	void depthFinding(IMAGE& road, IMAGE& wall, IMAGE& ren); // ���Ѱ·�㷨
	int mouseInButton(struct Button* pButton, MOUSEMSG* msg); // �ж�����Ƿ��ڰ�ť��
	int mouseControl(struct Button* pButton1, struct Button* pButton2); // �����Ϣ��Ӧ
};