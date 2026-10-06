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

void GameManager::StartGame() {
	// 初期はタイトル
	state = GAME_STATE::OP;
	bool running = true;

	//  ENDでescapeが押されるまでループ
	while (running) {
		// OP
		if (state == GAME_STATE::OP) {
			// 画面クリア
			system("cls");
			std::cout << "=== TITLE ===\n";
			std::cout << "何かキーを押すとゲームを開始します\n";
			//  キー待ち
			int ch = _getch();
			// 任意キーで PLAY へ
			state = GAME_STATE::PLAY;
		}

		// PLAY
		else if (state == GAME_STATE::PLAY) {
			// 画面クリア
			system("cls");
			std::cout << "=== PLAY ===\n";
			std::cout << "戦闘を開始します。'q' で END へ\n";

			// --- オブジェクトプールを用意 ---
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

			// --- 敵2体をランダム生成（プール使用） ---
			ObjectPool<Enemy> enemyPool(2);
			std::vector<PoolHandle<Enemy>> enemies;
			enemies.reserve(2);

			std::random_device rd;
			std::mt19937 mt(rd());
			std::uniform_int_distribution<int> enemyIdDist(1, 4); // EnemyFactory は ID 1..4 を想定

			for (int i = 0; i < 2; ++i) {
				int id = enemyIdDist(mt);
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

			//  ループ内かつ PLAY 状態の間（戦闘ループ）
			while (running && state == GAME_STATE::PLAY && anyPlayerAlive() && anyEnemyAlive()) {
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
					int targetIdx = aliveEnemyIdx[pickEnemy(mt)];
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
					int targetIdx = alivePlayerIdx[pickPlayer(mt)];
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
			}
			else if (state == GAME_STATE::END) {
				// 既に END に遷移した場合はそのまま
			}

			// 戦闘後はタイトルへ戻す（ENDにしていなければ）
			if (state != GAME_STATE::END) {
				std::cout << "任意キーでタイトルへ戻ります。\n";
				_getch();
				state = GAME_STATE::OP;
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