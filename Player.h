#pragma once
#include "FieldObjBase.h"
#include "Field.h";


class Player : public FieldObjBase
{
public:
	Player(int x, int y) : FieldObjBase(x, y) {

	}

	void Init(Field* field) {
		field->SetFieldState(_x, _y, Field::FValue::PLAYER);
	}

	void MoveRight(Field* field) {

		//‰E’[‚É‹‚é‚Ì‚Å‰½‚à‚µ‚È‚¢
		if (_x == field->GetFieldWidth() - 1) {
			return;
		}

		field->SetFieldState(_x, _y, Field::FValue::NONE);
		SetX(_x + 1);
		field->SetFieldState(_x, _y, Field::FValue::PLAYER);
	}

	void MoveLeft(Field* field) {

		//¶’[‚É‹‚é‚Ì‚Å‰½‚à‚µ‚È‚¢
		if (_x == 0) {
			return;
		}

		field->SetFieldState(_x, _y, Field::FValue::NONE);
		SetX(_x - 1);
		field->SetFieldState(_x, _y, Field::FValue::PLAYER);
	}

private:
};

