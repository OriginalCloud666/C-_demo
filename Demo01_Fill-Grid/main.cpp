#include "Model.h"
using namespace std;

IMAGE bk; // 存放背景图片

//-------The main method--------//

int main(int argc, char* argv) {
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk);
	pModel->bgm();
	initgraph(width, height);
	pModel->welcome(bk);
	pModel->init();

	fflush(stdin); // 清空键盘消息缓冲区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区

	BeginBatchDraw();
	while (1) {
		pModel->mouseControl();
		pModel->draw(bk);
		FlushBatchDraw();
		if (pModel->over() == grid_num * grid_num) {
			MessageBox(nullptr, "游戏通关！", "提示", MB_OK);
			break;
		}
	}
	EndBatchDraw();

	closegraph();
	return 0;
}