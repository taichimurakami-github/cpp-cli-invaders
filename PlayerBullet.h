#pragma once
#include "FieldObjBase.h"
#include "Field.h"
#include "Enemy.h"
#include "EnemyController.h"


class PlayerBullet : public FieldObjBase
{
public:
	static enum Attribute {//自弾の属性
		TYPE_P,
		TYPE_M
	};

	PlayerBullet(int x, int y) :FieldObjBase(x, y) {
		_is_fired = false;
		_is_hit = false;
		_attribute = Attribute::TYPE_M;
	}

	//Fire!!!!
	void Fire(int x_shoot_from, int y_shoot_from, Attribute attr) {
		_x = x_shoot_from;
		_y = y_shoot_from;
		_attribute = attr;
		_is_fired = true;
	}


	void Destroy() {
		_is_fired = false;
	}

	bool MoveUp(Field* field, EnemyController* enemy_controller) {
		field->SetFieldState(_x, _y, Field::FValue::NONE);

		if (_y - 1 <= 0) {
			//既に下端に到達していた場合：弾を消去
			Destroy();
			return false;
		}

		//当たり判定：敵と重なったら敵を消去して自身も消去
		if (field->GetFieldValue(_x, _y - 1) == Field::FValue::ENEMY) {

			Enemy* enemy = enemy_controller->GetEnemyFromFieldPosition(_x, _y - 1);
			enemy->Dead(field);
			Destroy();
			return true;
		}

		//上端に到達していない場合：弾を上に移動
		field->SetFieldState(_x, --_y, _GetFValue());
		return false;
	}

	bool GetIsFired() {
		return _is_fired;
	}

	bool GetIsHit() {
		return _is_hit;
	}

	Attribute GetAttribute() {
		return _attribute;
	}

	Attribute SetAttribute(Attribute attr) {
		_attribute = attr;
	}

private:
	bool _is_fired;
	bool _is_hit;
	Attribute _attribute;

	Field::FValue _GetFValue() {
		return (_attribute == Attribute::TYPE_M) ? Field::FValue::PLAYER_BULLET_M : Field::FValue::PLAYER_BULLET_P;
	}
};

