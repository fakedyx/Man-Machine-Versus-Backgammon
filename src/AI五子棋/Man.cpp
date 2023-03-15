#include "Man.h"
#include <iostream>
using namespace std;

void Man::init(Chess* chess)
{
	this->chess = chess;
}

void Man::go()
{
	MOUSEMSG msg;
	ChessPos pos;

	while (true) {
		//获取鼠标点击消息
		msg = GetMouseMsg();
		//通过棋盘对象chess 判断落子是否有效
		if (msg.uMsg == WM_LBUTTONDOWN && chess->clickBoard(msg.x, msg.y, &pos)) {
			break;
		}
	}

	//cout << pos.row << pos.col;
	chess->chessDown(&pos, CHESS_BLACK);

}
