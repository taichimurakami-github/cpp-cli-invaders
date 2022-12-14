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

	//敵本体に命中した場合のみtrue,それ以外はfalseを返す
	bool MoveUp(Field* field, EnemyController* enemy_controller) {
		field->SetFieldState(_x, _y, Field::FValue::NONE);

		if (_y - 1 <= 0) {
			//既に下端に到達していた場合：弾を消去
			Destroy();
			return false;
		}

		Field::FValue fvalue = field->GetFieldValue(_x, _y - 1);

		//敵の弾と重なったら撃ち負ける
		if (_IntersectEnemyBullet(fvalue)) {
			Destroy();
			return false;
		}

		//敵と重なったら属性相性から敵を撃破できるかどうか判定
		if (_IntersectEnemy(fvalue)) {
			Enemy* enemy = enemy_controller->GetEnemyFromFieldPosition(_x, _y - 1);

			if (_IsValidAttack(enemy->GetAttribute())) {
				//有効な属性相性の攻撃の場合は破壊できる
				enemy->Dead(field);
				Destroy();
				return true;
			}
			else {
				//有効な属性相性の攻撃ではない場合は弾が消えるが敵は破壊できない
				Destroy();
				return false;
			}


		}

		//上端に到達していない場合：弾を上に移動（再描画）
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

	bool _IntersectEnemy(Field::FValue fvalue) {
		//当たった先が敵の弾かどうか
		return ((fvalue == Field::FValue::ENEMY_P) || (fvalue == Field::FValue::ENEMY_M));
	}

	bool _IntersectEnemyBullet(Field::FValue fvalue) {
		//当たった先が敵の弾かどうか
		return (fvalue == Field::FValue::ENEMY_BULLET);
	}

	bool _IsValidAttack(Enemy::Attribute e_attribute) {

		//相手も自身もPタイプなら有効打
		bool validWhenEnemyIsTypeP = (e_attribute == Enemy::Attribute::TYPE_P) && (_attribute == Attribute::TYPE_P);

		//相手も自身もMタイプなら有効打
		bool validWhenEnemyIsTypeM = (e_attribute == Enemy::Attribute::TYPE_M) && (_attribute == Attribute::TYPE_M);

		return (validWhenEnemyIsTypeP || validWhenEnemyIsTypeM);
	}

};

