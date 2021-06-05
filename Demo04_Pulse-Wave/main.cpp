#include "Model.h"
using namespace std;

IMAGE src_img(pic_width, pic_height); // 原位图
IMAGE dest_img(pic_width, pic_height); // 处理后位图

//-------The main method--------//

int main(int argc, char* argv) {
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(src_img);
	pModel->bgm();
	initgraph(width, height);
	pModel->init(src_img, dest_img);

	fflush(stdin); // 清空键盘消息缓冲区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓存

	BeginBatchDraw();
	while (true) {
		pModel->frameFun();
		pModel->renderFun(dest_img);
		FlushBatchDraw();
	}
	EndBatchDraw();

	closegraph();
	return 0;
}