#pragma once
#include <iostream>

class Enemy
{
public:
	Enemy(int x, int y) {
		_isDead = false;
		_x = x;
		_y = y;
	};

	int getX() {
		return _x;
	}

	int getY() {
		return _y;
	}


private:
	static const int _update = 60;
	int _x;
	int _y;
	bool _isDead;
};

