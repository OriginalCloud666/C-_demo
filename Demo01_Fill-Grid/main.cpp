#include "Model.h"
using namespace std;

auto const width = 640; // 窗口宽度
auto const height = 480; // 窗口高度
auto const grid_num = 4; // 行列格子数量
auto const grid_length = 30; // 格子边长

IMAGE bk; // 存放背景图片

//-------The main method--------//

int main(int argc, char* argv) {
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk);
	pModel->bgm();
	initgraph(width, height);
	pModel->welcome(bk, width, height);
	pModel->init(grid_num, grid_length, width, height);

	fflush(stdin); // 清空键盘消息缓冲区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区

	BeginBatchDraw();
	while (1) {
		pModel->mouseControl(grid_num, grid_length, width, height);
		pModel->draw(bk, grid_num, grid_length, width, height);
		FlushBatchDraw();
		if (pModel->over(grid_num) == grid_num * grid_num) {
			MessageBox(NULL, "游戏通关！", "提示", MB_OK);
			break;
		}
	}
	EndBatchDraw();

	closegraph();
	return 0;
}