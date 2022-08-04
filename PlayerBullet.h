#pragma once
#include "FieldObjBase.h"
#include "Field.h"
#include "Enemy.h"
#include "EnemyController.h"


class PlayerBullet : public FieldObjBase
{
public:
	static enum Attribute {//���e�̑���
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
			//���ɉ��[�ɓ��B���Ă����ꍇ�F�e������
			Destroy();
			return false;
		}

		//�����蔻��F�G�Əd�Ȃ�����G���������Ď��g������
		if (field->GetFieldValue(_x, _y - 1) == Field::FValue::ENEMY) {

			Enemy* enemy = enemy_controller->GetEnemyFromFieldPosition(_x, _y - 1);
			enemy->Dead(field);
			Destroy();
			return true;
		}

		//��[�ɓ��B���Ă��Ȃ��ꍇ�F�e����Ɉړ�
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

