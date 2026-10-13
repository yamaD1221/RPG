#include <iostream>
#include "GameManager.h"
#include <conio.h>   // _getch()を使うため
#include <cstdlib>   // system("cls")を使うため
#include <vector>
#include <memory>
#include <random>
#include <algorithm>

#include "Objpool.h"
#include "PlayerFactory.h"
#include "EnemyFactory.h"
#include "Player.h"
#include "Enemy.h"
#include "Map.h"    

void GameManager::StartGame() {
	// タイトル
	state = GAME_STATE::OP;
	bool running = true;

	//  ENDでescapeが押されるまで
	while (running) {
		// OP
		if (state == GAME_STATE::OP) {
			// 画面クリア
			system("cls");
			std::cout << "=== TITLE ===\n";
			std::cout << "RPG start\n";
			//  キー待ち
			int ch = _getch();
			// 任意キーで PLAY へ
			state = GAME_STATE::PLAY;
		}

		// PLAY 
		else if (state == GAME_STATE::PLAY) {
			// 画面クリア
			system("cls");
			std::cout << "=== PLAY (探索モード) ===\n";
			std::cout << "上下キーで移動。ランダムで戦闘が発生します。'q' で END へ\n";

			// プレイヤー開始位置（自由に変更可）
			int playerRow = 1;
			int playerCol = 1;

			// 乱数
			std::random_device rd;
			std::mt19937 mt(rd());
			std::uniform_int_distribution<int> encounterDist(0, 9); // 0 のとき戦闘（10%）


			auto runBattle = [&](std::mt19937& rng) {

				ObjectPool<Player> playerPool(4);
				std::vector<PoolHandle<Player>> players;
				players.reserve(4);

				// 味方4名をプールから取得してデータを設定
				for (int id = 1; id <= 4; ++id) {
					auto handle = playerPool.Acquire();
					const PlayerData* pd = PlayerFactory::GetPlayerData(id);
					if (pd) {
						handle->data = *pd;
					}
					players.push_back(std::move(handle));
				}

				//  敵2体をランダム生成
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

				auto anyPlayerAlive = [&players]() {
					for (const auto& p : players) if (p->data.HP > 0) return true;
					return false;
					};
				auto anyEnemyAlive = [&enemies]() {
					for (const auto& e : enemies) if (e->data.HP > 0) return true;
					return false;
					};

				auto printStatus = [&]() {
					system("cls");
					std::cout << "=== BATTLE ===\n";
					std::cout << "----- Players -----\n";
					for (size_t i = 0; i < players.size(); ++i) {
						auto& d = players[i]->data;
						std::cout << i << ": " << d.Name << "  HP:" << d.HP << " ATK:" << d.ATK << " DEF:" << d.DEF << "\n";
					}
					std::cout << "----- Enemies -----\n";
					for (size_t i = 0; i < enemies.size(); ++i) {
						auto& d = enemies[i]->data;
						std::cout << i << ": " << d.Name << "  HP:" << d.HP << " ATK:" << d.ATK << " DEF:" << d.DEF << "\n";
					}
					std::cout << "-------------------\n";
					std::cout << "続行するには任意キーを押してください（'q' で戦闘中断して END へ）\n";
					};

				//  バトルループ
				while (state == GAME_STATE::PLAY && anyPlayerAlive() && anyEnemyAlive()) {
					printStatus();
					int ch = _getch();
					if (ch == 'q') { // ENDへ
						state = GAME_STATE::END;
						break;
					}

					// 味方ターン（4人）
					for (auto& p : players) {
						if (p->data.HP <= 0) continue; // 死亡キャラはスキップ
						// 生存するランダムな敵を選択
						std::vector<int> aliveEnemyIdx;
						for (size_t i = 0; i < enemies.size(); ++i) if (enemies[i]->data.HP > 0) aliveEnemyIdx.push_back((int)i);
						if (aliveEnemyIdx.empty()) break;

						std::uniform_int_distribution<int> pickEnemy(0, (int)aliveEnemyIdx.size() - 1);
						int targetIdx = aliveEnemyIdx[pickEnemy(rng)];
						Enemy& target = *enemies[targetIdx];

						int dmg = p->data.ATK - target.data.DEF;
						if (dmg < 1) dmg = 1;
						target.data.HP -= dmg;
						std::cout << p->data.Name << " の攻撃！ " << target.data.Name << " に " << dmg << " のダメージ\n";
						if (target.data.HP <= 0) {
							std::cout << target.data.Name << " を倒した！\n";
						}
					}

					// 敵が全滅していればプレイヤーの勝利
					if (!anyEnemyAlive()) break;

					// 敵ターン
					for (auto& e : enemies) {
						if (e->data.HP <= 0) continue;
						// 生存するランダムなプレイヤーを選択
						std::vector<int> alivePlayerIdx;
						for (size_t i = 0; i < players.size(); ++i) if (players[i]->data.HP > 0) alivePlayerIdx.push_back((int)i);
						if (alivePlayerIdx.empty()) break;

						std::uniform_int_distribution<int> pickPlayer(0, (int)alivePlayerIdx.size() - 1);
						int targetIdx = alivePlayerIdx[pickPlayer(rng)];
						Player& target = *players[targetIdx];

						int dmg = e->data.ATK - target.data.DEF;
						if (dmg < 1) dmg = 1;
						target.data.HP -= dmg;
						std::cout << e->data.Name << " の攻撃！ " << target.data.Name << " に " << dmg << " のダメージ\n";
						if (target.data.HP <= 0) {
							std::cout << target.data.Name << " は倒れた...\n";
						}
					}

					// ターン終了後はキー待ちで次ターンへ
					std::cout << "ターン終了。続行するには任意キーを押してください...\n";
					int dummy = _getch();
					(void)dummy;
				}

				// 戦闘結果表示
				if (!anyEnemyAlive() && anyPlayerAlive()) {
					system("cls");
					std::cout << "勝利！ 敵を全滅させた。\n";
				}
				else if (!anyPlayerAlive()) {
					system("cls");
					std::cout << "敗北... パーティが全滅した。\n";
					// 敗北したら探索に戻す
					state = GAME_STATE::OP;
				}
				else if (state == GAME_STATE::END) {

				}

				// 戦闘後は任意キーで続行
				if (state != GAME_STATE::END) {
					std::cout << "任意キーで探索へ戻ります。\n";
					_getch();
				}
				};

			// 探索ループ
			while (running && state == GAME_STATE::PLAY) {
				// マップ表示（プレイヤー位置を '@' で表示）
				system("cls");
				std::cout << "=== 探索中 ===\n";
				for (int r = 0; r < MAP_ROWS; ++r) {
					for (int c = 0; c < MAP_COLS; ++c) {
						if (r == playerRow && c == playerCol) {
							std::cout << '@';
						}
						else {
							// Map::GetTile でタイルを取得して表示
							auto t = Map::GetTile(r, c);
							std::cout << ((t == MapData::Tile) ? '.' : '#');
						}
					}
					std::cout << '\n';
				}

				std::cout << "上下キー（または W/S）で移動。'q' で END。移動でランダム遭遇あり。\n";

				int ch = _getch();

				int newRow = playerRow;
				int newCol = playerCol;

				// 矢印キー
				if (ch == 0 || ch == 224) {
					int code = _getch();
					if (code == 72) { // 上
						newRow = playerRow - 1;
					}
					else if (code == 80) { // 下
						newRow = playerRow + 1;
					}
					else if (code == 75) { // 左
						newCol = playerCol - 1;
					}
					else if (code == 77) { // 右
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
						state = GAME_STATE::END;
						break;
					}
				}

				// 移動先が通れるか判定
				if (!Map::IsBlocked(newRow, newCol)) {
					playerRow = newRow;
					playerCol = newCol;

					// 移動後、確率で戦闘発生
					if (encounterDist(mt) == 0) {
						// 敵と遭遇したことだけを表示（画面はこのメッセージのみ）
						system("cls");
						std::cout << "敵と遭遇した！\n";
						// 任意キーで戦闘画面へ
						_getch();

						// 戦闘開始
						runBattle(mt);
						// バトル内で END にした場合
						if (state == GAME_STATE::END) break;
						// 敗北でタイトルに戻る場合
						if (state == GAME_STATE::OP) break;
					}
				}
				else {
					// 移動できない場合はメッセージ
					std::cout << "そこには行けない。\n";
					_getch();
				}
			}

			// 探索から戻ってきた処理
			if (state == GAME_STATE::END) {
				//  画面クリア
				system("cls");
				std::cout << "=== END ===\n";
				std::cout << "END画面です。任意キーでタイトルに戻る。Esc で終了します。\n";
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
			else if (state == GAME_STATE::OP) {
				// タイトルへ戻る（戦闘で敗北した場合など）
				std::cout << "任意キーでタイトルへ戻ります。\n";
				_getch();
				// stateはすでに OP
			}
		}

		// END 
		else if (state == GAME_STATE::END) {
			//  画面クリア
			system("cls");
			std::cout << "=== END ===\n";
			std::cout << "END画面です。任意キーでタイトルに戻る。Esc で終了します。\n";
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
	system("cls");
	std::cout << "終了します\n";
}

int main() {
	GameManager::Instance().StartGame();
	return 0;
}