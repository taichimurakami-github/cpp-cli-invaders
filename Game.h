#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <time.h>
#include <string.h>


#include "Field.h"
#include "EnemyController.h"
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
	}
	~Game() {}

	static enum GState {
		TITLE,
		PLAYING,
		GAMEOVER,
		MAX
	};

	void SetState(GState s) {
		_game_state = s;
	}

	void PlayGame() {
		std::cout << "press any key to break" << std::endl;
		clock_t lastClock = clock();

		while (1) {
			clock_t nowClock = clock();

			clock_t clockDiff = nowClock - lastClock;
			if (clockDiff >= 1000 / _FPS) {
				lastClock = nowClock;

				C_Field->ResetFieldState();

				if (_kbhit()) {
					SetState(GState::GAMEOVER);
				}

				//プレーヤーの弾を移動させる

				//インベーダー移動処理


				//インベーダーの弾の発射処理

				//canvas更新
				C_EnemyController->SetEnemyIntoField(C_Field);
				C_Field->Draw();


				//インベーダーの弾が当たったらゲームオーバー！！

			}
		}
	}


private:
	static const int _FPS = 5;
	GState _game_state;

	Field* C_Field;
	EnemyController* C_EnemyController;
};