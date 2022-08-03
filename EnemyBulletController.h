#pragma once
#include <stdlib.h>
#include "Enemy.h"
#include "EnemyBullet.h"
#include "EnemyController.h"


class EnemyBulletController
{
public:
	EnemyBulletController(Field* field, EnemyController* enemy_controller) {
		C_EnemyController = enemy_controller;
		C_Field = field;
		_is_hit = false;


		for (int x = 0; x < 11; x++) {
			_enemy_bullets_state[x] = new EnemyBullet(x);
		}
	}

	void Update() {
		for (int x = 0; x < C_EnemyController->GetEnemiesCol(); x++) {
			EnemyBullet* bullet = _enemy_bullets_state[x];

			if (bullet->GetIsFired()) {
				//既に発射された弾の場合：弾を移動 or 消去
				bullet->MoveDown(C_Field);

				if (bullet->GetIsHit()) {
					//弾がヒットした場合
					_is_hit = true;
					return;
				}
			}
			else {
				//まだ発射されていない弾の場合：確率で新しい球を生成
				ShootNewBulletsRandomly(x);
			}
		}
	}

	//確率で新しい球を生成
	void ShootNewBulletsRandomly(int x) {

		bool flg = (rand() % 1000) > 990;

		if (flg) {
			int y = C_EnemyController->GetEnemiesRow() - 1;

			//col: xのy = enemy_controller - 1の部分
			Enemy* enemy_shoot_from = C_EnemyController->GetEnemyBottomOfCol(x);

			if (enemy_shoot_from == nullptr) {
				//その列に倒されていないインベーダーが存在しない場合は
				return;
			}

			int x_shoot_from = enemy_shoot_from->GetX() + 1;//enemyの1つ下マス
			int y_shoot_from = enemy_shoot_from->GetY();

			_enemy_bullets_state[x]->Init(x_shoot_from, y_shoot_from);
		}
	}

	bool GetIsHit() {
		return _is_hit;
	}

private:
	EnemyBullet* _enemy_bullets_state[11];
	EnemyController* C_EnemyController;
	Field* C_Field;
	bool _is_hit;
};

