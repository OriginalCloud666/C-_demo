#pragma once // ����ͷ�ļ��ظ�����
#pragma comment(lib,"winmm.lib") // ���ؾ�̬��

#include <iostream>
#include <vector>
#include <time.h>
#include <conio.h>
#include <graphics.h>
#include <mmsystem.h>

using namespace std;

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
	void load(IMAGE& bk, const int width, const int height); // ������Դ
	void bgm(); // ��������
	struct Star initStar(const int height); // ��ʼ������
	void moveStar(const int i, const int weidth, const int height); // �ƶ�����
	void init(const int star_num, const int width, const int height); // ��ʼ������
	void draw(IMAGE& bk); // ������Ϸ����
};