#pragma once

#include <random>

class BattleController
{
public:
    // í“¬Œ‹‰Ê
    enum BATTLE_RESULT
    {
        VICTORY,  // Ÿ—˜
        DEFEAT,   // ”s–k
        QUIT      // I—¹
    };

    // í“¬ŠJn
    static BATTLE_RESULT StartBattle(std::mt19937& rng);
};
