#pragma once // ����ͷ�ļ��ظ�����
#pragma comment(lib,"winmm.lib") // ���ؾ�̬��

#include "Plane.h"

#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

auto const width = 640; // ���ڿ��
auto const height = 720; // ���ڸ߶�
auto const enemy1_num = 1; // ���͵л�����
auto const enemy2_num = 8; // С�͵л�����
auto const bullet_step = 1; // �ӵ�����ٶ�

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

//-------�ɻ���ս��Ϸ��ģ��--------//

class Model {
private:
	int score; // ���ڼ�¼���յ÷�
	int cas; // ���ڿ��Ʒɻ���ը����
	Plane role; // �Ҿ��ɻ�
	vector<Plane> enemy1; // �о����ͷɻ�
	vector<Plane> enemy2; // �о�С�ͷɻ�
	struct Button* beginGame; // ��ʼ��ť
	struct Button* endGame; // ������ť
public:
	explicit Model();
	~Model();
	int& getScore();
	Plane& getRole();
	struct Button*& getButton1();
	struct Button*& getButton2();
	vector<Plane>& getEnemy1();
	vector<Plane>& getEnemy2();
	void load(IMAGE& bk, IMAGE& begin, IMAGE& author, IMAGE plane[][2], IMAGE enemy_big[], IMAGE enemy_small[], IMAGE bullet[]); // ������Դ
	void bgm(); // ��������
	void init(); // ��ʼ������
	bool timer(const int ms, const int id); // ��ʱ��
	void welcome(IMAGE& bk, IMAGE& begin, IMAGE& author); // ���ƿ�ʼ����
	void draw(IMAGE& bk, IMAGE enemy_big[], IMAGE enemy_small[], IMAGE plane[][2], IMAGE bullet[]); // ������Ϸ����
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // PNG͸��ͼƬ
	double distance(const double x1, const double y1, const double x2, const double y2); // ����������
	struct Button* creatButton(const int x, const int y, const int width, const int height, const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor); // ������ť
	void drawButton(struct Button* pButton); // ���ư�ť
	void moveEnemy(); // �л��ƶ�
	void hitEnemy(vector<Plane>& enemy, const int enemy_num, const int hp); // �л���ײ��ʧ
	int mouseInButton(struct Button* pButton, MOUSEMSG* msg); // �ж�����Ƿ��ڰ�ť��
	int mouseControl(struct Button* pButton1, struct Button* pButton2); // �����Ϣ��Ӧ
	void keyDown1(); // ������Ϣ��Ӧ���첽��
	void keyDown2(); // ������Ϣ��Ӧ��ͬ����
	bool condition(vector<Plane>& enemy); // ��Ϸ��������
	bool over(); // �����ж�
};