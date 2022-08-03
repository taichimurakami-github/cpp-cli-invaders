#pragma once
#include <iostream>
#include "FieldObjBase.h"
#include "Field.h";


class Enemy : public FieldObjBase
{
public:
	Enemy(int x, int y) : FieldObjBase(x, y) {
		_is_dead = false;
		_x = x;
		_y = y;
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
		field->SetFieldState(_x, _y, Field::FValue::NONE);
	}

	void MoveRight(Field* field) {
		field->SetFieldState(_x, _y, Field::FValue::NONE);
		SetX(_x + 1);
		field->SetFieldState(_x, _y, Field::FValue::ENEMY);
	}

	void MoveLeft(Field* field) {
		field->SetFieldState(_x, _y, Field::FValue::NONE);
		SetX(_x - 1);
		field->SetFieldState(_x, _y, Field::FValue::ENEMY);
	}

	void MoveDown(Field* field) {
		field->SetFieldState(_x, _y, Field::FValue::NONE);
		SetY(_y + 1);
		field->SetFieldState(_x, _y, Field::FValue::ENEMY);
	}

	bool IsMovable(MoveDir next_dir, int field_width, int field_height) {
		//“®‚¯‚È‚¢ê‡‚Ífalse‚ğ•Ô‚·
		switch (next_dir) {
		case MoveDir::RIGHT: {
			return (_x + 1 >= field_width);
			break;
		}
		case MoveDir::DOWN: {
			return (_y + 1 >= field_height);
		}

		case MoveDir::LEFT: {
			//¶‚És‚¯‚é‚È‚çÄ‚Ñ¶‚É“®‚­A‚¢‚¯‚È‚¢‚È‚ç‰º‚É“®‚­
			return (_x - 1 < 0);
		}
		}

	}

	bool GetIsDead() {
		return _is_dead;
	}

	void SetIsDead(bool _state) {
		_is_dead = _state;
	}

private:
	bool _is_dead;
};

