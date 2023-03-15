#pragma once
#include <graphics.h>
#include <vector>

using namespace std;

//����λ��
struct ChessPos {
	int row;
	int col;
	ChessPos(int r = 0, int c = 0) : row(r), col(c) {}
};

//�ڰ�
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
	int getBoardSize();  //��ȡ��С

	int getChessData(ChessPos* pos);
	int getChessData(int row, int col);

	bool checkOver();    //����Ƿ����


private:
	IMAGE chessBlackImg;
	IMAGE chessWhiteImg;

	int boardSize;  //��������
	int margin_x;  //��߽�
	int margin_y; //�ϱ߽�
	ChessPos lastPos;

	float chessSize; //���Ӵ�С(����С�����С)

	vector<vector<int>> chessMap; //�������� 0--�հ�
	bool playerFlag;  //true--�ú�����  false--������
	void updateGameMap(ChessPos* pos);
	bool checkWin();
};

