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

	void Reset() { /* data ‚ğ‰Šú‰»‚µ‚½‚¯‚ê‚Î‚±‚±‚Å */ }

public:
	EnemyData data;
};

