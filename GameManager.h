#pragma once
#include <iostream>

class GameManager
{
private:
	GameManager() {}
public:
	GameManager(const GameManager&) = delete;

	GameManager& operator = (const GameManager&) = delete;

	static GameManager& Instance()
	{
		static GameManager instance;

		return instance;
	}

	void StartGame();

private:

	enum GAME_STATE
	{
		OP,
		PLAY,
		END
	};

	GAME_STATE state = OP;
};