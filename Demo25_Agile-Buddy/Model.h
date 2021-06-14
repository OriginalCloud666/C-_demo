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
auto const pic_size = 160; // ͼƬ��С
auto const board_num = 5; // ľ������
auto const row = (height / pic_size + 1); //��
auto const col = (width / pic_size); // ��

enum DIR { // ����
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3
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

struct Board { // ľ��
	double x; // ľ��x����
	double y; // ľ��y����
	int width; // ľ����
	int height; // ľ��߶�
	int type; // ľ������
};

struct Role { // ��ɫ
	double x; // ��ɫx����
	double y; // ��ɫy����
	int width; // ��ɫ���
	int height; // ��ɫ�߶�
	double step; // ��ɫ�ƶ��ٶ�
	int dir; // ��ɫ��������
	int dirnum[4]; // �������к�
	double tempdir[4]; // ����ͼƬ�л��ٶ�
	bool flag; // ״̬��ʶ�������жϽ�ɫ�Ƿ���ľ����
	bool nail; // ״̬��ʶ�������ж̽�ɫ�Ƿ��ڴ��м�̵�ľ����
};

//-------���ߴ�����Ϸ��ģ��--------//

class Model {
private:
	int count; // ���ڼ�¼�ܹ�����
	int score; // ���ڼ�¼��ǰ����
	vector<Board> boards; // ľ��
	struct Role role; // ��ɫ
	struct Button* beginGame; // ��ʼ��ť
	struct Button* endGame; // ������ť
public:
	explicit Model();
	~Model();
	int& getScore();
	struct Button*& getButton1();
	struct Button*& getButton2();
	void load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& stage1, IMAGE stage2[], IMAGE& apron, IMAGE& nail, IMAGE& hp, IMAGE player[]); // ������Դ
	void bgm(); // ��������
	void init(); // ��ʼ������
	bool timer(const int ms, const int id); // ��ʱ��
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // PNGͼƬ͸��ʵ��
	void welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& nail, IMAGE& apron); // ���ƿ�ʼ����
	void draw(IMAGE& bk, IMAGE& nail, IMAGE& apron, IMAGE& stage1, IMAGE stage2[], IMAGE player[]); // ������Ϸ����
	struct Button* creatButton(const int x, const int y, const int width, const int height, const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor); // ������ť
	void drawButton(struct Button* pButton); // ���ư�ť
	void drawBackgound(IMAGE& bk); // ���Ʊ���
	void drawNail(IMAGE& nail); // ���Ƽ��
	void drawApron(IMAGE& apron); // ����ǽ��
	void drawStage02(const int x, const int y, IMAGE stage2[]); // �������ľ��
	void drawBoard(const int i, IMAGE& stage1, IMAGE stage2[]); // ����ľ��
	void drawRole(IMAGE player[]); // ���ƽ�ɫ
	void moveBoard(); // ľ���ƶ�
	void sumScore(); // �������
	void overAction(IMAGE& bk, IMAGE& nail, IMAGE& apron, IMAGE& stage1, IMAGE stage2[], IMAGE player[]); // ��������
	int mouseInButton(struct Button* pButton, MOUSEMSG* msg); // �ж�����Ƿ��ڰ�ť��
	int mouseControl(struct Button* pButton1, struct Button* pButton2); // �����Ϣ��Ӧ
	void keyDown(); // ������Ϣ��Ӧ
	bool over(); // �����ж�
};