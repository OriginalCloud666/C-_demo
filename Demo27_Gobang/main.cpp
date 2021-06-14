#include "Model.h"
using namespace std;

IMAGE bk; // 存放背景图片
IMAGE w1; // 存放文字图片
IMAGE w2; // 存放文字图片

//-------The main method--------//

int main(int argc, char* argv) {
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk, w1, w2);
	pModel->bgm();
	initgraph(width, height);
	pModel->init();

	BeginBatchDraw();//双缓冲
	while (1) {
		pModel->welcome(w1, w2);
		FlushBatchDraw();
		if (pModel->mouseControl(pModel->getButton1(), pModel->getButton2()) == 1) {
			break;
		}
	}
	fflush(stdin); // 清空键盘消息缓冲区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区

	while (1) {
		pModel->keyDown();
		pModel->draw();
		FlushBatchDraw();
		if (pModel->over() == true) {
			pModel->draw();
			FlushBatchDraw();
			if (pModel->getFlag() == 1) {
				MessageBox(GetHWnd(), "黑方胜利！", "Game Over!", MB_OK);
			}
			else if (pModel->getFlag() == 2) {
				MessageBox(GetHWnd(), "白方胜利！", "Game Over!", MB_OK);
			}
			else if (pModel->getFlag() == 3) {
				MessageBox(GetHWnd(), "双方和棋！", "Game Over!", MB_OK);
			}
			break;
		}
	}
	EndBatchDraw();

	closegraph();
	return 0;
}