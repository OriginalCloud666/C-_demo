#pragma once // ����ͷ�ļ��ظ�����
#pragma comment(lib,"winmm.lib") // ���ؾ�̬��

#include <iostream>
#include <vector>
#include <conio.h>
#include <time.h>
#include <graphics.h>
#include <mmsystem.h>

using namespace std;

//-------2048��Ϸ��ģ��--------//

class Model {
private:
	int top; // ��ͼ���Ͻ�y����
	int left; // ��ͼ���Ͻ�x����
	int down; // ��ͼ���½�y����
	int right; // ��ͼ���½�x����
	vector<vector<int>> gameMapVec; // ��ŵ�ͼ��Ϣ
public:
	explicit Model();
	~Model() = default;
	void load(IMAGE& bk, IMAGE img[], int imgIndex[], const int grid_length, const int width, const int height); // ������Դ
	void bgm(); // ��������
	void init(const int grid_num, const int grid_length, const int width, const int height); // ��ʼ������
	void welcome(IMAGE& bk); // ���ƿ�ʼ����
	void draw(IMAGE& bk, IMAGE img[], int imgIndex[], const int grid_num, const int grid_length, const int score); // ������Ϸ����
	bool condition1(const int grid_num); // ��������1
	bool condition2(const int grid_num); // ��������2
	bool over(const int grid_num); // ���������ж�
	int randIntNum(const int grid_num); // �������������
	int moveLeft(int& score, const int grid_num); // �����ƶ�
	int moveRight(int& score, const int grid_num); // �����ƶ�
	int moveUp(int& score, const int grid_num); // �����ƶ�
	int moveDown(int& score, const int grid_num); // �����ƶ�
	void keyDown(int& score, const int grid_num); // ������Ϣ��Ӧ
};
