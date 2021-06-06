#pragma once // ����ͷ�ļ��ظ�����
#pragma comment(lib,"winmm.lib") // ���ؾ�̬��

#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

auto const width = 640; // ���ڿ��
auto const height = 720; // ���ڸ߶�
auto const grid_width = 16; // ���ӿ��
auto const grid_height = 16; // ���Ӹ߶�
auto const grid_row = (height / grid_height); // ��������
auto const grid_col = (width / grid_width); // ��������
auto const snake_size = (grid_row * grid_col); // ��������

enum Direction {
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3,
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

struct Point { // ��
	int x; // x����
	int y; // y����
};

struct Snake { // ��
	vector<Point> point; // ��������ж��ٽ�
	int size; // �ߵ�ǰ�ж��ٽ�
	int dir; // ��ͷ�ķ���
};

struct Food { // ʳ��
	struct Point point; // ʳ���x��y����
	int flag; // �����ж�ʳ���Ƿ����
};

//-------̰������Ϸ��ģ��--------//

class Model {
private:
	struct Food food; // ʳ��
	struct Snake snake; // ��ɫ��
	struct Button* beginGame; // ��ʼ��ť
	struct Button* endGame; // ������ť
public:
	explicit Model();
	~Model() = default;
	struct Food& getFood();
	struct Snake& getSnake();
	struct Button*& getButton1();
	struct Button*& getButton2();
	void load(IMAGE& bk); // ������Դ
	void bgm(); // ��������
	void init(); // ��ʼ������
	void welcome(IMAGE& bk); // ���ƿ�ʼ����
	void draw(IMAGE& bk); // ������Ϸ����
	struct Button* creatButton(const int x, const int y, const int width, const int height, const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor); // ������ť
	void drawButton(struct Button* pButton); // ���ư�ť
	int mouseInButton(struct Button* pButton, MOUSEMSG* msg); // �ж�����Ƿ��ڰ�ť��
	int mouseControl(struct Button* pButton1, struct Button* pButton2); // �����Ϣ��Ӧ
	void keyDown(); // ������Ϣ��Ӧ
	void snackMove(); // �ߵ��ƶ�
	void creatFood(); // ����ʳ��
	void eatFood(); // ʳ�ﱻ�Ե�
	int condition1(); // ��������1
	int condition2(); // ��������2
	bool over(); // ���������ж�
};