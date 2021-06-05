#include "Model.h"
using namespace std;

IMAGE bk; // 存放背景图片
IMAGE white; // 存放空白图片

//-------The main method--------//

int main(int argc, char* argv) {
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk, white);
	pModel->bgm();
	initgraph(width, height);
	pModel->welcome(bk);
	pModel->init();

	fflush(stdin); // 清空键盘消息缓存区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区

	BeginBatchDraw();
	while (1) {
		pModel->draw(bk, white);
		FlushBatchDraw();
		if (pModel->over() == true) {
			mciSendString("close win", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/win.wav alias win", nullptr, 0, nullptr);
			mciSendString("play win", nullptr, 0, nullptr);
			MessageBox(GetHWnd(), "You Win!", "游戏结束！", MB_OK);
			break;
		}
		pModel->mouseControl();
	}
	EndBatchDraw();

	closegraph();
	return 0;
}