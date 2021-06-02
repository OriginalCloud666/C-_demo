#include "Model.h"
using namespace std;

auto const width = 640; // 窗口宽度
auto const height = 720; // 窗口高度
auto const grid_num = 4; // 行列格子数量
auto const grid_length = 60; // 格子边长
auto const photo_num = 12; // 图片数量

IMAGE bk; // 存放背景图片
vector<IMAGE> img(photo_num); // 存放数字图片
vector<int> imgIndex = { 0, 2, 4, 8, 16, 32, 64,128, 256, 512, 1024, 2048 };

//-------The main method--------//

int main(int argc, char* argv) {
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk, img, imgIndex, grid_length, width, height, photo_num);
	pModel->bgm();
	initgraph(width, height);
	pModel->welcome(bk);
	pModel->init(grid_num, grid_length, width, height);

	fflush(stdin); // 清空键盘消息缓存区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区

	BeginBatchDraw();
	while (1) {
		pModel->keyDown(grid_num);
		pModel->draw(bk, img, imgIndex, grid_num, grid_length, photo_num);
		FlushBatchDraw();
		if (pModel->over(grid_num)) {
			char output[30]; // 定义字符数组，用来以字符串形式存放得分
			sprintf(output, "游戏结束，您的分数为%d。", pModel->getScore()); // 得分写入output数组
			MessageBox(nullptr, output, "提示", MB_OK);
			break;
		}
	}
	EndBatchDraw();

	closegraph();
	return 0;
}