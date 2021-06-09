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
auto const pic_num = 3; // ͼƬ����
auto const str_num = 5; // �ַ�������
auto const pic_size = 150; // ͼƬ��С

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

struct Target { // ��׹���ַ���
	double x; // ��ͼx����
	double y; // ��ͼy����
	char* str; // �����ַ���
	double dx; // �ַ�����ͼƬ��ƫ������
	double dy; // �ַ�����ͼƬ��ƫ������
	double step; // �ַ����ƶ��ٶ�
	bool flag; // ״̬��ʶ�������ж��ַ����Ƿ����
};

struct Userkey { // �û������ֵ
	int x; // �ַ���x����
	int y; // �ַ���y����
	char str[20]; // �����ַ���
};

//-------������Ϸ��ģ��--------//

class Model {
private:
	int score; // ��ȷ������ַ�����
	int error; // ��©������ַ�����
	int count; // �ܹ����ֵ��ַ�����
	double rate; // �����ַ�����ȷ��
	struct Userkey userkey; // �û������ֵ
	struct Target target[str_num]; // ��׹���ַ���
	struct Button* beginGame; // ��ʼ��ť
	struct Button* endGame; // ������ť
public:
	explicit Model();
	~Model();
	struct Button*& getButton1();
	struct Button*& getButton2();
	void load(IMAGE& kk, IMAGE& w1, IMAGE& w2, vector<IMAGE>& img); // ������Դ
	void bgm(); // ��������
	void init(); // ��ʼ������
	int timer(time_t sec, int id); // ��ʱ��
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // PNGͼƬ͸��ʵ��
	void welcome(IMAGE& w1, IMAGE& w2, vector<IMAGE>& img); // ����ʼ����
	void draw(IMAGE& kk); // ������Ϸ����
	struct Button* creatButton(const int x, const int y, const int width, const int height, const char* text, const int dx, const int dy, COLORREF buttoncolor, COLORREF textcolor); // ������ť
	void drawButton(struct Button* pButton); // ���ư�ť
	void outtextxy_int(int x, int y, const char* format, int num); // ָ��λ���������
	void outtextxy_double(int x, int y, const  char* format, double num); // ָ��λ�����������
	void targetCreat(struct Target words[], int n); // �����ַ���
	void targetMove(); // �ַ�������
	int mystrcmp(const char* s1, const char* s2); // ���ո���ַ����Ƚϴ�С
	void errorStr(); // ����������ַ���
	int mouseInButton(struct Button* pButton, MOUSEMSG* msg); // �ж�����Ƿ��ڰ�ť��
	int mouseControl(struct Button* pButton1, struct Button* pButton2); // �����Ϣ��Ӧ
	void keyDown(); // ������Ϣ��Ӧ
	bool over(); // �����ж�
};