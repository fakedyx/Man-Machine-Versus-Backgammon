#include "Chess.h"
#include <cmath>
#include <mmsystem.h>
#include <conio.h>
#pragma comment(lib, "winmm.lib")  //���ֲ���

void putimagePNG(int x, int y, IMAGE* picture) //xΪ����ͼƬ��X���꣬yΪY����
{
	// ������ʼ��
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()���������ڻ�ȡ��ͼ�豸���Դ�ָ�룬EASYX�Դ�
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //��ȡpicture���Դ�ָ��
	int picture_width = picture->getwidth(); //��ȡpicture�Ŀ�ȣ�EASYX�Դ�
	int picture_height = picture->getheight(); //��ȡpicture�ĸ߶ȣ�EASYX�Դ�
	int graphWidth = getwidth();       //��ȡ��ͼ���Ŀ�ȣ�EASYX�Դ�
	int graphHeight = getheight();     //��ȡ��ͼ���ĸ߶ȣ�EASYX�Դ�
	int dstX = 0;    //���Դ������صĽǱ�

	// ʵ��͸����ͼ ��ʽ�� Cp=��p*FP+(1-��p)*BP �� ��Ҷ˹���������е���ɫ�ĸ��ʼ���
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //���Դ������صĽǱ�
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA��͸����
			int sr = ((src[srcX] & 0xff0000) >> 16); //��ȡRGB���R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + x) + (iy + y) * graphWidth; //���Դ������صĽǱ�
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //��ʽ�� Cp=��p*FP+(1-��p)*BP  �� ��p=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //��p=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //��p=sa/255 , FP=sb , BP=db
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
	
	//��ʼ��ChessMap
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
	//������Ϸ����
	//initgraph(897, 895, 1);
	initgraph(897, 895);
	//��ʾ����ͼƬ
	loadimage(0, L"res/����2.jpg");
	//���ſ�ʼ��ʾ��
	mciSendString(L"play res/start.wav", 0, 0, 0);

	//���غڰ�����ͼƬ
	loadimage(&chessBlackImg, L"res/black.png", chessSize, chessSize, true);
	loadimage(&chessWhiteImg, L"res/white.png", chessSize, chessSize, true);
	
	//��������
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			chessMap[i][j] = 0;
		}
	}
}


bool Chess::clickBoard(int x, int y, ChessPos* pos)
{
	int col = (x - margin_x) / chessSize; //���Ͻ���
	int row = (y - margin_y) / chessSize; //���Ͻ���
	int leftTopPosX = margin_x + chessSize * col;
	int leftTopPosY = margin_y + chessSize * row;
	int offset = chessSize * 0.4; //��ֵ
	bool ret = false;

	int len;
	
	do {
		/*����*/
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

		/*����*/
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

		/*����*/
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

		/*����*/
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
		if (playerFlag == false) {  //����Ӯ�����Ӯ��,��ʱ����Ѿ���ת���ֵ���������
			mciSendString(L"play res/����.mp3", 0, 0, 0);
			loadimage(0, L"res/ʤ��.jpg");
		}
		else {
			mciSendString(L"play res/ʧ��.mp3", 0, 0, 0);
			loadimage(0, L"res/ʧ��.jpg");
		}

		_getch(); // ����ͷ�ļ� #include <conio.h>
		return true;
	}

	return false;
}

void Chess::updateGameMap(ChessPos* pos)
{
	lastPos = *pos;
	chessMap[pos->row][pos->col] = (playerFlag ? CHESS_BLACK : CHESS_WHITE);
	playerFlag = !playerFlag; //�ڰ׽���
}

bool Chess::checkWin()
{
	// ����б���ִ������ÿ����������ݵ�ǰ�����������5�����ӣ���һ�ַ��Ͼ���Ӯ
	// ˮƽ����
	int row = lastPos.row;
	int col = lastPos.col;

	for (int i = 0; i < 5; i++)
	{
		// ����5��������ƥ��4���ӣ�20�����
		if (col - i >= 0 &&
			col - i + 4 < boardSize &&
			chessMap[row][col - i] == chessMap[row][col - i + 1] &&
			chessMap[row][col - i] == chessMap[row][col - i + 2] &&
			chessMap[row][col - i] == chessMap[row][col - i + 3] &&
			chessMap[row][col - i] == chessMap[row][col - i + 4])
			return true;
	}

	// ��ֱ����(��������4��)
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

	// ��/"����
	for (int i = 0; i < 5; i++)
	{
		if (row + i < boardSize &&
			row + i - 4 >= 0 &&
			col - i >= 0 &&
			col - i + 4 < boardSize &&
			// ��[row+i]�У���[col-i]�����ӣ������Ϸ�����4�����Ӷ���ͬ
			chessMap[row + i][col - i] == chessMap[row + i - 1][col - i + 1] &&
			chessMap[row + i][col - i] == chessMap[row + i - 2][col - i + 2] &&
			chessMap[row + i][col - i] == chessMap[row + i - 3][col - i + 3] &&
			chessMap[row + i][col - i] == chessMap[row + i - 4][col - i + 4])
			return true;
	}

	// ��\�� ����
	for (int i = 0; i < 5; i++)
	{
		// ��[row+i]�У���[col-i]�����ӣ������·�����4�����Ӷ���ͬ
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
