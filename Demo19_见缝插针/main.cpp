#include "Model.h"
using namespace std;

IMAGE bk; // 存放背景图片
IMAGE w1; // 存放文字图片
IMAGE w2; // 存放文字图片
vector<IMAGE> img(PIC_NUM); // 存放图片
vector<int> imgIndex = { 0, 1, 2, 3 };

//-------The main method--------//

int main(int argc, char* argv)
{
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk, w1, w2, img, imgIndex);
	pModel->bgm();
	initgraph(WIDTH, HEIGHT);
	pModel->init();

	BeginBatchDraw();
	while (1)
	{
		pModel->welcome(bk, w1, w2);
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
		if (pModel->getNeedleNum() < NEEDLE_NUM)
		{
			pModel->keyDown(bk, img);
		}
		pModel->rotateNeedle();
		if (pModel->timer(10, 0))
		{
			pModel->getFinaltime() -= 0.01;
		}
		pModel->draw(bk, img);
		FlushBatchDraw();
		if (pModel->over() == 1)
		{
			pModel->draw(bk, img);
			FlushBatchDraw();
			mciSendString("close win", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/win.wav alias win", nullptr, 0, nullptr);
			mciSendString("play win", nullptr, 0, nullptr);
			if (pModel->getRole1().score * 1.0 / pModel->getRole1().count > pModel->getRole2().score * 1.0 / pModel->getRole2().count)
			{
				MessageBox(GetHWnd(), "玩家1获胜！", "Game Over!", MB_OK);
			}
			else if (pModel->getRole1().score * 1.0 / pModel->getRole1().count < pModel->getRole2().score * 1.0 / pModel->getRole2().count)
			{
				MessageBox(GetHWnd(), "玩家2获胜！", "Game Over!", MB_OK);
			}
			else
			{
				if (pModel->getRole1().score > pModel->getRole2().score)
				{
					MessageBox(GetHWnd(), "玩家1获胜！", "Game Over!", MB_OK);
				}
				else if (pModel->getRole1().score < pModel->getRole2().score)
				{
					MessageBox(GetHWnd(), "玩家2获胜！", "Game Over!", MB_OK);
				}
				else
				{
					MessageBox(GetHWnd(), "双方玩家平局！", "Game Over!", MB_OK);
				}
			}
			break;
		}
	}
	EndBatchDraw();

	closegraph();
	return 0;
}
