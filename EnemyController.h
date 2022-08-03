#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>

//インベーダー達を一括で管理するクラス
//- フィールドデータの更新は個々のインベーダークラスで行う
//- 規定数のインベーダーを生成
//- すべてのインベーダーたちに対して指令を送る

//#include "PoolAllocator.h"
#include "Enemy.h"
#include "Field.h"

class EnemyController
{
public:
	EnemyController() {
		////PoolAllocator < Enemy, _NUM_OF_ENEMIES > palloc;


		//struct POINT {
		//	int x;
		//	int  y;
		//};
		//const int TEST_SIZE = 100;
		//PoolAllocator<POINT, TEST_SIZE> a;
		//POINT* ptr[TEST_SIZE];
		//for (int i = 0; i < TEST_SIZE; i++) {
		//	ptr[i] = a.Alloc();
		//}
		for (int row = 0; row < _ENEMIES_ROW; row++) {
			for (int col = 0; col < _ENEMIES_COL; col++) {
				_enemies_state[_getIndexFromPosition(col, row)] = new Enemy(col * 2, row * 2);
			}
		}

		std::cout << "enemy用メモリ割り当て完了" << std::endl;

		_next_dir = Enemy::MoveDir::RIGHT;
		isEnemyOnTheBottom = false;
	}

	bool GetIsEnemyOnTheBottom() {
		return isEnemyOnTheBottom;
	}

	void SetEnemiesIntoField(Field* field) {
		for (int i = 0; i < _NUM_OF_ENEMIES; i++) {
			Enemy* enemy = _enemies_state[i];

			if (enemy->GetIsDead()) {
				//死んでいるインベーダーについての処理
				field->SetFieldState(enemy->GetX(), enemy->GetY(), Field::FValue::NONE);
				continue;
			}

			//生きているインベーダーについての処理

			field->SetFieldState(enemy->GetX(), enemy->GetY(), Field::FValue::ENEMY);
		}
	}

	void SetEnemyIntoField(Field* field, Enemy* enemy) {
		field->SetFieldState(enemy->GetX(), enemy->GetY(), Field::FValue::ENEMY);
	}

	void SetEnemiesMoveDir(Enemy::MoveDir next_move_dir) {
		_next_dir = next_move_dir;
	}

	//生きているインベーダーを全員動かす
	void MoveAllEnemies(Field* field) {
		int field_width = field->GetFieldWidth();
		int field_height = field->GetFieldHeight();
		Enemy::MoveDir new_next_dir = _next_dir;


		for (int i = 0; i < _NUM_OF_ENEMIES; i++) {
			Enemy* enemy = _enemies_state[i];

			if (enemy->GetIsDead()) {
				continue;
			}

			//enemyを移動させる
			switch (_next_dir) {
			case Enemy::MoveDir::RIGHT: {
				enemy->MoveRight(field);//右に移動
				if (enemy->GetX() == field_width - 1) {
					new_next_dir = Enemy::MoveDir::DOWN;
				}
				break;
			}
			case Enemy::MoveDir::DOWN: {
				enemy->MoveDown(field);//下に移動
				//bool isMovableToRight = enemy->IsMovable(Enemy::MoveDir::RIGHT, field_width, field_height);

				if (enemy->GetY() == field_height - 1) {
					isEnemyOnTheBottom = true;
					return;
				}

				if (enemy->GetX() == field_width - 1) {
					new_next_dir = Enemy::MoveDir::LEFT;
				}
				else if (enemy->GetX() == 0) {
					new_next_dir = Enemy::MoveDir::RIGHT;
				}
				break;
			}

			case Enemy::MoveDir::LEFT: {
				enemy->MoveLeft(field);//左に移動

				if (enemy->GetX() == 0) {
					new_next_dir = Enemy::MoveDir::DOWN;
				}
				break;
			}
			}

		}

		//次動く方向を更新
		_next_dir = new_next_dir;
	}

	void UpdateMoveDir() {
		switch (_next_dir) {
		case Enemy::MoveDir::RIGHT: {
			//右端に接しているインベーダーあり
			break;
		}
		case Enemy::MoveDir::DOWN: {
			break;
		}

		case Enemy::MoveDir::LEFT: {
			break;
		}
		}

	}


private:
	static const int _ENEMIES_ROW = 5;
	static const int _ENEMIES_COL = 11;
	static const int _NUM_OF_ENEMIES = _ENEMIES_ROW * _ENEMIES_COL;
	Enemy* _enemies_state[_NUM_OF_ENEMIES];
	Enemy::MoveDir _next_dir;

	bool isEnemyOnTheBottom; //インベーダーが下端に到達したフラグ

	int _getIndexFromPosition(int x, int y) {
		return x + _ENEMIES_COL * y;
	}
};

