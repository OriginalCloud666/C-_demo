#pragma once // ����ͷ�ļ��ظ�����
#pragma comment(lib,"winmm.lib") // ���ؾ�̬��

#include <iostream>
#include <vector>
#include <conio.h>
#include <time.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

auto const width = 640; // ���ڿ��
auto const height = 720; // ���ڸ߶�
auto const grid_num = 4; // ���и�������
auto const grid_length = 60; // ���ӱ߳�
auto const photo_num = 12; // ͼƬ����

//-------2048��Ϸ��ģ��--------//

class Model {
private:
	int score; // ��¼�÷�
	int top; // ��ͼ���Ͻ�y����
	int left; // ��ͼ���Ͻ�x����
	int down; // ��ͼ���½�y����
	int right; // ��ͼ���½�x����
	vector<vector<int>> gameMapVec; // ��ŵ�ͼ��Ϣ
public:
	explicit Model();
	~Model() = default;
	int& getScore(); // ��ȡscore
	void load(IMAGE& bk, vector<IMAGE>& img, vector<int>& imgIndex); // ������Դ
	void bgm(); // ��������
	void init(); // ��ʼ������
	void welcome(IMAGE& bk); // ���ƿ�ʼ����
	void draw(IMAGE& bk, vector<IMAGE>& img, vector<int>& imgIndex); // ������Ϸ����
	bool condition1(); // ��������1
	bool condition2(); // ��������2
	bool over(); // ���������ж�
	int randIntNum(); // �������������
	int moveLeft(); // �����ƶ�
	int moveRight(); // �����ƶ�
	int moveUp(); // �����ƶ�
	int moveDown(); // �����ƶ�
	void keyDown(); // ������Ϣ��Ӧ
};