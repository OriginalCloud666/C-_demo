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
auto const pi = 3.1415926; // �����

//-------ӣ������Ϸ��ģ��--------//

class Model {
private:
	double offsetAngle; // ����֦�ɺ͸�֦��ƫ��ĽǶ�
	double shortenRate = 0.65; // ��֦�ɱȸ�֦�ɱ�̵ı���
	int isShowAnimation = 1; // �Ƿ���ʾ�����ɵĹ��̶���
public:
	explicit Model();
	~Model() = default;
	void bgm(); // ��������
	void init(); // ��ʼ������
	double mapValue(double input, double inputMin, double inputMax, double outputMin, double outputMax); // ӳ��
	double randBetween(double min, double max); // �����
	void brunch(double x_start, double y_start, double length, double angle, double thickness, int generation); // ��֧��
	void judeMove(); // С���ƶ������ж�
	void ballMove(); // С����ƶ�
	void mouseControl(); // �����Ϣ��Ӧ
};