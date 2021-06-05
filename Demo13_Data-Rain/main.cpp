#include "Model.h"
using namespace std;

//-------The main method--------//

int main(int argc, char* argv) {
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->bgm();
	initgraph(width, height);
	pModel->init();

	fflush(stdin); // 清空键盘消息缓冲区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区

	BeginBatchDraw();
	while (1) {
		pModel->printChar();
		pModel->rainChange();
		FlushBatchDraw();
		pModel->rainMove();
	}
	EndBatchDraw();

	closegraph();
	return 0;
}