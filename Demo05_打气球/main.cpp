#include "Model.h"
using namespace std;

IMAGE bk; // 存放背景图片
IMAGE w1; // 存放文字图片
IMAGE w2; // 存放文字图片
IMAGE w3; // 存放文字图片
IMAGE w4; // 存放文字图片

//-------The main method--------//

int main(int argc, char* argv) {
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk, w1, w2, w3, w4);
	pModel->bgm();
	initgraph(width, height);
	pModel->welcome(bk, w1, w2, w3, w4);
	pModel->init();

	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区
	fflush(stdin); // 清空键盘消息缓冲区

	BeginBatchDraw();
	while (1) {
		pModel->draw(bk);
		pModel->mouseControl();
		pModel->keyDown();
		pModel->keyDown2();
		pModel->moveBullet();
		pModel->crashBalloon();
		pModel->moveBalloon();
		FlushBatchDraw();
	}
	EndBatchDraw();

	closegraph();
	return 0;
}