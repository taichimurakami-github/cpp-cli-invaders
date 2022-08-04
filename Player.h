#pragma once
#include "FieldObjBase.h"
#include "Field.h";
#include "PlayerBullet.h"
#include "EnemyController.h"


class Player : public FieldObjBase
{
public:
	Player(int x, int y) : FieldObjBase(x, y) {
		C_PlayerBullet = new PlayerBullet(x, y);

		_init_x = x;
		_init_y = y;
		_hit_count = 0;
	}

	void Init(Field* field) {
		field->SetFieldState(_x, _y, Field::FValue::PLAYER);
		_hit_count = 0;
	}

	//playerbulletのアップデート
	void Update(Field* field, EnemyController* enemy_controller) {
		//発射されていなければなにもしない
		if (C_PlayerBullet->GetIsFired() == false) {
			return;
		}

		bool is_hit = C_PlayerBullet->MoveUp(field, enemy_controller);

		if (is_hit) {
			_hit_count += 1;
		}
	}


	bool MoveRight(Field* field) {

		//右端に居るので何もしない
		if (_x == field->GetFieldWidth() - 1) {
			return false;
		}

		//敵弾にあたりに行ったとき
		if (field->GetFieldValue(_x + 1, _y) == Field::FValue::ENEMY_BULLET) {
			//ゲームオーバー
			return true;
		}

		field->SetFieldState(_x, _y);
		SetX(_x + 1);
		field->SetFieldState(_x, _y, Field::FValue::PLAYER);

		return false;
	}

	bool MoveLeft(Field* field) {

		//左端に居るので何もしない
		if (_x == 0) {
			return false;
		}

		if (field->GetFieldValue(_x - 1, _y) == Field::FValue::ENEMY_BULLET) {
			//ゲームオーバー
			return true;
		}

		field->SetFieldState(_x, _y);
		SetX(_x - 1);
		field->SetFieldState(_x, _y, Field::FValue::PLAYER);

		return false;
	}

	void Shoot() {
		//既に発射済みだったら撃てない
		if (C_PlayerBullet->GetIsFired()) {
			return;
		}
		//Fire!!
		C_PlayerBullet->Init(_x, _y - 1);
	}

	int GetHitCount() {
		return _hit_count;
	}

private:
	PlayerBullet* C_PlayerBullet;
	int _init_x;
	int _init_y;
	int _hit_count;
};

