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

	//�G�{�̂ɖ��������ꍇ�̂�true,����ȊO��false��Ԃ�
	bool MoveUp(Field* field, EnemyController* enemy_controller) {
		field->SetFieldState(_x, _y, Field::FValue::NONE);

		if (_y - 1 <= 0) {
			//���ɉ��[�ɓ��B���Ă����ꍇ�F�e������
			Destroy();
			return false;
		}

		Field::FValue fvalue = field->GetFieldValue(_x, _y - 1);

		//�G�̒e�Əd�Ȃ����猂��������
		if (_IntersectEnemyBullet(fvalue)) {
			Destroy();
			return false;
		}

		//�G�Əd�Ȃ����瑮����������G�����j�ł��邩�ǂ�������
		if (_IntersectEnemy(fvalue)) {
			Enemy* enemy = enemy_controller->GetEnemyFromFieldPosition(_x, _y - 1);

			//�L���ȑ��������̍U���̏ꍇ�͔j��ł���
			if (_IsValidAttack(enemy->GetAttribute())) {
				enemy->Dead(field);
			}

			Destroy();
			return true;
		}

		//��[�ɓ��B���Ă��Ȃ��ꍇ�F�e����Ɉړ��i�ĕ`��j
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
		//���������悪�G�̒e���ǂ���
		return ((fvalue == Field::FValue::ENEMY_P) || (fvalue == Field::FValue::ENEMY_M));
	}

	bool _IntersectEnemyBullet(Field::FValue fvalue) {
		//���������悪�G�̒e���ǂ���
		return (fvalue == Field::FValue::ENEMY_BULLET);
	}

	bool _IsValidAttack(Enemy::Attribute e_attribute) {

		//��������g��P�^�C�v�Ȃ�L����
		bool validWhenEnemyIsTypeP = (e_attribute == Enemy::Attribute::TYPE_P) && (_attribute == Attribute::TYPE_P);

		//��������g��M�^�C�v�Ȃ�L����
		bool validWhenEnemyIsTypeM = (e_attribute == Enemy::Attribute::TYPE_M) && (_attribute == Attribute::TYPE_M);

		return (validWhenEnemyIsTypeP || validWhenEnemyIsTypeM);
	}

};

