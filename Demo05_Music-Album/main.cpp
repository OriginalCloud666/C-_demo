#include "Model.h"
using namespace std;

IMAGE w1; // 存放文字图片
IMAGE bk1; // 存放背景图片
IMAGE bk2; // 存放背景图片
vector<IMAGE> img(pic_num); // 存放展示图片
vector<int> imgIndex = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
vector<int> pic_width = { 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 450, 450, };
vector<int> pic_height = { 570, 570, 570, 570, 570, 570, 570, 570, 570, 570, 442, 442, };
vector<int> pic_x = { 75, 92, 90, 80, 72, 116, 115, 116, 115, 78, 85, 80, };
vector<int> pic_y = { 75, 138, 70, 80, 98, 88, 75, 90, 105, 112, 142, 95, };

char s[7][5] = {//定义二维数组，用于保存多个字符串
	"音",
	"乐",
	"相",
	"册",
	"！",
	"嘿",
	"嘿",
};

//-------The main method--------//

int main(int argc, char* argv) {
	unique_ptr<Model> pModel(new Model()); // Model对象

	pModel->load(bk1, bk2, w1, img, imgIndex, pic_width, pic_height);
	pModel->bgm();
	initgraph(width, height);
	pModel->welcome(bk1, w1);

	fflush(stdin); // 清空键盘消息缓存区
	FlushMouseMsgBuffer(); // 清空鼠标消息缓冲区

	BeginBatchDraw();
	while (1) {
		pModel->drawHeart(bk2, img, pic_x, pic_y, width / 2, height / 4 + 10, 100, width, s, 0, 0.5, 0);
		pModel->drawHeart(bk2, img, pic_x, pic_y, width / 2, height / 4 + 10, 100, width, s, 0, 0.5, 0);
		pModel->getCount()++;
	}
	EndBatchDraw();

	closegraph();
	return 0;
}