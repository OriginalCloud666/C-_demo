#include "Model.h"
using namespace std;

// ���ñ�������
void Model::bgm() {
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// �����ַ�
char Model::createChar() {
	char temp;
	int flag = rand() % 3;

	// ���������ͬ���ַ�
	if (flag == 0) { // ������д��ĸ
		temp = rand() % 26 + 'A';
	}
	else if (flag == 1) { // ����Сд��ĸ
		temp = rand() % 26 + 'a';
	}
	else { // ��������
		temp = rand() % 10 + '0';
	}
	return temp;
}

// ��ʼ������
void Model::init(const int str_num, const int str_size, const int str_width, const int height) {
	SetWindowText(GetHWnd(), "C++�ڿ͵۹�������");
	// �������������
	srand((unsigned)time(nullptr) * rand());
	// �Խṹ����г�ʼ��
	for (int i = 0; i < str_num; i++) {
		struct Rain newrain;
		newrain.x = i * str_width; // ���ַ���x�����ʼ��
		newrain.y = rand() % (height + str_size * str_width) - str_size * str_width;
		newrain.step = (rand() % 2000) / 1000.0 + 3;
		newrain.str = new char[str_size];
		for (int k = 0; k < str_size; k++) {
			newrain.str[k] = this->createChar();
		}
		this->rainVec.push_back(newrain);
	}
}

// �������겻�ϱ仯
void Model::rainChange(const int str_num, const int str_size) {
	for (int i = 0; i < str_num; i++) {
		this->rainVec[rand() % str_num].str[rand() % str_size] = this->createChar();
	}
}

// �������겻������
void Model::rainMove(const int str_num, const int str_size, const int str_width, const int height) {
	// ������������
	for (int i = 0; i < str_num; i++) {
		this->rainVec[i].y += this->rainVec[i].step;
		// ���ý��ޣ��������곬����Ļʱ��������
		if (this->rainVec[i].y > height) {
			this->rainVec[i].y = -str_size * str_width; // ���ַ���y�����ʼ��
			this->rainVec[i].step = (rand() % 2000) / 1000.0 + 3; // ���ַ��������ٶȽ��г�ʼ��
		}
	}
}

// ����ַ�����ͼ����
void Model::printChar(const int str_num, const int str_size, const int str_width) {
	// �����Ļ
	cleardevice();
	// ���������ʽ
	settextstyle(0, 0, 0, 0, 0, 1000, 0, 0, 0);
	// �ڻ�ͼ�����϶��ַ����������
	for (int i = 0; i < str_num; i++) {
		for (int k = 0; k < str_size; k++) {
			settextcolor(RGB(0, 15 + 12 * k, 0));
			outtextxy(this->rainVec[i].x + 2, this->rainVec[i].y + str_width * k, this->rainVec[i].str[k]);
		}
	}
}