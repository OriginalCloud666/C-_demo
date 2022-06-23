#include "Model.h"
using namespace std;

Model::Model() :score(0), top(0), left(0), down(0), right(0) {}

// 获取score
int& Model::getScore()
{
	return this->score;
}

// 加载资源
void Model::load(IMAGE& bk, vector<IMAGE>& img, vector<int>& imgIndex)
{
	// 加载数字图片
	for (int i = 0; i < PHOTO_NUM; i++)
	{
		// 批量加载
		char fileName[25]; // 定义字符数组，用来存放带有拓展名的图片名称
		sprintf(fileName, "./resource/picture/%d.bmp", imgIndex[i]);
		loadimage(&img[i], fileName, GRID_LENGTH, GRID_LENGTH);
	}
	// 加载背景图片
	loadimage(&bk, "./resource/picture/bk.jpg", WIDTH, HEIGHT);
}

// 加载音乐
void Model::bgm()
{
	mciSendString("open ./resource/music/bk.mp3 alias BGM", nullptr, 0, nullptr);
	mciSendString("play BGM repeat", nullptr, 0, nullptr);
}

// 初始化数据
void Model::init()
{
	// 初始化游戏区域坐标
	this->left = WIDTH / 2 - (GRID_NUM * GRID_LENGTH) / 2; // 游戏区域左上角x坐标
	this->top = HEIGHT / 2 - (GRID_NUM * GRID_LENGTH) / 2; // 游戏区域左上角y坐标
	this->right = this->left + (GRID_NUM * GRID_LENGTH); // 游戏区域右下角x坐标
	this->down = this->top + (GRID_NUM * GRID_LENGTH); // 游戏区域右下角y坐标

	// 初始化vector容器
	this->gameMapVec.clear();
	for (int i = 0; i < GRID_NUM; i++)
	{
		vector<int> lineGrid;
		for (int j = 0; j < GRID_NUM; j++)
		{
			lineGrid.push_back(0);
		}
		this->gameMapVec.push_back(lineGrid);
	}
}

// 开始界面绘制
void Model::welcome(IMAGE& bk)
{
	putimage(0, 0, &bk);
	setbkmode(TRANSPARENT);
	SetWindowText(GetHWnd(), "C++2048游戏");

	// 输出作者信息
	setbkcolor(WHITE);
	settextstyle(52, 0, "楷体", 0, 0, FW_LIGHT, 0, 0, 0);
	settextcolor(RGB(130, 130, 130));
	outtextxy(218, 440, "2048游戏");

	while (!_kbhit())
	{
		settextstyle(26, 0, "楷体", 0, 0, FW_EXTRABOLD, 0, 0, 0);
		settextcolor(RGB(245, 245, 245));
		outtextxy(232, 500, "按任意键继续！");
	}
}

// 游戏界面绘制
void Model::draw(IMAGE& bk, vector<IMAGE>& img, vector<int>& imgIndex)
{
	cleardevice();
	putimage(0, 0, &bk);

	// 对游戏区域外轮廓进行加深
	for (int x = 3; x > -2; x--)
	{
		setlinecolor(RGB(244, 215, 215));
		rectangle(this->left - x, this->top - x, this->right + x, this->down + x);
	}

	// 绘制外边框
	for (int x = 22; x > 12; x--)
	{
		setlinecolor(RGB(244, 215, 215)); // 设置外边框颜色
		rectangle(this->left - x, this->top - x, this->right + x, this->down + x);
	}

	// 绘制头部边框
	for (int x = 10; x > 0; x--)
	{
		setlinecolor(RGB(244, 215, 215)); // 设置头部边框颜色
		rectangle(this->left - 12 - x, this->top - 122 - x, this->right + 12 + x, this->top - 42 + x);
	}

	// 绘制头部边框内部
	setfillcolor(WHITE); // 对固体长方形进行内部颜色的填充
	solidrectangle(this->left - 12 + 13, this->top - 122 + 13, this->right + 12 - 13, this->top - 42 - 13);

	// 对头部白色边框轮廓进行加深
	for (int x = 3; x > -5; x--)
	{
		setlinecolor(RGB(244, 215, 215)); // 设置外边框颜色
		rectangle(this->left - 12 + 10 - x, this->top - 122 + 10 - x, this->right + 12 - 10 + x, this->top - 42 - 10 + x);
	}

	int k;
	// 根据二维数组中的值去画图
	for (int i = 0; i < GRID_NUM; i++)
	{
		for (int j = 0; j < GRID_NUM; j++)
		{
			// 将数组下标转化为坐标
			int x = this->left + j * GRID_LENGTH;
			int y = this->top + i * GRID_LENGTH;
			for (k = 0; k < PHOTO_NUM; k++)
			{
				// 求出当前元素对应的图片序号
				if (imgIndex[k] == this->gameMapVec[i][j])
				{
					break;
				}
			}
			// 贴相应序号的图片
			putimage(x, y, &img[k]);
		}
	}

	// 绘制头部分数
	settextcolor(RGB(244, 215, 215));
	settextstyle(35, 0, "楷体", 0, 0, 1000, 0, 0, 0);
	outtextxy((this->left + this->right) / 2 - 87, 142, "得分：");

	settextcolor(RGB(244, 215, 215));
	settextstyle(45, 0, "Times New Roman", 0, 0, FW_BOLD, 0, 0, 0);
	char scoreName[20]; // 定义字符数组，用来以字符串形式存放得分
	sprintf(scoreName, "%d ", this->score); // 得分写入scoreName数组
	outtextxy((this->left + this->right) / 2 + 20, 137, scoreName);
}

