#include "Model.h"
using namespace std;

IMAGE bk; // 存放背景图片

//-------The main method--------//

int main(int argc, char* argv) {
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk);
	pModel->bgm();
	initgraph(width, height);
	pModel->draw(bk);
	pModel->init();

	fflush(stdin); // 清空键盘消息缓冲区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区

	BeginBatchDraw();
	while (!_kbhit()) {
		cleardevice();
		pModel->draw(bk);

		for (int i = 0; i < star_num; i++) {
			pModel->moveStar(i);
		}
		Sleep(10);
		FlushBatchDraw();
	}
	EndBatchDraw();

	closegraph();
	return 0;
}