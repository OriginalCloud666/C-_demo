#include "Model.h"
using namespace std;

auto const width = 640; // 窗口宽度
auto const height = 720; // 窗口高度
auto const grid_num = 4; // 行列格子数量
auto const grid_width = 160; // 格子宽度
auto const grid_height = 180; // 格子高度

IMAGE bk; // 存放背景图片
IMAGE white; // 存放空白图片

//-------The main method--------//

int main(int argc, char* argv) {
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk, white, width, height);
	pModel->bgm();
	initgraph(width, height);
	pModel->welcome(bk);
	pModel->init(width, height, grid_num, grid_width, grid_height);

	fflush(stdin); // 清空键盘消息缓存区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区

	BeginBatchDraw();
	while (1) {
		pModel->draw(bk, white, grid_num, grid_width, grid_height);
		FlushBatchDraw();
		if (pModel->over(grid_num) == true) {
			mciSendString("close win", nullptr, 0, nullptr);
			mciSendString("open ./resource/music/win.wav alias win", nullptr, 0, nullptr);
			mciSendString("play win", nullptr, 0, nullptr);
			MessageBox(GetHWnd(), "You Win!", "游戏结束！", MB_OK);
			break;
		}
		pModel->mouseControl(grid_num, grid_width, grid_height);
	}
	EndBatchDraw();

	closegraph();
	return 0;
}