// 游戏结束条件1：没有格子为0，游戏结束
bool Model::condition1()
{
	int count = 0;
	for (int i = 0; i < GRID_NUM; i++)
	{
		for (int j = 0; j < GRID_NUM; j++)
		{
			if (this->gameMapVec[i][j] != 0)
			{
				count++;
			}
		}
	}
	if (count == GRID_NUM * GRID_NUM)
	{
		return true; // 游戏结束
	}
	else
	{
		return false; // 游戏继续
	}
}

// 游戏结束条件2：相邻（上下，左右）的值不相等
bool Model::condition2()
{
	int count = 0;
	for (int i = 0; i < GRID_NUM; i++)
	{
		for (int j = 0; j < GRID_NUM - 1; j++)
		{
			if ((this->gameMapVec[i][j] == this->gameMapVec[i][j + 1] ||
				this->gameMapVec[j][i] == this->gameMapVec[j + 1][i]))
			{
				count++;
			}
		}
	}
	if (count == 0)
	{
		return true; // 游戏结束
	}
	else
	{
		return false; // 游戏继续
	}
}

// 判断游戏是否结束
bool Model::over()
{
	if (this->condition1() && this->condition2())
	{
		return true;
	}
	else
	{
		return false;
	}
}

// 随机产生一个整形数
int Model::randIntNum()
{
	srand((unsigned)(time(nullptr) + rand()));

	while (!this->over())
	{
		int i = rand() % 4;
		int j = rand() % 4;
		if (this->gameMapVec[i][j] == 0 || condition1())
		{
		out:
			this->gameMapVec[i][j] = (rand() % 3) * 2; // 产生随机的0,2,4
			// 产生0的话，该次产生的值不算
			if (this->gameMapVec[i][j] == 0)
			{
				goto out;
			}
			break;
		}
	}
	return 0; // 返回值为0，代表一切正常，返回的随机数为2，或4
}

// 向左移动
int Model::moveLeft()
{
	int flag = 0; // 游戏结束标记
	for (int i = 0; i < GRID_NUM; i++)
	{
		for (int j = 0; j < GRID_NUM; j++)
		{
			int curKey = this->gameMapVec[i][j]; // 当前元素的值
			if (curKey != 0)
			{
				int k = j + 1;
				while (k < GRID_NUM)
				{
					int curKeyNext = this->gameMapVec[i][k]; // 当前元素的下一个
					if (curKeyNext != 0)
					{
						// 如果相邻的是相同，那么就合并
						if (curKey == curKeyNext)
						{
							flag = 1;
							this->gameMapVec[i][j] += this->gameMapVec[i][k];
							this->gameMapVec[i][k] = 0;
							this->score += this->gameMapVec[i][j]; // 记录得分
						}
						k = 4; // 退出循环
						break;
					}
					k++; // 判断其他的位置
				}
			}
		}
	}
	for (int i = 0; i < GRID_NUM; i++)
	{
		for (int j = 0; j < GRID_NUM - 1; j++)
		{
			int curKey = this->gameMapVec[i][j];
			if (curKey == 0)
			{
				int k = j + 1;
				while (k < GRID_NUM)
				{
					int curKeyNext = this->gameMapVec[i][k];
					if (curKeyNext != 0)
					{
						flag = 1;
						this->gameMapVec[i][j] = curKeyNext; // 把非0元素移到0的位置
						this->gameMapVec[i][k] = 0; // 移动后变为0
						k = 4;
					}
					k++; // 其他位置也需要判断
				}
			}
		}
	}
	if (flag)
	{
		return 0;
	}
	else
	{
		return 4;
	}
}

