#include "Model.h"
using namespace std;

auto const width = 640; // 窗口宽度
auto const height = 720; // 窗口高度
auto const pic_size = 32; // 图片大小
auto const pic_num = 12; // 图片数量

auto const map_size1 = 9; // 地图大小（小）
auto const map_size2 = 13; // 地图大小（中）
auto const map_size3 = 17; // 地图大小（大）
auto const mine_num1 = 10; // 地雷数量（少）
auto const mine_num2 = 26; // 地雷数量（中）
auto const mine_num3 = 47; // 地雷数量（多）

IMAGE w1; // 存放文字图片
IMAGE w2; // 存放文字图片
IMAGE w3; // 存放文字图片
IMAGE bk; // 存放背景图片
IMAGE mine; // 存放红雷图片
vector<IMAGE> img(pic_num); // 存放数字图片
vector<int> imgIndex = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

//-------The main method--------//

int main(int argc, char* argv) {
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk, w1, w2, w3, mine, img, imgIndex, pic_size, pic_num, width, height);
	pModel->bgm();
	initgraph(width, height);
	pModel->welcome(bk, w1, w2, w3);

	fflush(stdin); // 清空键盘消息缓冲区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓存

	pModel->keyDown(map_size1, map_size2, map_size3, mine_num1, mine_num2, mine_num3, pic_size, width, height);
	pModel->getStart() = clock();

	BeginBatchDraw();
	while (1) {
		pModel->draw(bk, mine, img, pic_size);
		FlushBatchDraw();
		pModel->over(bk, mine, img, pic_size, pModel->mouseControl(pic_size), width, height);
	}
	EndBatchDraw();

	closegraph();
	return 0;
}