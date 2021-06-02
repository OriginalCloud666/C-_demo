#pragma once // ����ͷ�ļ��ظ�����
#pragma comment(lib,"winmm.lib") // ���ؾ�̬��

#include <iostream>
#include <vector>
#include <conio.h>
#include <time.h>
#include <graphics.h>
#include <mmsystem.h>

using namespace std;

//-------ɨ����Ϸ��ģ��--------//

class Model {
private:
	int flag; // ״̬��ʶ������Ƿ�ʤ��
	clock_t start; // ��ǳ���ʼʱ��
	clock_t finish; // ��ǳ������ʱ��
	double duration; // ��¼��������ʱ��
	int mine_num; // ��ͼ�����������
	int row; // ��ͼ��������
	int col; // ��ͼ��������
	int top; // ��ͼ���Ͻ�y����
	int left; // ��ͼ���Ͻ�x����
	int down; // ��ͼ���½�y����
	int right; // ��ͼ���½�x����
	vector<vector<int>> gameMapVec; // ��ŵ�ͼ��Ϣ
public:
	explicit Model();
	~Model() = default;
	clock_t& getStart();
	clock_t& getFinish();
	void load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3, IMAGE& mine, vector<IMAGE>& img, vector<int>& imgIndex, const int pic_size, const int pic_num, const int width, const int height); // ������Դ
	void bgm(); // ��������
	void init(const int row, const int col, const int mine_num, const int pic_size, const int width, const int height); // ��ʼ������
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // ����PNG͸��ͼƬ
	void welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3); // ���ƿ�ʼ����
	void draw(IMAGE& bk, IMAGE& mine, vector<IMAGE>& img, const int pic_size); // ������Ϸ����
	void openNull(const int cur_row, const int cur_col); // �򿪿ո���Χ����
	int mouseControl(const int pic_size); // �����Ϣ��Ӧ
	void keyDown(const int map_size1, const int map_size2, const int map_size3, const int mine_num1, const int mine_num2, const int mine_num3, const int pic_size, const int width, const int height); // ������Ϣ��Ӧ
	void over(IMAGE& bk, IMAGE& mine, vector<IMAGE>& img, const int pic_size, int flag, const int width, const int height); // ��Ϸ�����ж�
};