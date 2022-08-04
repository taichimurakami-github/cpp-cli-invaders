#pragma once
#include "FieldObjBase.h"
#include "Field.h"

class EnemyBullet : public FieldObjBase
{
public:
	EnemyBullet(int x) :FieldObjBase(x, 0) {
		_is_fired = false;
		_is_hit = false;
	}

	//Fire!!!!
	void Init(int x_shoot_from, int y_shoot_from) {
		_x = x_shoot_from;
		_y = y_shoot_from;
		_is_fired = true;
	}

	void Destroy() {
		_is_fired = false;
	}

	void MoveDown(Field* field) {
		field->SetFieldState(_x, _y, Field::FValue::NONE);

		if (_y + 1 >= field->GetFieldHeight()) {
			//既に下端に到達していた場合：弾を消去
			Destroy();
			return;
		}

		if (_IntersectPlayer(field->GetFieldValue(_x, _y + 1))) {
			//当たり判定：プレーヤーと重なったらフラグを立てて処理を終了
			_is_hit = true;
			Destroy();
			return;
		}

		//下端に到達していない場合：弾を下に移動
		field->SetFieldState(_x, ++_y, Field::FValue::ENEMY_BULLET);
	}

	bool GetIsFired() {
		return _is_fired;
	}

	bool GetIsHit() {
		return _is_hit;
	}

private:
	bool _is_fired;
	bool _is_hit;

	bool _IntersectPlayer(Field::FValue fvalue) {
		//移動先がPlayerだったらゲームオーバー
		//敵弾は属性関係なく１種類で、当たったらPlayerはOUT	
		return ((fvalue == Field::FValue::PLAYER_P) || (fvalue == Field::FValue::PLAYER_M));
	}
};
