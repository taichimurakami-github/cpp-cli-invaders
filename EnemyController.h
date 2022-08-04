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
		//TODO: PoolAllocatorを使った生成に変更
		for (int row = 0; row < _ENEMIES_ROW; row++) {
			for (int col = 0; col < _ENEMIES_COL; col++) {
				int ptr = col + _ENEMIES_COL * row;
				_enemies_state[ptr] = new Enemy(col * 2, row * 2);
			}
		}

		std::cout << "enemy用メモリ割り当て完了" << std::endl;

		_next_dir = Enemy::MoveDir::RIGHT;
		isEnemyOnTheBottom = false;
	}

	void Init(Field* field) {
		for (int i = 0; i < _NUM_OF_ENEMIES; i++) {
			_enemies_state[i]->Init(field);
		}
	}

	bool GetIsEnemyOnTheBottom() {
		return isEnemyOnTheBottom;
	}

	int GetEnemiesRow() {
		return _ENEMIES_ROW;
	}

	int GetEnemiesCol() {
		return _ENEMIES_COL;
	}

	Enemy* GetEnemyFromFieldPosition(int x, int y) {

		int enemy_id = _GetEnemyIdFromFieldPosition(x, y);
		return _enemies_state[enemy_id];
	}

	//指定された列colの中で、最も底辺に近いenemyを返す
	Enemy* GetEnemyBottomOfCol(int col) {

		for (int y = _ENEMIES_ROW - 1; y >= 0; y--) {
			Enemy* enemy = _enemies_state[col + _ENEMIES_COL * y];

			if (enemy->GetIsDead() == false) {
				return enemy;
			}
		}

		return nullptr;
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
	//描画処理用のバッファ書き込みも担当
	void Update(Field* field) {
		int field_width = field->GetFieldWidth();
		int field_height = field->GetFieldHeight();
		Enemy::MoveDir new_next_dir = _next_dir;


		for (int i = 0; i < _NUM_OF_ENEMIES; i++) {
			Enemy* enemy = _enemies_state[i];

			//enemyを移動させる
			switch (_next_dir) {
			case Enemy::MoveDir::RIGHT: {
				enemy->MoveRight(field);//右に移動

				//右に動けなかったら下に動く
				if (enemy->GetX() == field_width - 1) {
					new_next_dir = Enemy::MoveDir::DOWN;
				}
				break;
			}
			case Enemy::MoveDir::DOWN: {
				enemy->MoveDown(field);//下に移動
				//bool isMovableToRight = enemy->IsMovable(Enemy::MoveDir::RIGHT, field_width, field_height);

				//インベーダーが底辺にたどり着いたらゲームオーバー
				if (enemy->GetY() == field_height - 1) {
					isEnemyOnTheBottom = true;
					return;
				}

				//次に左右どちらに動くか決定
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

				//左に動けなかったら下に動く
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


private:
	static const int _ENEMIES_ROW = 5;
	static const int _ENEMIES_COL = 11;
	static const int _NUM_OF_ENEMIES = _ENEMIES_ROW * _ENEMIES_COL;
	Enemy* _enemies_state[_NUM_OF_ENEMIES];
	Enemy::MoveDir _next_dir;

	bool isEnemyOnTheBottom; //インベーダーが下端に到達したフラグ

	int _GetEnemyIdFromFieldPosition(int x, int y) {
		//計算の基準にする左上のインベーダー
		//（倒されても亡霊として動かし続けるので基準として使える）
		int x_base = _enemies_state[0]->GetX();
		int y_base = _enemies_state[0]->GetY();

		return (x - x_base) / 2 + _ENEMIES_COL * (y - y_base) / 2;
	}

};

