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

auto const width = 1280; // ���ڿ��
auto const height = 720; // ���ڸ߶�
auto const str_width = 15; // �ַ������
auto const str_num = 86; // �ַ�����Ŀ
auto const str_size = 20; // �ַ������ַ���

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
	void init(); // ��ʼ������
	char createChar(); // �����ַ�
	void rainChange(); // �����겻�ϱ仯
	void rainMove(); // �������ƶ�
	void printChar(); // �����ʾ�ַ�
};