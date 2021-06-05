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
		pModel->keyDown1();
		pModel->keyDown2();
		pModel->ballMove();
		pModel->ballCrash();
		pModel->draw();
		FlushBatchDraw();
		if (pModel->over() == 1 && pModel->getBall().flag == 0) {
			mciSendString("close win", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/win.wav alias win", nullptr, 0, nullptr);
			mciSendString("play win", nullptr, 0, nullptr);
			MessageBox(GetHWnd(), "You Win!", "游戏结束！", MB_OK);
			break;
		}
	}
	EndBatchDraw();

	closegraph();
	return 0;
}