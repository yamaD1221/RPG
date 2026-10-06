#pragma once
#include "PlayerData.h"

class Player;

class PlayerFactory
{
	static const PlayerData PlayerTable[];

	static const int PlayerTableSize;
	
public:

	static Player* CreatePlayer(int ID);

	// 追加: テーブルからデータを参照するための関数
	static const PlayerData* GetPlayerData(int ID);
};

