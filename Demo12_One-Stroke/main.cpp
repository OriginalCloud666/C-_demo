#include "Model.h"
using namespace std;

auto const width = 640; // 窗口宽度
auto const height = 720; // 窗口高度
auto const grid_size = 50; // 格子大小
auto const grid_num = 10; // 地图行列
auto const point_num = 100; // 记录位置点的数量

IMAGE bk; // 存放背景图片
IMAGE w1; // 存放文字图片
IMAGE w2; // 存放文字图片

//-------The main method--------//

int main(int argc, char* argv) {
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk, w1, w2);
	pModel->bgm();
	initgraph(width, height);
	pModel->welcome(bk, w1, w2);
	pModel->init(width, height, grid_num, grid_size, point_num);
	pModel->pointInit(grid_num);

	fflush(stdin); // 清空键盘消息缓冲区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区

	BeginBatchDraw();
	while (1) {
		pModel->keyDown();
		if (pModel->roleStop(pModel->getRole().flag) == 1) { // 返回值为1，说明可以移动
			pModel->moveRole(pModel->getRole().flag);
			Sleep(15);
			pModel->getPoint()[pModel->getCount()].row = pModel->getRole().row;
			pModel->getPoint()[pModel->getCount()].col = pModel->getRole().col;
			pModel->getCount()++;
		}
		pModel->draw(grid_num, grid_size);
		for (int i = 0; i < pModel->getCount() - 1; i++) {
			pModel->drawLine(&pModel->getPoint()[i], &pModel->getPoint()[i + 1], grid_size);
		}
		FlushBatchDraw();
		if (pModel->over(grid_num) == 1) {
			pModel->getFlag()++;
			if (pModel->getFlag() < 3) {
				mciSendString("close littlewin", nullptr, 0, nullptr);
				mciSendString("open ./resource/music/littlewin.wav alias littlewin", nullptr, 0, nullptr);
				mciSendString("play littlewin", nullptr, 0, nullptr);
				pModel->pointInit(grid_num); // 重新初始化数据
				char fileName[25];
				sprintf(fileName, "Welcome to the %d level!", pModel->getFlag() + 1);
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