#pragma once // ����ͷ�ļ��ظ�����
#pragma comment(lib,"winmm.lib") // ���ؾ�̬��

#include <iostream>
#include <vector>
#include <time.h>
#include <conio.h>
#include <graphics.h>
#include <mmsystem.h>
using namespace std;

auto const width = 640; // ���ڿ��
auto const height = 480; // ���ڸ߶�
auto const star_num = 100; // ��������

struct Star {
	double x; // ����x����
	double y; // ����y����
	double radius; // ���ǰ뾶
	double step; // �����ٶ�
	int color; // ������ɫ
};

//-------��˸������Ϸ��ģ��--------//

class Model {
private:
	vector<Star> startVec; // �������
public:
	explicit Model() = default;
	~Model() = default;
	void load(IMAGE& bk); // ������Դ
	void bgm(); // ��������
	struct Star initStar(); // ��ʼ������
	void moveStar(const int i); // �ƶ�����
	void init(); // ��ʼ������
	void draw(IMAGE& bk); // ������Ϸ����
};