// 向右移动
int Model::moveRight()
{
	int flag = 0; // 游戏结束标志
	for (int i = 0; i < GRID_NUM; i++)
	{
		for (int j = GRID_NUM - 1; j >= 0; j--)
		{
			int curkey = this->gameMapVec[i][j]; // 当前元素的值
			if (curkey != 0)
			{
				int k = j - 1;
				while (k >= 0)
				{
					int curkeyNext = this->gameMapVec[i][k]; // 当前元素的左边下一个元素的值
					if (curkeyNext != 0)
					{
						// 如果相邻的是相同，那么我们就合并
						if (curkey == curkeyNext)
						{
							flag = 1;
							this->gameMapVec[i][j] += this->gameMapVec[i][k];
							this->gameMapVec[i][k] = 0;
							this->score += this->gameMapVec[i][j]; // 记录得分
						}
						k = -1; // 退出循环
						break;
					}
					k--; // 判断其他的位置
				}
			}
		}
	}
	for (int i = 0; i < GRID_NUM; i++)
	{
		for (int j = GRID_NUM - 1; j > 0; j--)
		{
			int curkey = this->gameMapVec[i][j]; // 当前元素的值
			if (curkey == 0)
			{
				int k = j - 1;
				while (k >= 0)
				{
					int curkeyNext = this->gameMapVec[i][k]; // 当前元素的左边下一个元素的值
					if (curkeyNext != 0)
					{
						flag = 1;
						this->gameMapVec[i][j] = curkeyNext; // 把非零元素移到0的位置
						this->gameMapVec[i][k] = 0; // 移动后变为0
						k = -1;
					}
					k--; // 其他位置也需要判断
				}
			}
		}
	}
	if (flag)
	{
		return 0;
	}
	else
	{
		return 4;
	}
}

// 向上移动
int Model::moveUp()
{
	int flag = 0;
	for (int i = 0; i < GRID_NUM; i++)
	{
		for (int j = 0; j < GRID_NUM; j++)
		{
			int curKey = this->gameMapVec[i][j];
			if (curKey != 0)
			{
				int k = i + 1;
				while (k < GRID_NUM)
				{
					int curKeyNext = this->gameMapVec[k][j];
					if (curKeyNext != 0)
					{
						if (curKey == curKeyNext)
						{
							flag = 1;
							this->gameMapVec[i][j] += this->gameMapVec[k][j];
							this->gameMapVec[k][j] = 0;
							this->score += this->gameMapVec[i][j]; // 记录得分
						}
						k = 4;
						break;
					}
					k++;
				}
			}
		}
	}
	for (int i = 0; i < GRID_NUM - 1; i++)
	{
		for (int j = 0; j < GRID_NUM; j++)
		{
			int curKey = this->gameMapVec[i][j];
			if (curKey == 0)
			{
				int k = i + 1;
				while (k < GRID_NUM)
				{
					int curKeyNext = this->gameMapVec[k][j];
					if (curKeyNext != 0)
					{
						flag = 1;
						this->gameMapVec[i][j] = curKeyNext;
						this->gameMapVec[k][j] = 0;
						k = 4;
					}
					k++;
				}
			}
		}
	}
	if (flag)
	{
		return 0;
	}
	else
	{
		return 4;
	}
}

// 向下移动
int Model::moveDown() {
	int flag = 0;
	for (int i = GRID_NUM - 1; i >= 0; i--)
	{
		for (int j = 0; j < GRID_NUM; j++)
		{
			int curKey = this->gameMapVec[i][j];
			if (curKey != 0)
			{
				int k = i - 1;
				while (k >= 0)
				{
					int curKeyNext = this->gameMapVec[k][j];
					if (curKeyNext != 0)
					{
						if (this->gameMapVec[i][j] == this->gameMapVec[k][j])
						{
							flag = 1;
							this->gameMapVec[i][j] += this->gameMapVec[k][j];
							this->gameMapVec[k][j] = 0;
							this->score += this->gameMapVec[i][j];//记录得分
						}
						k = -1;
						break;
					}
					k--;
				}
			}
		}
	}
	for (int i = GRID_NUM - 1; i > 0; i--)
	{
		for (int j = 0; j < GRID_NUM; j++)
		{
			int curKey = this->gameMapVec[i][j];
			if (curKey == 0)
			{
				int k = i - 1;
				while (k >= 0)
				{
					int curKeyNext = this->gameMapVec[k][j];
					if (curKeyNext != 0)
					{
						flag = 1;
						this->gameMapVec[i][j] = curKeyNext;
						this->gameMapVec[k][j] = 0;
						k = -1;
					}
					k--;
				}
			}
		}
	}
	if (flag)
	{
		return 0;
	}
	else
	{
		return 4;
	}
}

// 按键响应
void Model::keyDown()
{
	// 判断有无按键消息
	while (_kbhit())
	{
		// 用于接收按键消息
		char key = _getch();
		switch (key)
		{
		case 'W':
		case 'w':
		case 72: // 上方向的小键盘
			this->moveUp();
			this->randIntNum();
			break;
		case 'S':
		case 's':
		case 80: // 下方向的小键盘
			this->moveDown();
			this->randIntNum();
			break;
		case 'A':
		case 'a':
		case 75: // 左方向的小键盘
			this->moveLeft();
			this->randIntNum();
			break;
		case 'D':
		case 'd':
		case 77: // 右方向的小键盘
			this->moveRight();
			this->randIntNum();
			break;
		}
	}
}
