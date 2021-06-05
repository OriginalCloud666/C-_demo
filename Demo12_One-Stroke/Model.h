#pragma once // ����ͷ�ļ��ظ�����
#pragma comment(lib,"winmm.lib") // ���ؾ�̬��

#include <iostream>
#include <vector>
#include <conio.h>
#include <graphics.h>
#include <mmsystem.h>

using namespace std;

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
	void init(const int width, const int height, const int grid_num, const int grid_size, const int point_num); // ��ʼ������
	void pointInit(const int grid_num); // λ����Ϣ��ĳ�ʼ��
	void drawAlpha(IMAGE* picture, const int  picture_x, const int picture_y); // ����PNGͼƬ
	void welcome(IMAGE& bk, IMAGE& w1, IMAGE& w2); // ���ƿ�ʼ����
	void draw(const int grid_num, const int grid_size); // ������Ϸ����
	void drawRole(const double x, const double y, const int border_size, const int grid_num); // ��ɫ����
	void drawWall(const double x, const double y, const int grid_size); // ǽ�ڻ���
	void drawLine(struct Point* begin, struct Point* end, const int grid_size); // ���߹���
	void searchRolePos(const int grid_num); // Ѱ�ҽ�ɫλ��
	int roleStop(const int dir); // �ƶ���׼�ж�
	void moveRole(const int dir); // ��ɫ�ƶ�
	void keyDown(); // ������Ϣ��Ӧ
	bool over(const int grid_num); // ���������ж�
};