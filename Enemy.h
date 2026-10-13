#pragma once
#include "EnemyData.h"

class Enemy
{
public:
	Enemy() = default;

	Enemy(const EnemyData& data)
		: data(data)
	{
	}

	void Reset() {  }

public:
	EnemyData data;
};

