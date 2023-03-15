#include "Chess.h"
#include <cmath>
#include <mmsystem.h>
#include <conio.h>
#pragma comment(lib, "winmm.lib")  //音乐播放

void putimagePNG(int x, int y, IMAGE* picture) //x为载入图片的X坐标，y为Y坐标
{
	// 变量初始化
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
	int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
	int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
	int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
	int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
	int dstX = 0;    //在显存里像素的角标

	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + x) + (iy + y) * graphWidth; //在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}

Chess::Chess(int boardSize, int margin_x, int margin_y, float chessSize)
{
	this->boardSize = boardSize;
	this->margin_x = margin_x;
	this->margin_y = margin_y;
	this->chessSize = chessSize;
	playerFlag = CHESS_BLACK;
	
	//初始化ChessMap
	for (int i = 0; i < boardSize; i++) {
		vector<int> row;
		for (int j = 0; j < boardSize; j++) {
			row.push_back(0);
		}
		chessMap.push_back(row);
	}
}

void Chess::init()
{
	//创建游戏窗口
	//initgraph(897, 895, 1);
	initgraph(897, 895);
	//显示棋盘图片
	loadimage(0, L"res/棋盘2.jpg");
	//播放开始提示音
	mciSendString(L"play res/start.wav", 0, 0, 0);

	//加载黑白棋子图片
	loadimage(&chessBlackImg, L"res/black.png", chessSize, chessSize, true);
	loadimage(&chessWhiteImg, L"res/white.png", chessSize, chessSize, true);
	
	//棋盘清理
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			chessMap[i][j] = 0;
		}
	}
}


bool Chess::clickBoard(int x, int y, ChessPos* pos)
{
	int col = (x - margin_x) / chessSize; //左上角列
	int row = (y - margin_y) / chessSize; //左上角行
	int leftTopPosX = margin_x + chessSize * col;
	int leftTopPosY = margin_y + chessSize * row;
	int offset = chessSize * 0.4; //阈值
	bool ret = false;

	int len;
	
	do {
		/*左上*/
		len = sqrt((x - leftTopPosX) * (x - leftTopPosX) +
			(y - leftTopPosY) * (y - leftTopPosY));
		if (len < offset)
		{
			pos->col = col;
			pos->row = row;
			if (chessMap[pos->row][pos->col] == 0)
			{
				ret = true;
				break;
			}
		}

		/*右上*/
		int x2 = leftTopPosX + chessSize - x;
		int y2 = y - leftTopPosY;
		len = sqrt(x2 * x2 + y2 * y2);
		if (len < offset)
		{
			pos->col = col + 1;
			pos->row = row;
			if (chessMap[pos->row][pos->col] == 0)
			{
				ret = true;
				break;
			}
		}

		/*左下*/
		int x3 = x - leftTopPosX;
		int y3 = leftTopPosY + chessSize - y;
		len = sqrt(x3 * x3 + y3 * y3);
		if (len < offset)
		{
			pos->col = col;
			pos->row = row + 1;
			if (chessMap[pos->row][pos->col] == 0)
			{
				ret = true;
				break;
			}
		}

		/*右下*/
		int x4 = leftTopPosX + chessSize - x;
		int y4 = leftTopPosY + chessSize - y;
		len = sqrt(x4 * x4 + y4 * y4);
		if (len < offset)
		{
			pos->col = col + 1;
			pos->row = row + 1;
			if (chessMap[pos->row][pos->col] == 0)
			{
				ret = true;
				break;
			}
		}
	} while (0);

	return ret;
}

void Chess::chessDown(ChessPos* pos, chess_kind_t kind)
{
	mciSendString(L"play res/down7.wav", 0, 0, 0);
	int x = margin_x + chessSize * pos->col - chessSize * 0.5;
	int y = margin_y + chessSize * pos->row - chessSize * 0.5;
	
	if (kind == CHESS_WHITE) {
		putimagePNG(x, y, &chessWhiteImg);
	}
	else {
		putimagePNG(x, y, &chessBlackImg);
	}
	updateGameMap(pos);
}

