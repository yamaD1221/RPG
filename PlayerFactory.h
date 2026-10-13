#pragma once
#include "PlayerData.h"

class Player;

class PlayerFactory
{
	static const PlayerData PlayerTable[];

	static const int PlayerTableSize;
	
public:

	static Player* CreatePlayer(int ID);


	static const PlayerData* GetPlayerData(int ID);
};

