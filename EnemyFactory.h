#pragma once
#include "EnemyData.h"

class Enemy;

class EnemyFactory
{
	static const EnemyData EnemyTable[];

	static const int EnemyTableSize;

public:

	static Enemy* CreateEnemy(int ID);

	
	static const EnemyData* GetEnemyData(int ID);
};

