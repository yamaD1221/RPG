#include "EnemyFactory.h"
#include "Enemy.h"

const EnemyData EnemyFactory::EnemyTable[] =
{
	{1,"Slime",10,4,2,1,10,5,1},
	{2,"Goblin",20,10,5,2,20,10,5},
	{3,"Orc",30,10,8,3,30,15,20},
	{4,"Dragon",60,15,20,5,1000,1000,50}
};

const int EnemyFactory::EnemyTableSize = sizeof(EnemyTable) / sizeof(EnemyData);

Enemy* EnemyFactory::CreateEnemy(int ID)
{
	for (int i = 0; i < EnemyTableSize; ++i)
	{
		if (EnemyTable[i].ID == ID)
		{
			return new Enemy(EnemyTable[i]);
		}
	}
	return nullptr;
}

const EnemyData* EnemyFactory::GetEnemyData(int ID)
{
	for (int i = 0; i < EnemyTableSize; ++i)
	{
		if (EnemyTable[i].ID == ID)
			return &EnemyTable[i];
	}
	return nullptr;
}