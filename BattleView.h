#pragma once

#include <vector>
#include <string>
#include "Player.h"
#include "Enemy.h"
#include "Objpool.h"

class BattleView
{
public:

    static void PrintStatus(
        const std::vector<PoolHandle<Player>>& players,
        const std::vector<PoolHandle<Enemy>>& enemies
    );

    static void ShowEncounter();
    static void ShowAttack(const std::string& attacker,
        const std::string& target,
        int damage);
    static void ShowDefeat(const std::string& name);
    static void ShowTurnEnd();
    static void ShowReturnToExplore();
};