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
auto const pic_num = 10; // ͼƬ����
auto const pic_size = 50; // ͼƬ��С
auto const grid_num = 8; // ��ͼ����

//-------��������Ϸ��ģ��--------//

class Model {
private:
	int flag; // ״̬��ʶ���жϹؿ�
	int top; // ��ͼ���Ͻ�y����
	int left; // ��ͼ���Ͻ�x����
	int down; // ��ͼ���½�y����
	int right; // ��ͼ���½�x����
	vector<vector<vector<int>>> gameMapVec; // ��ŵ�ͼ���ؿ���Ϣ
public:
	explicit Model();
	~Model() = default;
	int& getFlag();
	void load(IMAGE& bk, vector<IMAGE>& img, vector<int>& imgIndex); // ������Դ
	void bgm(); // ��������
	void init(); // ��ʼ������
	void welcome(IMAGE& bk); // ���ƿ�ʼ����
	void draw(IMAGE& bk, vector<IMAGE>& img); // ������Ϸ����
	void changOne(const int i, const int j); // �������λ��
	void keyDown(); // ������Ӧ
	int over(); // ��Ϸ�����ж�
	void action(IMAGE& bk, vector<IMAGE>& img); // ��������
};