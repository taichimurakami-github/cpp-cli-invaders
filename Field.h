#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <time.h>
#include <string.h>

//Include base class 

class Field
{
public:
	static enum FValue {
		NONE,
		ENEMY,
		PLAYER,
		PLAYER_BULLET,
		ENEMY_BULLET,
		MAX,
	};

	Field() {
		//_field_stateの初期化
		_ResetFieldState();

	}
	~Field() {}

	void Init() {
		_ResetFieldState();
	}

	void FillTile(int x, int y, FValue val) {
		_field_state[y][x] = val;
	}

	//_field_stateにしたがってコンソールに出力
	void Draw() {
		//コンソールのバッファ削除
		//memset(_field_state, 0, sizeof _field_state);


		//コンソールのリフレッシュ
		system("cls");

		for (int y = 0; y < _FIELD_HEIGHT; y++) {
			std::cout << " |"; //左端の壁
			for (int x = 0; x < _FIELD_WIDTH; x++) {
				FValue fvalue = _field_state[y][x];
				std::cout << _TILE[fvalue];
			}
			std::cout << "|" << std::endl;//右端の壁と改行
		}

	}

	//GET FIELD_WIDTH
	int GetFieldWidth() {
		return _FIELD_WIDTH;
	}

	//GET FIELD_HEIGHT
	int GetFieldHeight() {
		return _FIELD_HEIGHT;
	}

	//GET SPECIFIC FIELD VALUE
	FValue GetFieldValue(int x, int y) {
		return _field_state[y][x];
	}

	//SET SPECIFIC FIELD_VALUE
	void SetFieldState(int x, int y, FValue new_field_value) {
		_field_state[y][x] = new_field_value;
	}

private:
	static const int _FIELD_WIDTH = 32;
	static const int _FIELD_HEIGHT = 27;
	FValue _field_state[_FIELD_HEIGHT][_FIELD_WIDTH];//tileの値が入る

	std::string _TILE[FValue::MAX] = {//各IDの識別に使える
		 "　",//FValue::NONE
		 "☆",//FValue::ENEMY
		 "▲",//FValue::PLAYER
		 "↑",//FValue::PLAYER_BULLET
		 "○",//FValue::ENEMY_BULLET
	};

	void _ResetFieldState() {
		for (int y = 0; y < _FIELD_HEIGHT; y++) {
			for (int x = 0; x < _FIELD_WIDTH; x++) {
				_field_state[y][x] = FValue::NONE;
			}
		}
	}
};

