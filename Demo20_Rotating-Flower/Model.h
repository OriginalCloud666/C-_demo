#pragma once // ����ͷ�ļ��ظ�����
#pragma comment(lib,"winmm.lib") // ���ؾ�̬��

#include <iostream>
#include <vector>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

auto const width = 640; // ���ڿ��
auto const height = 720; // ���ڸ߶�
auto const ball_num = 16; // С�����
auto const pi = 3.1415926; // �����

struct Ball { // ��
	double x; // ����x����
	double y; // ����y����
	double step; // ��������ƶ��ٶ�
	double tempstep; // ��¼С���ʼʱ���ٶ�
	int juge; // С���Ƿ�ʼ�ƶ���״̬��ʶ
	int flag; // С���ƶ�ת���״̬��ʶ
	int r; // ��İ뾶
	COLORREF color; // С�����ɫ
};

//-------��ת����Ϸ��ģ��--------//

class Model {
private:
	vector<Ball> balls; // ��ת��С��
public:
	explicit Model() = default;
	~Model() = default;
	void bgm(); // ��������
	int timer(time_t sec, int id); // ��ʱ��
	void init(); // ��ʼ������
	void draw(); // ����������
	void judeMove(); // С���ƶ������ж�
	void ballMove(); // С����ƶ�
};