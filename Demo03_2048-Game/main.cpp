#include "Model.h"
using namespace std;

auto const width = 640; // 窗口宽度
auto const height = 720; // 窗口高度
auto const grid_num = 4; // 行列格子数量
auto const grid_length = 60; // 格子边长

IMAGE bk; // 存放背景图片
IMAGE img[12]; // 存放数字图片
int imgIndex[12] = { 0, 2, 4, 8, 16, 32, 64,128, 256, 512, 1024, 2048 };
int score = 0; // 记录得分

//-------The main method--------//

int main(int argc, char* argv) {
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk, img, imgIndex, grid_length, width, height);
	pModel->bgm();
	initgraph(width, height);
	pModel->welcome(bk);
	pModel->init(grid_num, grid_length, width, height);

	fflush(stdin); // 清空键盘消息缓存区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区

	BeginBatchDraw();
	while (1) {
		pModel->keyDown(score, grid_num);
		pModel->draw(bk, img, imgIndex, grid_num, grid_length, score);
		FlushBatchDraw();
		if (pModel->over(grid_num)) {
			char output[30]; // 定义字符数组，用来以字符串形式存放得分
			sprintf(output, "游戏结束，您的分数为%d。", score); // 得分写入output数组
			MessageBox(NULL, output, "提示", MB_OK);
			break;
		}
	}
	EndBatchDraw();

	closegraph();
	return 0;
}