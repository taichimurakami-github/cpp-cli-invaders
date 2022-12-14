#pragma once
#include <stdlib.h>
#include <string>

class FieldObjBase
{
public:
	FieldObjBase(int x, int y) {
		_x = x;
		_y = y;
	}
	~FieldObjBase() {}

	int GetX() {
		return _x;
	}

	int GetY() {
		return _y;
	}

	void SetX(int new_x) {
		_x = new_x;
	}

	void SetY(int new_y) {
		_y = new_y;
	}

protected:
	int _x;
	int _y;
	std::string _tile;
};

