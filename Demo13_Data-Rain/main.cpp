#include "Model.h"
using namespace std;

auto const width = 1280; // 窗口宽度
auto const height = 720; // 窗口高度
auto const str_width = 15; // 字符串宽度
auto const str_num = 86; // 字符串数目
auto const str_size = 20; // 字符串内字符数

//-------The main method--------//

int main(int argc, char* argv) {
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->bgm();
	initgraph(width, height);
	pModel->init(str_num, str_size, str_width, height);

	fflush(stdin); // 清空键盘消息缓冲区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区

	BeginBatchDraw();
	while (1) {
		pModel->printChar(str_num, str_size, str_width);
		pModel->rainChange(str_num, str_size);
		FlushBatchDraw();
		pModel->rainMove(str_num, str_size, str_width, height);
	}
	EndBatchDraw();

	closegraph();
	return 0;
}