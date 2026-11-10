
#include "ExploreController.h"

#include <conio.h>
#include <random>

#include "Map.h"
#include "MapView.h"
#include "BattleView.h"
#include "BattleController.h"

ExploreController::EXPLORE_RESULT ExploreController::StartExplore(std::mt19937& rng)
{
    // 探索開始時の画面表示
    MapView::ShowExploreInfo();
    _getch();

    // プレイヤー開始位置
    int playerRow = 1;
    int playerCol = 1;

    // エンカウント確率（10%）
    std::uniform_int_distribution<int> encounterDist(0, 9);

    // 探索ループ
    while (true) {

        // マップ表示
        MapView::Draw(playerRow, playerCol);

        int ch = _getch();

        int newRow = playerRow;
        int newCol = playerCol;

        // 矢印キー
        if (ch == 0 || ch == 224) {

            int code = _getch();

            if (code == 72) {
                newRow = playerRow - 1;
            }
            else if (code == 80) {
                newRow = playerRow + 1;
            }
            else if (code == 75) {
                newCol = playerCol - 1;
            }
            else if (code == 77) {
                newCol = playerCol + 1;
            }
        }
        else {

            // WASD / Q
            if (ch == 'w' || ch == 'W') {
                newRow = playerRow - 1;
            }
            else if (ch == 's' || ch == 'S') {
                newRow = playerRow + 1;
            }
            else if (ch == 'a' || ch == 'A') {
                newCol = playerCol - 1;
            }
            else if (ch == 'd' || ch == 'D') {
                newCol = playerCol + 1;
            }
            else if (ch == 'q' || ch == 'Q') {
                return QUIT;
            }
        }

        // 移動先の判定
        if (!Map::IsBlocked(newRow, newCol)) {

            playerRow = newRow;
            playerCol = newCol;

            // エンカウント判定
            if (encounterDist(rng) == 0) {

                BattleView::ShowEncounter();
                _getch();

                // 戦闘開始
                BattleController::BATTLE_RESULT result =
                    BattleController::StartBattle(rng);

                if (result == BattleController::QUIT) {
                    return QUIT;
                }
                else if (result == BattleController::DEFEAT) {
                    return DEFEAT;
                }
            }
        }
        else {

            // 移動できない場合
            MapView::ShowBlockedMessage();
            _getch();
        }
    }
}
