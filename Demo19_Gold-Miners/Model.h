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
auto const pic_num = 8; // ͼƬ����
auto const gold1_num = 3; // С��������
auto const gold2_num = 3; // ���������
auto const stone_num = 3; // ʯͷ����
auto const sack_num = 4; // ��������
auto const pi = 3.1415926; // �����

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

struct Role { // ����
	int x; // ����x����
	int y; // ����y����
	int width; // ����ͼƬ���
	int height; // ����ͼƬ�߶�
};

struct Mine { // ��Ʒ
	double x; // ��Ʒx����
	double y; // ��Ʒy����
	int width; // ��Ʒ���
	int height; // ��Ʒ�߶�
	double weight; // ��Ʒ����
	bool flag; // �ж���Ʒ�Ƿ񻹴���
	int value; // ��Ʒ��ֵ
};

struct Hook { // ����
	double start_x; // ������ʼλ��x����
	double start_y; // ������ʼλ��y����
	double len; // ���ӳ���
	double end_x; // ���ӽ���λ��x����
	double end_y; // ���ӽ���λ��y����
	double radian; // ������ˮƽ�满��
	int dir; // �����ƶ���������
	bool rockflag; // ״̬��ʶ�������жϹ����Ƿ��ƶ�
	bool flexflag; // ״̬��ʶ�������жϹ����Ƿ�����
};

//-------�ƽ����Ϸ��ģ��--------//

class Model {
private:
	int aim; // ��¼Ŀ��÷�
	int coin; // ��¼��ҵ÷�
	int cas; // �������ﶯ��
	double tempcas; // �����Ŀ���
	struct Role role; // ��ɫ
	struct Hook hook; // ����
	vector<Mine> gold1; // С����
	vector<Mine> gold2; // �����
	vector<Mine> stone; // ʯͷ
	vector<Mine> sack; // ����
	struct Button* beginGame; // ��ʼ��ť
	struct Button* endGame; // ������ť
public:
	explicit Model();
	~Model();
	struct Button*& getButton1();
	struct Button*& getButton2();
	void load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE rolen[][2], vector<IMAGE>& img, vector<int>& imgIndex); // ������Դ
	void bgm(); // ��������
	void init(); // ��ʼ������
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // PNGͼƬ͸��ʵ��
	void welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE rolen[][2], vector<IMAGE> img); // ���ƿ�ʼ����
	void draw(IMAGE& bk, IMAGE rolen[][2], vector<IMAGE> img); // ������Ϸ����
	struct Button* creatButton(const int x, const int y, const int width, const int height, const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor); // ������ť
	void drawButton(struct Button* pButton); // ���ư�ť
	void drawHook(); // ���ƹ���
	void drawRole(IMAGE rolen[][2]); // ���ƽ�ɫ
	void drawGold1(vector<IMAGE> img); // ���ƽ���
	void drawGold2(vector<IMAGE> img); // ���ƽ���
	void drawStone(vector<IMAGE> img); // ����ʯͷ
	void drawSack(vector<IMAGE> img); // ���ƴ���
	void rockHook(); // �����ƶ�
	void grabHook(); // ����ץȡ
	int mouseInButton(struct Button* pButton, MOUSEMSG* msg); // �ж�����Ƿ��ڰ�ť��
	int mouseControl(struct Button* pButton1, struct Button* pButton2); // �����Ϣ��Ӧ
	void keyDown(); // ������Ϣ��Ӧ
	bool over(); // ���������ж�
};