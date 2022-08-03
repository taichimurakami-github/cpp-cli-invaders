#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <time.h>
#include <string.h>

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
};

