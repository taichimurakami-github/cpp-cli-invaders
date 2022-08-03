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
//#include "Enemy.h"
//#include "PoolAllocator.h"

class Game
{
public:
	Game() {
		//初期設定の適用
		_game_state = GState::TITLE;

		//フィールドクラスの保持
		C_Field = new Field();

		//敵データの生成と初期化
		C_EnemyController = new EnemyController();

		//プレーヤーの生成と初期化
		C_Player = new Player(C_Field->GetFieldHeight() - 1, C_Field->GetFieldWidth() / 2);

	}
	~Game() {}

	static enum GState {
		TITLE,
		PLAYING,
		GAMEOVER,
		MAX
	};

	void SetGState(GState s) {
		_game_state = s;
	}

	void PlayGame() {

		C_EnemyController->Init(C_Field);
		C_Player->Init(C_Field);

		clock_t lastClock = clock();
		clock_t lastClockForInvaderInterval = lastClock;
		clock_t lastClockForInvaderBulletInterval = lastClock;

		int update_interval = 1000 / _game_fps;

		while (_game_state == GState::PLAYING) {
			clock_t nowClock = clock();

			clock_t clockDiff = nowClock - lastClock;
			clock_t clockDiffForInvaderInterval = nowClock - lastClockForInvaderInterval;
			clock_t clockDiffForInvaderBulletinterval = nowClock - lastClockForInvaderBulletInterval;

			if (clockDiff >= update_interval) {
				lastClock = nowClock;

				if (_kbhit()) {


					switch (_getch()) {
					case 'a': {
						C_Player->MoveLeft(C_Field);
						break;
					}
					case 'd': {
						C_Player->MoveRight(C_Field);
						break;
					}
					}

				}

				//プレーヤーの弾を移動させる

				//インベーダー移動処理
				if (clockDiffForInvaderInterval >= 1000 / 2) {
					lastClockForInvaderInterval = nowClock;
					C_EnemyController->MoveAllEnemies(C_Field);
					if (C_EnemyController->GetIsEnemyOnTheBottom()) {
						//インベーダーが下に到達していたらゲームオーバー
						SetGState(GState::GAMEOVER);
						break;
					}
				}



				//インベーダーの弾の発射処理


				//canvas更新
				//C_EnemyController->SetEnemiesIntoField(C_Field);
				C_Field->Draw();


				//インベーダーの弾が当たったらゲームオーバー！！

			}
		}
	}


private:
	static const int _game_fps = 60;

	GState _game_state;

	Field* C_Field;
	EnemyController* C_EnemyController;
	Player* C_Player;
};