#include "Model.h"
using namespace std;

IMAGE bk; // 用于存放背景图片
IMAGE w1; // 用于存放文字图片
IMAGE w2; // 用于存放文字图片
IMAGE man; // 用于存放人物图片
IMAGE brick; // 用于存放砖块图片
IMAGE replace; // 用于存放替代图片
IMAGE fire[2]; // 用于存放火焰图片
IMAGE door[2]; // 用于存放终点图片
IMAGE ghost[2]; // 用于存放幽灵图片
IMAGE character[2]; // 用于存放人物图片

//-------The main method--------//

int main(int argc, char* argv)
{
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk, w1, w2, man, brick, replace, fire, door, ghost, character);
	pModel->bgm();
	initgraph(WIDTH, HEIGHT);
	pModel->welcome(bk, w1, w2, man);
	pModel->init();

	BeginBatchDraw();
	while (1)
	{
		pModel->welcome(bk, w1, w2, man);
		FlushBatchDraw();
		if (pModel->mouseControl(pModel->getButton1(), pModel->getButton2()) == 1)
		{
			break;
		}
	}
	fflush(stdin); // 清空键盘消息缓冲区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区

	while (1)
	{
		pModel->draw(bk, brick, fire, door, ghost, character);
		FlushBatchDraw();
		pModel->keyDown();
		pModel->moveRole(bk, brick, replace, fire, door, ghost, character);
		if (pModel->over() == true)
		{
			if (pModel->condition1() == 1)
			{
				pModel->getCas()++;
				if (pModel->getCas() < MAXCAS)
				{
					mciSendString("close littlewin", nullptr, 0, nullptr);
					mciSendString("open ./resource/music/littlewin.wav alias littlewin", nullptr, 0, nullptr);
					mciSendString("play littlewin", nullptr, 0, nullptr);
					char fileName[25];
					sprintf(fileName, "Welcome to the %d level!", pModel->getCas() + 1);
					MessageBox(GetHWnd(), fileName, "游戏继续！", MB_OK);
					pModel->searchRole();
				}
				else if (pModel->getCas() == MAXCAS)
				{
					pModel->getCas()--;
					mciSendString("close win", nullptr, 0, nullptr);
					mciSendString("open ./resource/music/win.wav alias win", nullptr, 0, nullptr);
					mciSendString("play win", nullptr, 0, nullptr);
					MessageBox(GetHWnd(), "You Win!", "游戏结束！", MB_OK);
					break;
				}
			}
			else if (pModel->condition2() == 1)
			{
				mciSendString("close end", nullptr, 0, nullptr);
				mciSendString("open ./resource/music/end.wav alias end", nullptr, 0, nullptr);
				mciSendString("play end", nullptr, 0, nullptr);
				MessageBox(GetHWnd(), "You Lose!", "游戏结束！", MB_OK);
				break;
			}
		}
	}
	EndBatchDraw();

	closegraph();
	return 0;
}
