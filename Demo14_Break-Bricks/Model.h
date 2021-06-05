#pragma once // ����ͷ�ļ��ظ�����
#pragma comment(lib,"winmm.lib") // ���ؾ�̬��

#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>
#include <conio.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

auto const width = 640; // ���ڿ��
auto const height = 720; // ���ڸ߶�
auto const brick_width = 80; // ש����
auto const brick_height = 30; // ש��߶�
auto const brick_row = (height / brick_height - 16); // ש������
auto const brick_col = (width / brick_width); // ש������
auto const pi = 3.1415926; // ��

struct Board { // ľ��
	double x; // ľ��x����
	double y; // ľ��y����
	double step; // ľ���ƶ��ٶ�
	int width; // ľ����
	int height; // ľ��߶�
};

struct Ball { // ��
	double x; // С��x����
	double y; // С��y����
	double dx; // С��x���ٶȷ���
	double dy; // С��y���ٶȷ���
	double step; // С����ƶ��ٶ�
	double radian; // С���ƶ��ķ���Ƕ�
	double r; // С��뾶��С
	int flag = 0; // ״̬��ʶ�������ж�С���Ƿ���
};

//-------��ש����Ϸ��ģ��--------//

class Model {
private:
	int top; // ��ͼ���Ͻ�y����
	int left; // ��ͼ���Ͻ�x����
	int down; // ��ͼ���½�y����
	int right; // ��ͼ���½�x����
	struct Board board; // ľ��
	struct Ball ball; // С��
	vector<vector<int>> gameMapVec; // ��ŵ�ͼ��Ϣ
public:
	explicit Model();
	~Model() = default;
	struct Ball& getBall();
	double distance(const double x1, const double y1, const double x2, const double y2); // ����������
	void load(IMAGE& bk); // ������Դ
	void bgm(); // ��������
	void init(); // ��ʼ������
	void welcome(IMAGE& bk); // ���ƿ�ʼ����
	void draw(); // ������Ϸ����
	void keyDown1(); // ������Ӧ���첽��
	void keyDown2(); // ������Ӧ��ͬ����
	void ballMove(); // С���ƶ���ײ
	void ballCrash(); // С����ש����ײ
	bool over(); // ���������ж�
};