#include "Model.h"
using namespace std;

IMAGE bk;
vector<IMAGE> img(pic_num); // 存放图片
vector<int> imgIndex = { 0, 1, 2, 3, 4, 5, 6, 7, 10, 11 };

//-------The main method--------//

int main(int argc, char* argv) {
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk, img, imgIndex);
	pModel->bgm();
	initgraph(width, height);
	pModel->welcome(bk);
	pModel->init();

	fflush(stdin); // 清空键盘消息缓存区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区

	BeginBatchDraw();
	while (1) {
		pModel->draw(bk, img);
		FlushBatchDraw();
		if (pModel->over() == 1) {
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
				pModel->action(bk, img);
				MessageBox(GetHWnd(), "You Win!", "游戏结束！", MB_OK);
				break;
			}
		}
		pModel->keyDown();
	}
	EndBatchDraw();

	closegraph();
	return 0;
}