#pragma once
#include "FieldObjBase.h"
#include "Field.h";
#include "PlayerBullet.h"
#include "EnemyController.h"


class Player : public FieldObjBase
{
public:
	static enum Attribute {//自機の属性
		TYPE_P,
		TYPE_M
	};

	Player(int x, int y) : FieldObjBase(x, y) {
		C_PlayerBullet = new PlayerBullet(x, y);

		_init_x = x;
		_init_y = y;
		_hit_count = 0;
		_attribute = Attribute::TYPE_M;
	}

	void Init(Field* field) {
		_hit_count = 0;
		_attribute = Attribute::TYPE_M;
		field->SetFieldState(_x, _y, _GetPlayerFValue());
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

		if (_IntersectEnemyBullet(field->GetFieldValue(_x + 1, _y))) {
			//ゲームオーバー
			return true;
		}

		field->SetFieldState(_x, _y);
		SetX(_x + 1);
		field->SetFieldState(_x, _y, _GetPlayerFValue());

		return false;
	}

	bool MoveLeft(Field* field) {

		//左端に居るので何もしない
		if (_x == 0) {
			return false;
		}

		//敵弾にあたりに行ったとき
		if (_IntersectEnemyBullet(field->GetFieldValue(_x - 1, _y))) {
			//ゲームオーバー
			return true;
		}

		field->SetFieldState(_x, _y);
		SetX(_x - 1);
		field->SetFieldState(_x, _y, _GetPlayerFValue());

		return false;
	}

	void Shoot() {
		//既に発射済みだったら撃てない
		if (C_PlayerBullet->GetIsFired()) {
			return;
		}

		//自機の属性と同じ属性を発射されるPlayerBulletに適用
		PlayerBullet::Attribute pb_attribute = (_attribute == Attribute::TYPE_M) ? PlayerBullet::Attribute::TYPE_M : PlayerBullet::Attribute::TYPE_P;

		//Fire!!
		C_PlayerBullet->Fire(_x, _y - 1, pb_attribute);
	}

	void ChangeAttribute(Field* field) {
		_attribute = (_attribute == Attribute::TYPE_M) ? Attribute::TYPE_P : Attribute::TYPE_M;
		field->SetFieldState(_x, _y, _GetPlayerFValue());
	}

	int GetHitCount() {
		return _hit_count;
	}

	Attribute GetAttribute() {
		return _attribute;
	}

	Attribute SetAttribute(Attribute attr) {
		_attribute = attr;
	}

private:
	PlayerBullet* C_PlayerBullet;
	int _init_x;
	int _init_y;
	int _hit_count;
	Attribute _attribute;

	Field::FValue _GetPlayerFValue() {
		return (_attribute == Attribute::TYPE_M) ? Field::FValue::PLAYER_M : Field::FValue::PLAYER_P;
	}

	bool _IntersectEnemyBullet(Field::FValue fvalue) {
		//当たった先が敵の弾だったらゲームオーバー
		//敵弾は属性関係なく１種類で、当たったらOUT
		//敵の弾でなければ何も起こらない（？？）
		return (fvalue == Field::FValue::ENEMY_BULLET);
	}

};

