#pragma once // ����ͷ�ļ��ظ�����
#pragma comment(lib,"winmm.lib") // ���ؾ�̬��

#include <iostream>
#include <vector>
#include <conio.h>
#include <time.h>
#include <string>
#include <graphics.h>
#include <mmsystem.h>

using namespace std;

struct Rain { // ������
	char* str; // �ַ�����С
	double x; // �ַ�����x����
	double y; // �ַ�����y����
	double step; // �ַ���ÿ���ƶ��ľ���
};

//-------��������ģ��--------//

class Model {
private:
	vector<Rain> rainVec; // ������
public:
	explicit Model() = default;
	~Model() = default;
	void bgm(); // ��������
	void init(const int str_num, const int str_size, const int str_width, const int height); // ��ʼ������
	char createChar(); // �����ַ�
	void rainChange(const int str_num, const int str_size); // �����겻�ϱ仯
	void rainMove(const int str_num, const int str_size, const int str_width, const int height); // �������ƶ�
	void printChar(const int str_num, const int str_size, const int str_width); // �����ʾ�ַ�
};