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
			//���ɉ��[�ɓ��B���Ă����ꍇ�F�e������
			Destroy();
			return;
		}

		if (_IntersectPlayer(field->GetFieldValue(_x, _y + 1))) {
			//�����蔻��F�v���[���[�Əd�Ȃ�����t���O�𗧂Ăď������I��
			_is_hit = true;
			Destroy();
			return;
		}

		//���[�ɓ��B���Ă��Ȃ��ꍇ�F�e�����Ɉړ�
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
		//�ړ��悪Player��������Q�[���I�[�o�[
		//�G�e�͑����֌W�Ȃ��P��ނŁA����������Player��OUT	
		return ((fvalue == Field::FValue::PLAYER_P) || (fvalue == Field::FValue::PLAYER_M));
	}
};
