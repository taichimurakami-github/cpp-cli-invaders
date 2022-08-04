#pragma once
#include <iostream>
#include <string>
#include "FieldObjBase.h"
#include "Field.h"

class Enemy : public FieldObjBase
{
public:
	static enum Attribute {//���@�̑���
		TYPE_P,
		TYPE_M
	};

	Enemy(int x, int y, Attribute attr = Attribute::TYPE_P) : FieldObjBase(x, y) {
		_is_dead = false;
		_x = x;
		_y = y;
		_attribute = attr;
	};

	static enum MoveDir {
		RIGHT,
		DOWN,
		LEFT,
		STOP
	};

	void Init(Field* field) {
		field->SetFieldState(_x, _y, Field::FValue::ENEMY);
	}

	void Dead(Field* field) {
		SetIsDead(true);
		field->SetFieldState(_x, _y);
	}

	void DrawSelf(Field* field) {
		field->SetFieldState(_x, _y, Field::FValue::ENEMY);
	}

	void MoveRight(Field* field) {
		//�|����Ă�������W�����������i�`�悳��Ȃ������Ŏ��̂͏����Ȃ��j
		if (_is_dead) {
			SetX(_x + 1);
			return;
		}

		field->SetFieldState(_x, _y);
		SetX(_x + 1);
		field->SetFieldState(_x, _y, Field::FValue::ENEMY);
	}

	void MoveLeft(Field* field) {
		if (_is_dead) {
			SetX(_x - 1);
			return;
		}

		field->SetFieldState(_x, _y);
		SetX(_x - 1);
		field->SetFieldState(_x, _y, Field::FValue::ENEMY);
	}

	void MoveDown(Field* field) {
		if (_is_dead) {
			SetY(_y + 1);
			return;
		}

		field->SetFieldState(_x, _y);
		SetY(_y + 1);
		field->SetFieldState(_x, _y, Field::FValue::ENEMY);
	}

	bool IsMovable(MoveDir next_dir, int field_width, int field_height) {
		//�����Ȃ��ꍇ��false��Ԃ�
		switch (next_dir) {
		case MoveDir::RIGHT: {
			return (_x + 1 >= field_width);
			break;
		}
		case MoveDir::DOWN: {
			return (_y + 1 >= field_height);
		}

		case MoveDir::LEFT: {
			//���ɍs����Ȃ�Ăэ��ɓ����A�����Ȃ��Ȃ牺�ɓ���
			return (_x - 1 < 0);
		}
		}

	}
	bool GetIsDead() {
		return _is_dead;
	}

	Attribute GetAttribute() {
		return _attribute;
	}

	void SetIsDead(bool _state) {
		_is_dead = _state;
	}

	Attribute SetAttribute(Attribute attr) {
		_attribute = attr;
	}

private:
	bool _is_dead;
	Attribute _attribute;
};

