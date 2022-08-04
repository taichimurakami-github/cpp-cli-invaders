#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <time.h>
#include <string.h>


#include "Field.h"
#include "EnemyController.h"
#include "Player.h"
#include "EnemyBulletController.h"
#include "CLIOutput.h"
//#include "PoolAllocator.h"

class Game
{
public:
	Game() {
		//フィールドクラスの保持
		C_Field = new Field();

		//敵データの生成と初期化
		C_EnemyController = new EnemyController();

		//プレーヤーの生成と初期化
		C_Player = new Player(C_Field->GetFieldWidth() / 2, C_Field->GetFieldHeight() - 1);

		//敵の弾データの生成と初期化
		C_EnemyBulletController = new EnemyBulletController(C_Field, C_EnemyController);

		C_CLIOutput = new CLIOutput();

		//タイトル画面表示
		SetGState(GState::TITLE);
	}
	~Game() {}


	enum GState {
		TITLE,
		PLAYING,
		GAMEOVER,
		GAMECLEAR,
		MAX
	};

	//リフレッシュ用インターバル一覧
	enum class ENEMY_BULLET_LEVEL {
		LV1,
		LV2,
		LV3,
	};

	enum class ENEMY_LEVEL {
		LV1,
		LV2,
		LV3,
		LV4,
	};

	void SetGState(GState s) {
		_game_state = s;

		switch (s) {
		case GState::TITLE: {
			_ShowTitle();
			break;
		}

		case GState::PLAYING: {
			_PlayGame();
			break;
		}

		case GState::GAMEOVER: {
			_ShowGameOver();
			break;
		}

		case GState::GAMECLEAR: {
			_ShowGameClear();
			break;
		}
		}
	}


private:
	clock_t _lastClockForEnemyInterval;
	clock_t _lastClockForEnemyBulletInterval;
	clock_t _lastClock;
	GState _game_state;
	static const int _GAME_FPS = 60;

	Field* C_Field;
	EnemyController* C_EnemyController;
	Player* C_Player;
	EnemyBulletController* C_EnemyBulletController;
	CLIOutput* C_CLIOutput;

	void _InitGame() {
		//インスタンスの初期化
		C_Field->Init();
		C_EnemyController->Init(C_Field);
		C_Player->Init(C_Field);
		C_EnemyBulletController->Init();
	}

	void _ShowTitle() {
		system("cls");
		C_CLIOutput->DrawTitle();

		while (_game_state == GState::TITLE) {
			//キー入力待ち
			switch (_getch()) {
			case ' ': {
				SetGState(GState::PLAYING);
				break;
			}
			}
		}
	}

	void _ShowGameOver() {
		system("cls");
		C_CLIOutput->DrawGameOver();

		while (_game_state == GState::GAMEOVER) {
			//キー入力待ち
			switch (_getch()) {
			case 'q': {
				SetGState(GState::TITLE);
				break;
			}
			case ' ': {
				SetGState(GState::PLAYING);
				break;
			}
			}
		}
	}

	void _ShowGameClear() {
		system("cls");
		C_CLIOutput->DrawGameClear();

		while (_game_state == GState::GAMECLEAR) {
			//キー入力待ち
			switch (_getch()) {
			case 'q': {
				SetGState(GState::TITLE);
				break;
			}
			case ' ': {
				SetGState(GState::PLAYING);
				break;
			}
			}
		}
	}

	void _PlayGame() {
		int update_interval = 1000 / _GAME_FPS;//60fps固定
		int max_hit_count = C_EnemyController->GetNumOfEnemies();
		const int game_master_interval = 1000 / _GAME_FPS;
		clock_t nowClock = clock();


		//初期化処理
		_InitGame();

		_lastClock = clock();
		_lastClockForEnemyInterval = _lastClock;
		_lastClockForEnemyBulletInterval = _lastClock;


		C_CLIOutput->DrawReady();

		//ゲームループ
		while (_game_state == GState::PLAYING) {
			nowClock = clock();
			clock_t clockDiff = nowClock - _lastClock;
			clock_t clockDiffForInvaderInterval = nowClock - _lastClockForEnemyInterval;
			clock_t clockDiffForInvaderBulletinterval = nowClock - _lastClockForEnemyBulletInterval;

			if (clockDiff >= game_master_interval) {
				_lastClock = nowClock;

				if (_kbhit()) {

					//プレーヤーの機体操作
					switch (_getch()) {
					case 'a': {
						if (C_Player->MoveLeft(C_Field)) {
							//移動先に敵弾があったら被弾
							SetGState(GState::GAMEOVER);
						}
						break;
					}
					case 'd': {
						if (C_Player->MoveRight(C_Field)) {
							//移動先に敵弾があったら被弾
							SetGState(GState::GAMEOVER);
						}
						break;
					}
					case 'q': {
						SetGState(GState::TITLE);
						break;
					}

					default: {
						C_Player->Shoot();
						break;
					}
					}

				}
				//プレーヤーの弾を移動させる&当たり判定
				C_Player->Update(C_Field, C_EnemyController);

				//インベーダー本体関連の処理
				if (clockDiffForInvaderInterval >= C_EnemyController->GetRefreshInterval()) {
					//インベーダー移動処理
					_lastClockForEnemyInterval = nowClock;
					C_EnemyController->Update(C_Field);
					if (C_EnemyController->GetIsEnemyOnTheBottom()) {
						//インベーダーが下に到達していたらゲームオーバー
						SetGState(GState::GAMEOVER);
					}
				}
				//else {
				//	//インベーダー再描画処理
				//	C_EnemyController->Redraw(C_Field);
				//}

				//インベーダーの弾の発射処理
				if (_lastClockForEnemyBulletInterval >= C_EnemyBulletController->GetRefreshInterval()) {
					_lastClockForEnemyBulletInterval = nowClock;
					C_EnemyBulletController->Update();

					if (C_EnemyBulletController->GetIsHit()) {
						//インベーダーの弾がプレーヤーに当たったらゲームオーバー
						SetGState(GState::GAMEOVER);
					}
				}

				//canvas更新
				//C_EnemyController->SetEnemiesIntoField(C_Field);
				C_Field->Draw(C_EnemyController->GetFontColorset());

				//敵を全滅したかどうかの判定
				int now_hit_count = C_Player->GetHitCount();
				if (now_hit_count >= max_hit_count) {
					//ゲームクリア！
					SetGState(GState::GAMECLEAR);
				}

				//レベルアップするかどうかの判定

				if (now_hit_count >= max_hit_count * 0.75) {
					//二段階レベルアップ：全体の敵が元の3割以下に減ったとき
					C_EnemyController->SetEnemiesLevel((int)ENEMY_LEVEL::LV3);
					C_EnemyBulletController->SetEnemyBulletLevel((int)ENEMY_BULLET_LEVEL::LV3);
				}
				else if (now_hit_count >= max_hit_count * 0.4) {
					//一段階レベルアップ：全体の敵が元の5割以下に減ったとき
					C_EnemyController->SetEnemiesLevel((int)ENEMY_LEVEL::LV2);
					C_EnemyBulletController->SetEnemyBulletLevel((int)ENEMY_BULLET_LEVEL::LV2);
				}
			}
		}
	}
};