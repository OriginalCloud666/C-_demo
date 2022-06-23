#include "Model.h"
using namespace std;

IMAGE bk; // 存放背景图片
IMAGE w1; // 存放文字图片
IMAGE w2; // 存放文字图片

//-------The main method--------//

int main(int argc, char* argv)
{
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk, w1, w2);
	pModel->bgm();
	initgraph(WIDTH, HEIGHT);
	pModel->welcome(bk, w1, w2);
	pModel->init();
	pModel->pointInit();

	fflush(stdin); // 清空键盘消息缓冲区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区

	BeginBatchDraw();
	while (1)
	{
		pModel->keyDown();
		if (pModel->roleStop(pModel->getRole().flag) == 1) // 返回值为1，说明可以移动
		{
			pModel->moveRole(pModel->getRole().flag);
			Sleep(15);
			pModel->getPoint()[pModel->getCount()].row = pModel->getRole().row;
			pModel->getPoint()[pModel->getCount()].col = pModel->getRole().col;
			pModel->getCount()++;
		}
		pModel->draw();
		for (int i = 0; i < pModel->getCount() - 1; i++)
		{
			pModel->drawLine(&pModel->getPoint()[i], &pModel->getPoint()[i + 1]);
		}
		FlushBatchDraw();
		if (pModel->over() == 1)
		{
			pModel->getFlag()++;
			if (pModel->getFlag() < 3)
			{
				mciSendString("close littlewin", nullptr, 0, nullptr);
				mciSendString("open ./resource/music/littlewin.wav alias littlewin", nullptr, 0, nullptr);
				mciSendString("play littlewin", nullptr, 0, nullptr);
				pModel->pointInit(); // 重新初始化数据
				char fileName[25];
				sprintf(fileName, "Welcome to the %d level!", pModel->getFlag() + 1);
				MessageBox(GetHWnd(), fileName, "游戏继续！", MB_OK);
			}
			else if (pModel->getFlag() == 3)
			{
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
