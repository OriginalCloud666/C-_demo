#pragma once // ����ͷ�ļ��ظ�����
#pragma comment(lib,"winmm.lib") // ���ؾ�̬��

#include <iostream>
#include <vector>
#include <conio.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

auto const width = 640; // ���ڿ��
auto const height = 720; // ���ڸ߶�
auto const grid_num = 4; // ���и�������
auto const grid_width = 160; // ���ӿ��
auto const grid_height = 180; // ���Ӹ߶�

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
	void load(IMAGE& bk, IMAGE& white); // ������Դ
	void bgm(); // ��������
	void init(); // ��ʼ������
	void welcome(IMAGE& bk); // ���ƿ�ʼ����
	void draw(IMAGE& bk, IMAGE& white); // ������Ϸ����
	int searchArray_i(); // ��λ�׿�λ��i
	int searchArray_j(); // ��λ�׿�λ��j
	void mouseControl(); // ������
	bool over(); // ��Ϸ�����ж�
};