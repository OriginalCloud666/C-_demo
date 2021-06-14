#pragma once // ����ͷ�ļ��ظ�����
#pragma comment(lib,"winmm.lib") // ���ؾ�̬��

#include <iostream>
#include <vector>
#include <conio.h>
#include <string.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

auto const width = 1280; // ���ڿ��
auto const height = 720; // ���ڸ߶�
auto const grid_size = 20; // ���Ӵ�С
auto const row = 24; // ����
auto const col = 40; // ����
auto const maxcas = 3; // �ؿ�����

enum Tools { // ����
	BLANK = 0, // �յ�
	WALL = 1, // ǽ��
	ROLE = 2, // ���
	FIRE = 3, // ����
	DOOR = 4, // �յ�
	GHOST = 5, // ����
};

enum Dir { // ����
	NONE = 0, // ��ֹ״̬
	UP = 1, // �Ϸ���
	DOWN = 2, // �·���
	LEFT = 3, // ����
	RIGHT = 4, // �ҷ���
};

struct Button { // ��ť
	int x; // ��ťx����
	int y; // ��ťy����
	int width; // ��ť���
	int height; // ��ť�߶�
	char* text; // ��ť������
	int dx; // ��ť����������ڰ�ť��ƫ��ֵ
	int dy; // ��ť����������ڰ�ť��ƫ��ֵ
	COLORREF buttoncolor; // ��ť��ɫ
	COLORREF textcolor; // ��ť��������ɫ
};

struct Role { // ���
	double x; // ���x����
	double y; // ���y����
	double step; // ����ƶ��ٶ�
	int dir; // ����ƶ�����
};

//-------�ӳ�������Ϸ��ģ��--------//

class Model {
private:
	int cas;// ���ھ�����Ϸ�ؿ�
	int top; // ��ͼ���Ͻ�y����
	int left; // ��ͼ���Ͻ�x����
	int down; // ��ͼ���½�y����
	int right; // ��ͼ���½�x����
	struct Role role; // ��ɫ
	vector<vector<vector<int>>> gameMapVec; // ��ŵ�ͼ��Ϣ
	struct Button* beginGame; // ��ʼ��ť
	struct Button* endGame; // ������ť
public:
	explicit Model();
	~Model();
	int& getCas();
	struct Button*& getButton1();
	struct Button*& getButton2();
	void load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& man, IMAGE& brick, IMAGE& replace, IMAGE fire[], IMAGE door[], IMAGE ghost[], IMAGE character[]); // ������Դ
	void bgm(); // ��������
	void init(); // ��ʼ������
	void map1(); // ��ʼ����ͼ
	void map2(); // ��ʼ����ͼ
	void map3(); // ��ʼ����ͼ
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // PNGͼƬ͸��ʵ��
	void welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& man); // ���ƿ�ʼ����
	void draw(IMAGE& bk, IMAGE& brick, IMAGE fire[], IMAGE door[], IMAGE ghost[], IMAGE character[]); // ������Ϸ����
	struct Button* creatButton(const int x, const int y, const int width, const int height, const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor); // ������ť
	void drawButton(struct Button* pButton); // ���ư�ť
	void drawFire(const int x, const int y, IMAGE fire[]); // ������ͼƬ
	void drawDoor(const int x, const int y, IMAGE door[]); // ���յ�ͼƬ
	void drawGhost(const int x, const int y, IMAGE ghost[]); // ������ͼƬ
	void drawRole(IMAGE character[]); // ������ͼƬ
	void searchRole(); // ��Ѱ����ͼƬ
	void moveRole(IMAGE& bk, IMAGE& brick, IMAGE& replace, IMAGE fire[], IMAGE door[], IMAGE ghost[], IMAGE character[]); // ��ɫ�ƶ�
	int mouseInButton(struct Button* pButton, MOUSEMSG* msg); // �ж�����Ƿ��ڰ�ť��
	int mouseControl(struct Button* pButton1, struct Button* pButton2); // �����Ϣ��Ӧ
	void keyDown(); // ������Ӧ
	int condition1(); // ��������1
	int condition2(); // ��������2
	bool over(); // ���������ж�
};