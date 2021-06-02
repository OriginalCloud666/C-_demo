#include "Model.h"
using namespace std;

auto const width = 640; // 窗口宽度
auto const height = 720; // 窗口高度
auto const pic_width = 640; // 图片宽度
auto const pic_height = 905; // 图片高度
auto const fps_count = 8; // 帧率计算间隔时间

IMAGE src_img(pic_width, pic_height); // 原位图
IMAGE dest_img(pic_width, pic_height); // 处理后位图

//-------The main method--------//

int main(int argc, char* argv) {
	unique_ptr<Model> pModel(new Model(pic_width, pic_height, width)); // Model对象

	pModel->load(src_img, pic_width, pic_height);
	pModel->bgm();
	initgraph(width, height);
	pModel->init(src_img, dest_img, pic_width, pic_height);

	fflush(stdin); // 清空键盘消息缓冲区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓存

	BeginBatchDraw();
	while (true) {
		pModel->frameFun(pic_width, pic_height, width, height);
		pModel->renderFun(dest_img, pic_width, pic_height, fps_count);
		FlushBatchDraw();
	}
	EndBatchDraw();

	closegraph();
	return 0;
}