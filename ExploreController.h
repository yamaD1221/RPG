#pragma once

#include <random>

class ExploreController
{
public:
    // 探索結果
    enum EXPLORE_RESULT
    {
        CONTINUE, // 通常の探索終了ではない
        QUIT,     // ゲーム終了
        DEFEAT    // 戦闘で敗北
    };

    // 探索開始
    static EXPLORE_RESULT StartExplore(std::mt19937& rng);
};
