#include "ResultView.h"
#include <iostream>
#include <cstdlib>

void ResultView::ShowEnd()
{
    system("cls");

    std::cout << "=== END ===\n";
    std::cout << "END画面です。任意キーでタイトルに戻る。Esc で終了します。\n";
}

void ResultView::ShowVictory()
{
    system("cls");

    std::cout << "=== VICTORY ===\n";
    std::cout << "敵を全て倒した！\n";
    std::cout << "任意キーで続行します。\n";
}

void ResultView::ShowDefeat()
{
    system("cls");

    std::cout << "=== DEFEAT ===\n";
    std::cout << "プレイヤーは全滅した...\n";
    std::cout << "任意キーでタイトルに戻ります。\n";
}
void ResultView::ShowReturnToTitle()
{
    std::cout << "任意キーでタイトルへ戻ります。\n";
}

void ResultView::ShowExit()
{
    system("cls");
    std::cout << "終了します\n";
}