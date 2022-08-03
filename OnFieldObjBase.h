#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <time.h>
#include <string.h>

class OnFieldObjBase
{
public:
	OnFieldObjBase(int _x, int _y, int _update_fps) {
		x = _x;
		y = _y;
		update_fps = _update_fps;
	}
	~OnFieldObjBase() {}

	int x;
	int y;
	int update_fps;
};

