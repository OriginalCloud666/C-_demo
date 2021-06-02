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
	void load(IMAGE& bk, vector<IMAGE>& img, vector<int>& imgIndex, const int grid_length, const int width, const int height, const int photo_num); // ������Դ
	void bgm(); // ��������
	void init(const int grid_num, const int grid_length, const int width, const int height); // ��ʼ������
	void welcome(IMAGE& bk); // ���ƿ�ʼ����
	void draw(IMAGE& bk, vector<IMAGE>& img, vector<int>& imgIndex, const int grid_num, const int grid_length, const int photo_num); // ������Ϸ����
	bool condition1(const int grid_num); // ��������1
	bool condition2(const int grid_num); // ��������2
	bool over(const int grid_num); // ���������ж�
	int randIntNum(const int grid_num); // �������������
	int moveLeft(const int grid_num); // �����ƶ�
	int moveRight(const int grid_num); // �����ƶ�
	int moveUp(const int grid_num); // �����ƶ�
	int moveDown(const int grid_num); // �����ƶ�
	void keyDown(const int grid_num); // ������Ϣ��Ӧ
};