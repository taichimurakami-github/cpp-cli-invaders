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
	}

	void Init(Field* field) {
		field->SetFieldState(_x, _y, Field::FValue::PLAYER);
	}

	//playerbulletのアップデート
	void Update(Field* field, EnemyController* enemy_controller) {
		if (C_PlayerBullet->GetIsFired()) {
			C_PlayerBullet->MoveUp(field, enemy_controller);
		}
	}


	void MoveRight(Field* field) {

		//右端に居るので何もしない
		if (_x == field->GetFieldWidth() - 1) {
			return;
		}

		field->SetFieldState(_x, _y, Field::FValue::NONE);
		SetX(_x + 1);
		field->SetFieldState(_x, _y, Field::FValue::PLAYER);
	}

	void MoveLeft(Field* field) {

		//左端に居るので何もしない
		if (_x == 0) {
			return;
		}

		field->SetFieldState(_x, _y, Field::FValue::NONE);
		SetX(_x - 1);
		field->SetFieldState(_x, _y, Field::FValue::PLAYER);
	}

	void Shoot() {
		C_PlayerBullet->Init(_x, _y - 1);
	}

private:
	PlayerBullet* C_PlayerBullet;
};

