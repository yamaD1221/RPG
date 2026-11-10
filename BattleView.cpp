#include "BattleView.h"
#include <iostream>
#include <cstdlib>

void BattleView::PrintStatus(
    const std::vector<PoolHandle<Player>>& players,
    const std::vector<PoolHandle<Enemy>>& enemies)
{
    system("cls");

    std::cout << "=== BATTLE ===\n";

    std::cout << "----- Players -----\n";

    for (size_t i = 0; i < players.size(); ++i)
    {
        auto& d = players[i]->data;

        std::cout << i << ": " << d.Name
            << "  HP:" << d.HP
            << " ATK:" << d.ATK
            << " DEF:" << d.DEF << "\n";
    }

    std::cout << "----- Enemies -----\n";

    for (size_t i = 0; i < enemies.size(); ++i)
    {
        auto& d = enemies[i]->data;

        std::cout << i << ": " << d.Name
            << "  HP:" << d.HP
            << " ATK:" << d.ATK
            << " DEF:" << d.DEF << "\n";
    }

    std::cout << "-------------------\n";
    std::cout << "続行するには任意キーを押してください（'q' で戦闘中断して END へ）\n";
}

void BattleView::ShowEncounter()
{
    system("cls");
    std::cout << "敵と遭遇した！\n";
}

void BattleView::ShowAttack(
    const std::string& attacker,
    const std::string& target,
    int damage)
{
    std::cout << attacker << " の攻撃！ "
        << target << " に " << damage << " のダメージ\n";
}

void BattleView::ShowDefeat(const std::string& name)
{
    std::cout << name << " を倒した！\n";
}

void BattleView::ShowTurnEnd()
{
    std::cout << "ターン終了。続行するには任意キーを押してください...\n";
}

void BattleView::ShowReturnToExplore()
{
    std::cout << "任意キーで探索へ戻ります。\n";
}