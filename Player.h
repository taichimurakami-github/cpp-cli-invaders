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
		_hit_count = 0;
	}

	void Init(Field* field) {
		field->SetFieldState(_x, _y, Field::FValue::PLAYER);
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


	void MoveRight(Field* field) {

		//�E�[�ɋ���̂ŉ������Ȃ�
		if (_x == field->GetFieldWidth() - 1) {
			return;
		}

		field->SetFieldState(_x, _y, Field::FValue::NONE);
		SetX(_x + 1);
		field->SetFieldState(_x, _y, Field::FValue::PLAYER);
	}

	void MoveLeft(Field* field) {

		//���[�ɋ���̂ŉ������Ȃ�
		if (_x == 0) {
			return;
		}

		field->SetFieldState(_x, _y, Field::FValue::NONE);
		SetX(_x - 1);
		field->SetFieldState(_x, _y, Field::FValue::PLAYER);
	}

	void Shoot() {
		//���ɔ��ˍς݂������猂�ĂȂ�
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
	int _hit_count;
};

