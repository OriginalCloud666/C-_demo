#pragma once // ����ͷ�ļ��ظ�����
#pragma comment(lib,"winmm.lib") // ���ؾ�̬��

#include <iostream>
#include <vector>
#include <conio.h>
#include <graphics.h>
#include <mmsystem.h>

using namespace std;

//-------��������Ϸ��ģ��--------//

class Model {
private:
	int flag; // ״̬��ʶ���жϹؿ�
	int top; // ��ͼ���Ͻ�y����
	int left; // ��ͼ���Ͻ�x����
	int down; // ��ͼ���½�y����
	int right; // ��ͼ���½�x����
	vector<vector<vector<int>>> gameMapVec; // ��ŵ�ͼ���ؿ���Ϣ
public:
	explicit Model();
	~Model() = default;
	int& getFlag();
	void load(IMAGE& bk, vector<IMAGE>& img, vector<int>& imgIndex, const int pic_num, const int pic_size, const int width, const int height); // ������Դ
	void bgm(); // ��������
	void init(const int width, const int height, const int grid_num, const int pic_size, const int cas); // ��ʼ������
	void welcome(IMAGE& bk); // ���ƿ�ʼ����
	void draw(IMAGE& bk, vector<IMAGE>& img, const int grid_num, const int pic_size); // ������Ϸ����
	void changOne(const int i, const int j); // �������λ��
	void keyDown(const int grid_num); // ������Ӧ
	int over(const int grid_num); // ��Ϸ�����ж�
	void action(IMAGE& bk, vector<IMAGE>& img, const int grid_num, const int pic_size, const int width, const int height); // ��������
};