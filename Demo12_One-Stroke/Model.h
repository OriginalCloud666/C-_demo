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
auto const grid_size = 50; // ���Ӵ�С
auto const grid_num = 10; // ��ͼ����
auto const point_num = 100; // ��¼λ�õ������

enum Tools {
	BLANK = 0, // �յ���0��ʾ
	WALL = 1, // ǽ����1��ʾ
	ROLE = 2, // ������2��ʾ
	BOARD = 3, // ���߹��ĵط���3��ʾ
};

enum Direction {
	LEFT = 0, // ����
	RIGHT = 1, // �ҷ���
	UP = 2, // �Ϸ���
	DOWN = 3, // �·���
};

struct Role { // ��ɫ
	int row; // ��ɫ������
	int col; // ��ɫ������
	int flag; // �ƶ����
};

struct Point { // �����ڼ�¼��ʼ����ֹλ��
	int row; // ��ǰ��������
	int col; // ��ǰ��������
};

//-------һ�ʻ���Ϸ��ģ��--------//

class Model {
private:
	int flag; // ״̬��ʶ���жϹؿ�
	int count; // ������
	int top; // ��ͼ���Ͻ�y����
	int left; // ��ͼ���Ͻ�x����
	int down; // ��ͼ���½�y����
	int right; // ��ͼ���½�x����
	struct Role role; // ��ɫ����
	vector<Point> point; // ����λ��
	vector<vector<vector<int>>> gameMapVec; // ��ŵ�ͼ���ؿ���Ϣ
public:
	explicit Model();
	~Model() = default;
	int& getCount(); // ��ȡcount
	int& getFlag(); // ��ȡflag
	struct Role& getRole(); // ��ȡrole
	vector<Point>& getPoint(); // ��ȡpoint
	void load(IMAGE& bk, IMAGE& w1, IMAGE& w2); // ������Դ
	void bgm(); // ��������
	void init(); // ��ʼ������
	void pointInit(); // λ����Ϣ��ĳ�ʼ��
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // ����PNGͼƬ
	void welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2); // ���ƿ�ʼ����
	void draw(); // ������Ϸ����
	void drawRole(const double x, const double y, const int border_size); // ��ɫ����
	void drawWall(const double x, const double y); // ǽ�ڻ���
	void drawLine(struct Point* begin, struct Point* end); // ���߹���
	void searchRolePos(); // Ѱ�ҽ�ɫλ��
	int roleStop(const int dir); // �ƶ���׼�ж�
	void moveRole(const int dir); // ��ɫ�ƶ�
	void keyDown(); // ������Ϣ��Ӧ
	bool over(); // ���������ж�
};