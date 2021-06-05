#include "Model.h"
using namespace std;

IMAGE w1; // 存放文字图片
IMAGE w2; // 存放文字图片
IMAGE w3; // 存放文字图片
IMAGE bk; // 存放背景图片
IMAGE mine; // 存放红雷图片
vector<IMAGE> img(pic_num); // 存放数字图片
vector<int> imgIndex = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

//-------The main method--------//

int main(int argc, char* argv) {
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk, w1, w2, w3, mine, img, imgIndex);
	pModel->bgm();
	initgraph(width, height);
	pModel->welcome(bk, w1, w2, w3);

	fflush(stdin); // 清空键盘消息缓冲区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓存

	pModel->keyDown();
	pModel->getStart() = clock();

	BeginBatchDraw();
	while (1) {
		pModel->draw(bk, mine, img);
		FlushBatchDraw();
		pModel->over(bk, mine, img, pModel->mouseControl());
	}
	EndBatchDraw();

	closegraph();
	return 0;
}