#include "GameManager.h"
#include <conio.h>
#include <cstdlib>
#include <random>

#include "ResultView.h"
#include "ExploreController.h"
#include "TitleView.h"

void GameManager::StartGame() {
	// タイトル
	state = GAME_STATE::OP;
	bool running = true;

	//  ENDでescapeが押されるまで
	while (running) {
		// OP
		if (state == GAME_STATE::OP) {
			// 画面クリア
			TitleView::ShowTitle();
			//  キー待ち
			int ch = _getch();
			// 任意キーで PLAY へ
			state = GAME_STATE::PLAY;
		}

		// PLAY 
		else if (state == GAME_STATE::PLAY) {

			// 乱数
			std::random_device rd;
			std::mt19937 mt(rd());

			// 探索開始
			ExploreController::EXPLORE_RESULT result =
				ExploreController::StartExplore(mt);

			// 探索結果による状態変更
			if (result == ExploreController::QUIT) {
				state = GAME_STATE::END;
			}
			else if (result == ExploreController::DEFEAT) {
				state = GAME_STATE::OP;
			}
		}

	
		// END 
		else if (state == GAME_STATE::END) {
			//  画面クリア
			ResultView::ShowEnd();
			//  キー待ち
			int ch = _getch();
			//  escapeで終了
			if (ch == 27) {
				//  ループ終了
				running = false;
				break;
			}
			// 任意キーでタイトルへ戻す
			state = GAME_STATE::OP;
		}
	}
	//  画面クリア
	ResultView::ShowExit();
}

int main() {
	GameManager::Instance().StartGame();
	return 0;
}