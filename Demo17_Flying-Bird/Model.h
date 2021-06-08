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
auto const bird_size = 60; // С���С
auto const pillar_num = 3; // ��������
auto const move_step = 0.7; // ���Ӽ������ƶ��ٶ�

struct Bird { // С��
	double x; // ���������Ͻ�x����
	double y; // ���������Ͻ�y����
	double speed; // ��׹�ٶ�
};

struct Pillar { // ����
	double x; // ���������Ͻ�x����
	int height; // ������ʾ�ĸ߶�
	int flag = 0; // ״̬��ʶ���ж������Ƿ���ʧ
};

struct BkDown { // ����ͼ�²��ֵ���
	double x; // ������ͼ���Ͻ�x����
	int flag = 0; // ״̬��ʶ���жϵ����Ƿ���ʧ
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

//-------�����С����Ϸ��ģ��--------//

class Model {
private:
	int score; // ��¼����
	int cas; // С�񶯻�����
	double tempcas; // С�񶯻�����
	struct Bird bird; // С��
	vector<Pillar> pillar; // ����
	vector<BkDown> bkdown; // ����������
	struct Button* beginGame; // ��ʼ��ť
	struct Button* endGame; // ������ť
public:
	explicit Model();
	~Model();
	int& getScore();
	struct Button*& getButton1();
	struct Button*& getButton2();
	void load(IMAGE& bk, IMAGE mybird[][2], IMAGE guid[], IMAGE up[], IMAGE down[], IMAGE start[], IMAGE end[]); // ������Դ
	void bgm(); // ��������
	void init(); // ��ʼ������
	void welcome(IMAGE& bk, IMAGE guid[]); // ���ƿ�ʼ����
	void draw(IMAGE& bk, IMAGE up[], IMAGE down[], IMAGE mybird[][2]); // ������Ϸ����
	struct Button* creatButton(const int x, const int y, const int width, const int height, const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor); // ������ť
	void drawButton(struct Button* pButton); // ���ư�ť
	void drawGuid(IMAGE guid[], const int x, const int y); // ��������ͼƬ
	void drawBird(IMAGE mybird[][2], const int x, const int y); // ����С��ͼƬ
	void drawPillar(IMAGE up[], IMAGE down[], struct Pillar mypillar); // ��������ͼƬ
	void drawBackground(IMAGE& bk, struct BkDown mybkdown); // ���Ƶ���ͼƬ
	void createPillar(const int i); // ��������
	void createBkDown(const int i); // ��������
	void moveBird(); // ����ƶ�
	void movePillar(); // �����ƶ�
	void moveBkDown(); // �����ƶ�
	int hitPillar(); // ������ײ
	int mouseControl(struct Button* pButton1, struct Button* pButton2); // �����Ϣ��Ӧ
	int mouseInButton(struct Button* pButton, MOUSEMSG* msg); // �ж�����Ƿ��ڰ�ť��
	void keyDown(); // ������Ӧ
	int condition1(); // ��������1
	int condition2(); // ��������2
	bool over(); // �����ж�
	void beginAction(IMAGE& bk, IMAGE guid[], IMAGE start[]); // ��ʼ����
	void flyAction(); // ���趯��
	void hitAction(IMAGE& bk, IMAGE up[], IMAGE down[], IMAGE mybird[][2]); // ײ������
	void endAction(IMAGE& bk, IMAGE up[], IMAGE down[], IMAGE mybird[][2], IMAGE end[]); // ��������
};