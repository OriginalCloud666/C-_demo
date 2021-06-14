#pragma once // ����ͷ�ļ��ظ�����
#pragma comment(lib,"winmm.lib") // ���ؾ�̬��

#include <iostream>
#include <vector>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

auto const width = 640; // ���ڿ��
auto const height = 720; // ���ڸ߶�
auto const pic_num = 4; // ͼƬ����
auto const needle_num = 36; // �������
auto const pi = 3.1415926; // �����

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

struct Needle { // ��
	double x; // ��ĩ�˵�x����
	double y; // ��ĩ�˵�y����
	double len; // ��ĳ���
	double rotateSpeed; // ������ת�����ת�ٶ�
	double angle; // ������ת�������ת�Ƕ�
	int flag; // ״̬��ʶ�������ж���Ϊ�ĸ���ҷ���
};

struct Ball { // ��
	double x; // ���x����
	double y; // ���y����
	double r; // ��İ뾶
};

struct Role { // ���
	double x; // ���x����
	double y; // ���y����
	struct Ball ball; // ���β���������
	double step; // ���β�����������ٶ�
	int score; // ��ҵĵ÷�
	int count; // ��������������
};

//-------���������Ϸ��ģ��--------//

class Model {
private:
	int needleNum; // ��ת���������
	double finaltime; // ���ڼ�¼����ʱʱ��
	struct Role role1; // ���һ
	struct Role role2; // ��Ҷ�
	struct Ball ball; // �в�����
	vector<Needle> needleVec; // ���������
	struct Button* beginGame; // ��ʼ��ť
	struct Button* endGame; // ������ť
public:
	explicit Model();
	~Model();
	int& getNeedleNum();
	double& getFinaltime();
	struct Role& getRole1();
	struct Role& getRole2();
	struct Button*& getButton1();
	struct Button*& getButton2();
	void load(IMAGE& bk, IMAGE& w1, IMAGE& w2, vector<IMAGE>& img, vector<int>& imgIndex); // ������Դ
	void bgm(); // ��������
	void init(); // ��ʼ������
	int timer(time_t sec, int id); // ��ʱ��
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // PNGͼƬ͸��ʵ��
	void welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2); // ���ƿ�ʼ����
	void draw(IMAGE& bk, vector<IMAGE>& img); // ������Ϸ����
	struct Button* creatButton(const int x, const int y, const int width, const int height, const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor); // ������ť
	void drawButton(struct Button* pButton); // ���ư�ť
	void drawRole1(vector<IMAGE>& img); // �������һ
	void drawRole2(vector<IMAGE>& img); // ������Ҷ�
	void rotateNeedle(); // �����ת
	int mouseInButton(struct Button* pButton, MOUSEMSG* msg); // �ж�����Ƿ��ڰ�ť��
	int mouseControl(struct Button* pButton1, struct Button* pButton2); // �����Ϣ��Ӧ
	void keyDown(IMAGE& bk, vector<IMAGE>& img); // ������Ϣ��Ӧ
	bool over(); // �����ж�
};