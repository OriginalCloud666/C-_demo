#pragma once; // ����ͷ�ļ��ظ�����
#pragma comment(lib,"winmm.lib") // ���ؾ�̬��

#include <iostream>
#include <vector>
#include <conio.h>
#include <graphics.h>
#include <mmsystem.h>

using namespace std;

//-------Ϳ������Ϸ��ģ��--------//

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
	void load(IMAGE& bk); // ������Դ
	void bgm(); // ��������
	void init(int grid_num, int grid_length, int width, int height); // ��ʼ������
	void welcome(IMAGE& bk, int width, int height); // ���ƿ�ʼ����
	void draw(IMAGE& bk, int grid_num, int grid_length, int width, int height); // ������Ϸ����
	void mouseControl(int grid_num, int grid_length, int width, int height); // �����Ϣ��Ӧ
	int over(int grid_num); // ���������ж�
};