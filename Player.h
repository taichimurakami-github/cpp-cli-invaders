#pragma once
#include "FieldObjBase.h"
#include "Field.h";
#include "PlayerBullet.h"
#include "EnemyController.h"


class Player : public FieldObjBase
{
public:
	static enum Attribute {//���@�̑���
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

	//playerbullet�̃A�b�v�f�[�g
	void Update(Field* field, EnemyController* enemy_controller) {
		//���˂���Ă��Ȃ���΂Ȃɂ����Ȃ�
		if (C_PlayerBullet->GetIsFired() == false) {
			return;
		}

		bool is_hit = C_PlayerBullet->MoveUp(field, enemy_controller);

		if (is_hit) {
			_hit_count += 1;
		}
	}


	bool MoveRight(Field* field) {

		//�E�[�ɋ���̂ŉ������Ȃ�
		if (_x == field->GetFieldWidth() - 1) {
			return false;
		}

		//�G�e�ɂ�����ɍs�����Ƃ�

		if (_IntersectEnemyBullet(field->GetFieldValue(_x + 1, _y))) {
			//�Q�[���I�[�o�[
			return true;
		}

		field->SetFieldState(_x, _y);
		SetX(_x + 1);
		field->SetFieldState(_x, _y, _GetPlayerFValue());

		return false;
	}

	bool MoveLeft(Field* field) {

		//���[�ɋ���̂ŉ������Ȃ�
		if (_x == 0) {
			return false;
		}

		//�G�e�ɂ�����ɍs�����Ƃ�
		if (_IntersectEnemyBullet(field->GetFieldValue(_x - 1, _y))) {
			//�Q�[���I�[�o�[
			return true;
		}

		field->SetFieldState(_x, _y);
		SetX(_x - 1);
		field->SetFieldState(_x, _y, _GetPlayerFValue());

		return false;
	}

	void Shoot() {
		//���ɔ��ˍς݂������猂�ĂȂ�
		if (C_PlayerBullet->GetIsFired()) {
			return;
		}

		//���@�̑����Ɠ��������𔭎˂����PlayerBullet�ɓK�p
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
		//���������悪�G�̒e��������Q�[���I�[�o�[
		//�G�e�͑����֌W�Ȃ��P��ނŁA����������OUT
		//�G�̒e�łȂ���Ή����N����Ȃ��i�H�H�j
		return (fvalue == Field::FValue::ENEMY_BULLET);
	}

};

