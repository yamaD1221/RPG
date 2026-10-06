#pragma once
#include "EnemyData.h"

class Enemy;

class EnemyFactory
{
	static const EnemyData EnemyTable[];

	static const int EnemyTableSize;

public:

	static Enemy* CreateEnemy(int ID);

	// 追加: テーブルからデータを参照するための関数
	static const EnemyData* GetEnemyData(int ID);
};

