/*
	开发日志:
	1.创建项目、导入素材
	2.设计项目的框架(设计类)
	3.每个类对外提供的功能
	4.对内
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