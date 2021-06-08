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
auto const grid_num = 8; // ��ͼ����
auto const pic_num = 6; // ͼƬ����
auto const pic_size = 50; // ͼƬ��С

struct Button {//��ť
	int x;//���Ͻ�x����
	int y;//���Ͻ�y����
	int width;//��ť���
	int height;//��ť�߶�
	COLORREF color1;//�ڲ�߿���ɫ
	COLORREF color2;//���߿���ɫ
};

//-------��ש����Ϸ��ģ��--------//

class Model {
private:
	int flag; // ״̬��ʶ���жϹؿ�
	int top; // ��ͼ���Ͻ�y����
	int left; // ��ͼ���Ͻ�x����
	int down; // ��ͼ���½�y����
	int right; // ��ͼ���½�x����
	struct Button* beginGame; // ��ʼ��ť
	vector<vector<vector<int>>> gameMapVec; // ��ŵ�ͼ���ؿ���Ϣ
public:
	explicit Model();
	~Model();
	int& getFlag();
	struct Button*& getBeginGame();
	void load(IMAGE& bk, IMAGE& button, vector<IMAGE>& img, vector<int>& imgIndex); // ������Դ
	void bgm(); // ��������
	void init(); // ��ʼ������
	void welcome(IMAGE& bk); // ���ƿ�ʼ����
	void draw(IMAGE& bk, IMAGE& button, vector<IMAGE>& img); // ������Ϸ����
	struct Button* creatButton(int x, int y, int width, int height, COLORREF color1, COLORREF color2); // ������ť
	void drawButton(struct Button* pButton, IMAGE& button); // ���ư�ť
	int search_i(); // �ҵ����������
	int search_j(); // �ҵ�����������
	void keyDown(); // ������Ӧ��ͬ����
	int mouseInButton(struct Button* pButton, MOUSEMSG* msg); // �ж�����Ƿ��ڰ�ť��
	void mouseControl(struct Button* pButton); // �����Ϣ��Ӧ
	bool over(); // ���������ж�
};