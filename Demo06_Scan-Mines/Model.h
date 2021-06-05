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
auto const pic_size = 32; // ͼƬ��С
auto const pic_num = 12; // ͼƬ����

auto const map_size1 = 9; // ��ͼ��С��С��
auto const map_size2 = 13; // ��ͼ��С���У�
auto const map_size3 = 17; // ��ͼ��С����
auto const mine_num1 = 10; // �����������٣�
auto const mine_num2 = 26; // �����������У�
auto const mine_num3 = 47; // �����������ࣩ

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
	void load(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3, IMAGE& mine, vector<IMAGE>& img, vector<int>& imgIndex); // ������Դ
	void bgm(); // ��������
	void init(const int row, const int col, const int mine_num); // ��ʼ������
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // ����PNG͸��ͼƬ
	void welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2, IMAGE& w3); // ���ƿ�ʼ����
	void draw(IMAGE& bk, IMAGE& mine, vector<IMAGE>& img); // ������Ϸ����
	void openNull(const int cur_row, const int cur_col); // �򿪿ո���Χ����
	int mouseControl(); // �����Ϣ��Ӧ
	void keyDown(); // ������Ϣ��Ӧ
	void over(IMAGE& bk, IMAGE& mine, vector<IMAGE>& img, const int judgeflag); // ��Ϸ�����ж�
};