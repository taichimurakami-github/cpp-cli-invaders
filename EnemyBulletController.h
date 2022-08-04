#pragma once
#include <stdlib.h>
#include "Enemy.h"
#include "EnemyBullet.h"
#include "EnemyController.h"
#include "PoolAllocator.h"

class EnemyBulletController
{
public:
	EnemyBulletController(Field* field, EnemyController* enemy_controller, int enemy_bullet_level = 0) {
		C_EnemyController = enemy_controller;
		C_Field = field;
		_enemy_bullet_level = enemy_bullet_level;

		//初期化済みなのでメモリを確保
		_AllocateEnemyBullets();
	}

	void Init() {
		_is_hit = false;
		_enemy_bullet_level = 0;

		//メモリ開放処理
		_FreeEnemyBullets();

		//メモリ確保
		_AllocateEnemyBullets();
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

		if (_GetShootProbavility()) {
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

	int GetRefreshInterval() {
		switch (_enemy_bullet_level) {
		case 0://LV:1
			return 1000 / 3;
		case 1://LV:2
			return 1000 / 5;
		case 2://LV:3
			return 1000 / 7;
		}

		throw "E_INVALID_ENEMY_BULLET_LEVEL";
	}

	void SetEnemyBulletLevel(int new_level) {
		_enemy_bullet_level = new_level;
	}


private:
	static const int _ENEMY_BULLETS_COL = 11;
	EnemyBullet* _enemy_bullets_state[_ENEMY_BULLETS_COL];
	PoolAllocator<EnemyBullet, _ENEMY_BULLETS_COL> _palloc;
	EnemyController* C_EnemyController;
	Field* C_Field;
	int _enemy_bullet_level;
	bool _is_hit;

	void _AllocateEnemyBullets() {
		for (int x = 0; x < _ENEMY_BULLETS_COL; x++) {
			_enemy_bullets_state[x] = new(_palloc.Alloc()) EnemyBullet(x);
		}
	}

	void _FreeEnemyBullets() {
		for (int x = 0; x < _ENEMY_BULLETS_COL; x++) {
			_palloc.Free(_enemy_bullets_state[x]);
		}
	}

	bool _GetShootProbavility() {
		switch (_enemy_bullet_level) {
		case 0://LV:1
			return (rand() % 1000) > 990;
		case 1://LV:2
			return (rand() % 1000) > 960;
		case 2://LV:3
			return (rand() % 1000) > 920;
		case 3://LV:4
			return (rand() % 1000) > 900;
		}
	}
};

