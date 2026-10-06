#pragma once
#include "PlayerData.h"

class Player
{
public:
	Player() = default;

	Player(const PlayerData& data)
		: data(data)
	{
	}

	// 必要ならリセット用メソッドを追加可能
	void Reset() { /* data を初期化したければここで */ }

public:
	PlayerData data;
};

