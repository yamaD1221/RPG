#include "MapView.h"
#include "Map.h"
#include <iostream>
#include <cstdlib>

void MapView::Draw(int playerRow, int playerCol)
{
    system("cls");

    std::cout << "=== 探索中 ===\n";

    for (int r = 0; r < MAP_ROWS; ++r)
    {
        for (int c = 0; c < MAP_COLS; ++c)
        {
            if (r == playerRow && c == playerCol)
            {
                std::cout << '@';
            }
            else
            {
                auto t = Map::GetTile(r, c);
                std::cout << ((t == MapData::Tile) ? '.' : '#');
            }
        }

        std::cout << '\n';
    }

    std::cout << "上下キー（または W/S）で移動。'q' で END。移動でランダム遭遇あり。\n";
}

void MapView::ShowExploreInfo()
{
    system("cls");
    std::cout << "=== PLAY (探索モード) ===\n";
    std::cout << "上下キーで移動。ランダムで戦闘が発生します。'q' で END へ\n";
}

void MapView::ShowBlockedMessage()
{
    std::cout << "そこには行けない。\n";
}