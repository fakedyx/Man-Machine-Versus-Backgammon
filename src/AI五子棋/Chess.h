#pragma once
#include <graphics.h>
#include <vector>

using namespace std;

//落子位置
struct ChessPos {
	int row;
	int col;
	ChessPos(int r = 0, int c = 0) : row(r), col(c) {}
};

//黑白
typedef enum {
	CHESS_WHITE = -1,
	CHESS_BLACK = 1
}chess_kind_t;

class Chess
{
public:
	Chess(int boardSize, int margin_x, int margin_y, float chessSize);
	void init();
	bool clickBoard(int x, int y, ChessPos* pos);
	void chessDown(ChessPos* pos, chess_kind_t kind);
	int getBoardSize();  //获取大小

	int getChessData(ChessPos* pos);
	int getChessData(int row, int col);

	bool checkOver();    //检查是否结束


private:
	IMAGE chessBlackImg;
	IMAGE chessWhiteImg;

	int boardSize;  //多少条线
	int margin_x;  //左边界
	int margin_y; //上边界
	ChessPos lastPos;

	float chessSize; //棋子大小(棋盘小方格大小)

	vector<vector<int>> chessMap; //棋盘数据 0--空白
	bool playerFlag;  //true--该黑棋下  false--白棋下
	void updateGameMap(ChessPos* pos);
	bool checkWin();
};

