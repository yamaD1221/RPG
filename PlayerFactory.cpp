#include "PlayerFactory.h"
#include "Player.h"

const PlayerData PlayerFactory::PlayerTable[] =
{
	{1,"Swordsman",40,10,10,5,30},
	{2,"Archer",20,9,8,6,40},
	{3,"Mage",15,15,6,8,10},
	{4,"Healer",22,6,8,10,10}

};

const int PlayerFactory::PlayerTableSize = sizeof(PlayerTable) / sizeof(PlayerData);

Player* PlayerFactory::CreatePlayer(int ID)
{
	for (int i = 0; i < PlayerTableSize; ++i)
	{
		if (PlayerTable[i].ID == ID)
		{
			return new Player(PlayerTable[i]);
		}
	}
	return nullptr;
}

const PlayerData* PlayerFactory::GetPlayerData(int ID)
{
	for (int i = 0; i < PlayerTableSize; ++i)
	{
		if (PlayerTable[i].ID == ID)
			return &PlayerTable[i];
	}
	return nullptr;
}