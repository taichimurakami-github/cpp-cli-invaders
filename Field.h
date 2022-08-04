#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <time.h>
#include <string.h>

#include "CLIOutput.h"

//Include base class 

class Field
{
public:
	static enum FValue {
		NONE,
		//ENEMY,
		//PLAYER,
		//PLAYER_BULLET,
		ENEMY_BULLET,

		PLAYER_P,
		PLAYER_M,
		ENEMY_P,
		ENEMY_M,
		PLAYER_BULLET_P,
		PLAYER_BULLET_M,
		//ENEMY_BULLET_P,
		//ENEMY_BULLET_M,
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
	void Draw(std::string enemy_colorset = CLIOutput::colorset_white) {
		//コンソールのバッファ削除
		//memset(_field_state, 0, sizeof _field_state);


		//コンソールのリフレッシュ
		system("cls");

		for (int y = 0; y < _FIELD_HEIGHT; y++) {
			std::cout << " |"; //左端の壁
			for (int x = 0; x < _FIELD_WIDTH; x++) {
				FValue fvalue = _field_state[y][x];

				switch (fvalue) {
				case FValue::ENEMY_P: {
					std::cout << enemy_colorset << _TILE[fvalue] << CLIOutput::colorset_white;
					break;
				}

				case FValue::ENEMY_M: {
					std::cout << enemy_colorset << _TILE[fvalue] << CLIOutput::colorset_white;
					break;
				}

				default: {
					std::cout << _TILE[fvalue];
					break;
				}
				}
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
	void SetFieldState(int x, int y, FValue new_field_value = FValue::NONE) {
		_field_state[y][x] = new_field_value;
	}

private:
	static const int _FIELD_WIDTH = 32;
	static const int _FIELD_HEIGHT = 27;
	FValue _field_state[_FIELD_HEIGHT][_FIELD_WIDTH];//tileの値が入る

	std::string _TILE[FValue::MAX] = {//各IDの識別に使える
		 "　",//FValue::NONE
		 //"☆",//FValue::ENEMY
		 //"▲",//FValue::PLAYER
		 //"↑",//FValue::PLAYER_BULLET
		 "○",//FValue::ENEMY_BULLET

		 //追加
		 "▲",//FValue::PLAYER_P
		 "△",//FValue::PLAYER_M
		 "★",//FValue::ENEMY_P
		 "☆",//FValue::ENEMY_M
		 "◆",//FValue::PLAYER_BULLET_P
		 "◇",//FValue::PLAYER_BULLET_M
		 //"●",//FValue::ENEMY_BULLET_P
		 //"○",//FValue::ENEMY_BULLET_M
	};

	void _ResetFieldState() {
		for (int y = 0; y < _FIELD_HEIGHT; y++) {
			for (int x = 0; x < _FIELD_WIDTH; x++) {
				_field_state[y][x] = FValue::NONE;
			}
		}
	}
};

