#pragma once // ����ͷ�ļ��ظ�����
#pragma comment(lib,"winmm.lib") // ���ؾ�̬��

#include <iostream>
#include <vector>
#include <conio.h>
#include <graphics.h>
#include <mmsystem.h>
#include <initializer_list>

using namespace std;

//-------ƴͼ��Ϸ��ģ��--------//

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
	void load(IMAGE& bk, IMAGE& white, const int width, const int height); // ������Դ
	void bgm(); // ��������
	void init(const int width, const int height, const int grid_num, const int grid_width, const int grid_height); // ��ʼ������
	void welcome(IMAGE& bk); // ���ƿ�ʼ����
	void draw(IMAGE& bk, IMAGE& white, const int grid_num, const int grid_width, const int grid_height); // ������Ϸ����
	int searchArray_i(const int grid_num); // ��λ�׿�λ��i
	int searchArray_j(const int grid_num); // ��λ�׿�λ��j
	void mouseControl(const int grid_num, const int grid_width, const int grid_height); // ������
	bool over(const int grid_num); // ��Ϸ�����ж�
};