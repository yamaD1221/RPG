
#include "BattleController.h"

#include <iostream>
#include <vector>
#include <random>
#include <conio.h>

#include "Objpool.h"
#include "PlayerFactory.h"
#include "EnemyFactory.h"
#include "Player.h"
#include "Enemy.h"
#include "BattleView.h"
#include "ResultView.h"

BattleController::BATTLE_RESULT BattleController::StartBattle(std::mt19937& rng)
{
    // 味方4人を生成
    ObjectPool<Player> playerPool(4);
    std::vector<PoolHandle<Player>> players;
    players.reserve(4);

    for (int id = 1; id <= 4; ++id) {
        auto handle = playerPool.Acquire();
        const PlayerData* pd = PlayerFactory::GetPlayerData(id);

        if (pd) {
            handle->data = *pd;
        }

        players.push_back(std::move(handle));
    }

    // 敵2体をランダム生成
    ObjectPool<Enemy> enemyPool(2);
    std::vector<PoolHandle<Enemy>> enemies;
    enemies.reserve(2);

    std::uniform_int_distribution<int> enemyIdDist(1, 4);

    for (int i = 0; i < 2; ++i) {
        int id = enemyIdDist(rng);
        auto handle = enemyPool.Acquire();
        const EnemyData* ed = EnemyFactory::GetEnemyData(id);

        if (ed) {
            handle->data = *ed;
        }

        enemies.push_back(std::move(handle));
    }

    // 味方の生存確認
    auto anyPlayerAlive = [&players]() {
        for (const auto& p : players) {
            if (p->data.HP > 0) return true;
        }
        return false;
        };

    // 敵の生存確認
    auto anyEnemyAlive = [&enemies]() {
        for (const auto& e : enemies) {
            if (e->data.HP > 0) return true;
        }
        return false;
        };

    // 戦闘ループ
    while (anyPlayerAlive() && anyEnemyAlive()) {

        BattleView::PrintStatus(players, enemies);

        int ch = _getch();

        if (ch == 'q') {
            return QUIT;
        }

        // 味方ターン
        for (auto& p : players) {

            if (p->data.HP <= 0) continue;

            std::vector<int> aliveEnemyIdx;

            for (size_t i = 0; i < enemies.size(); ++i) {
                if (enemies[i]->data.HP > 0) {
                    aliveEnemyIdx.push_back((int)i);
                }
            }

            if (aliveEnemyIdx.empty()) break;

            std::uniform_int_distribution<int> pickEnemy(
                0, (int)aliveEnemyIdx.size() - 1
            );

            int targetIdx = aliveEnemyIdx[pickEnemy(rng)];
            Enemy& target = *enemies[targetIdx];

            int dmg = p->data.ATK - target.data.DEF;
            if (dmg < 1) dmg = 1;

            target.TakeDamage(dmg);

            BattleView::ShowAttack(
                p->data.Name, target.data.Name, dmg
            );

            if (!target.IsAlive()) {
                BattleView::ShowDefeat(target.data.Name);
            }
        }

        if (!anyEnemyAlive()) break;

        // 敵ターン
        for (auto& e : enemies) {

            if (e->data.HP <= 0) continue;

            std::vector<int> alivePlayerIdx;

            for (size_t i = 0; i < players.size(); ++i) {
                if (players[i]->data.HP > 0) {
                    alivePlayerIdx.push_back((int)i);
                }
            }

            if (alivePlayerIdx.empty()) break;

            std::uniform_int_distribution<int> pickPlayer(
                0, (int)alivePlayerIdx.size() - 1
            );

            int targetIdx = alivePlayerIdx[pickPlayer(rng)];
            Player& target = *players[targetIdx];

            int dmg = e->data.ATK - target.data.DEF;
            if (dmg < 1) dmg = 1;

            target.TakeDamage(dmg);

            BattleView::ShowAttack(
                e->data.Name, target.data.Name, dmg
            );

            if (!target.IsAlive()) {
                BattleView::ShowDefeat(target.data.Name);
            }
        }

        BattleView::ShowTurnEnd();

        int dummy = _getch();
        (void)dummy;
    }

    // 戦闘結果
    if (!anyEnemyAlive() && anyPlayerAlive()) {
        ResultView::ShowVictory();
        _getch();

        BattleView::ShowReturnToExplore();
        _getch();

        return VICTORY;
    }
    else if (!anyPlayerAlive()) {
        ResultView::ShowDefeat();
        _getch();

        return DEFEAT;
    }

    return QUIT;
}