int Chess::getBoardSize()
{
	return boardSize;
}

bool Chess::checkOver()
{
	if (checkWin()) {
		Sleep(1500);
		if (playerFlag == false) {  //黑棋赢（玩家赢）,此时标记已经反转，轮到白棋落子
			mciSendString(L"play res/不错.mp3", 0, 0, 0);
			loadimage(0, L"res/胜利.jpg");
		}
		else {
			mciSendString(L"play res/失败.mp3", 0, 0, 0);
			loadimage(0, L"res/失败.jpg");
		}

		_getch(); // 补充头文件 #include <conio.h>
		return true;
	}

	return false;
}

void Chess::updateGameMap(ChessPos* pos)
{
	lastPos = *pos;
	chessMap[pos->row][pos->col] = (playerFlag ? CHESS_BLACK : CHESS_WHITE);
	playerFlag = !playerFlag; //黑白交换
}

bool Chess::checkWin()
{
	// 横竖斜四种大情况，每种情况都根据当前落子往后遍历5个棋子，有一种符合就算赢
	// 水平方向
	int row = lastPos.row;
	int col = lastPos.col;

	for (int i = 0; i < 5; i++)
	{
		// 往左5个，往右匹配4个子，20种情况
		if (col - i >= 0 &&
			col - i + 4 < boardSize &&
			chessMap[row][col - i] == chessMap[row][col - i + 1] &&
			chessMap[row][col - i] == chessMap[row][col - i + 2] &&
			chessMap[row][col - i] == chessMap[row][col - i + 3] &&
			chessMap[row][col - i] == chessMap[row][col - i + 4])
			return true;
	}

	// 竖直方向(上下延伸4个)
	for (int i = 0; i < 5; i++)
	{
		if (row - i >= 0 &&
			row - i + 4 < boardSize &&
			chessMap[row - i][col] == chessMap[row - i + 1][col] &&
			chessMap[row - i][col] == chessMap[row - i + 2][col] &&
			chessMap[row - i][col] == chessMap[row - i + 3][col] &&
			chessMap[row - i][col] == chessMap[row - i + 4][col])
			return true;
	}

	// “/"方向
	for (int i = 0; i < 5; i++)
	{
		if (row + i < boardSize &&
			row + i - 4 >= 0 &&
			col - i >= 0 &&
			col - i + 4 < boardSize &&
			// 第[row+i]行，第[col-i]的棋子，与右上方连续4个棋子都相同
			chessMap[row + i][col - i] == chessMap[row + i - 1][col - i + 1] &&
			chessMap[row + i][col - i] == chessMap[row + i - 2][col - i + 2] &&
			chessMap[row + i][col - i] == chessMap[row + i - 3][col - i + 3] &&
			chessMap[row + i][col - i] == chessMap[row + i - 4][col - i + 4])
			return true;
	}

	// “\“ 方向
	for (int i = 0; i < 5; i++)
	{
		// 第[row+i]行，第[col-i]的棋子，与右下方连续4个棋子都相同
		if (row - i >= 0 &&
			row - i + 4 < boardSize &&
			col - i >= 0 &&
			col - i + 4 < boardSize &&
			chessMap[row - i][col - i] == chessMap[row - i + 1][col - i + 1] &&
			chessMap[row - i][col - i] == chessMap[row - i + 2][col - i + 2] &&
			chessMap[row - i][col - i] == chessMap[row - i + 3][col - i + 3] &&
			chessMap[row - i][col - i] == chessMap[row - i + 4][col - i + 4])
			return true;
	}

	return false;
}

int Chess::getChessData(ChessPos* pos)
{
	return chessMap[pos->row][pos->col];
}

int Chess::getChessData(int row, int col)
{
	return chessMap[row][col];
}
