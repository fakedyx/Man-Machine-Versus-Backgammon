/*
	������־:
	1.������Ŀ�������ز�
	2.�����Ŀ�Ŀ��(�����)
	3.ÿ��������ṩ�Ĺ���
	4.����
*/

#include <iostream>
#include "ChessGame.h"
#include "AI.h"
#include "Chess.h"
#include "Man.h"


int main()
{
	Man man;
	AI ai;
	//Chess chess;
	Chess chess(13, 44, 43, 67.3);
	ChessGame game(&man, &ai, &chess);

	game.play();

	system("pause");
	return 0;
}