#include "Model.h"
using namespace std;

IMAGE bk;//存放背景图片
IMAGE button;//存放按钮图片
vector<IMAGE> img(pic_num); // 存放图片
vector<int> imgIndex = { 0, 1, 2, 3, 4, 5 };

//-------The main method--------//

int main(int argc, char* argv) {
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk, button, img, imgIndex);
	pModel->bgm();
	initgraph(width, height);
	pModel->welcome(bk);
	pModel->init();

	fflush(stdin); // 清空键盘消息缓冲区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区

	BeginBatchDraw();
	while (1) {
		pModel->keyDown();
		pModel->mouseControl(pModel->getBeginGame());
		pModel->draw(bk, button, img);
		FlushBatchDraw();
		if (pModel->over() == 1) { // 如果游戏结束
			pModel->getFlag()++;
			if (pModel->getFlag() < 3) {
				mciSendString("close littlewin", nullptr, 0, nullptr);
				mciSendString("open ./resource/music/littlewin.wav alias littlewin", nullptr, 0, nullptr);
				mciSendString("play littlewin", nullptr, 0, nullptr);
				char fileName[25];
				sprintf(fileName, "Welcome to the %d level！", pModel->getFlag() + 1);
				MessageBox(GetHWnd(), fileName, "游戏继续！", MB_OK);
			}
			else if (pModel->getFlag() == 3) {
				pModel->getFlag()--;
				mciSendString("close win", nullptr, 0, nullptr);
				mciSendString("open ./resource/music/win.wav alias win", nullptr, 0, nullptr);
				mciSendString("play win", nullptr, 0, nullptr);
				MessageBox(GetHWnd(), "You Win!", "游戏结束！", MB_OK);
				break;
			}
		}
	}
	EndBatchDraw();

	closegraph();
	return 0;
}