#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <time.h>
#include <string.h>


#include "Field.h"
#include "EnemyController.h"
//#include "Enemy.h"
//#include "PoolAllocator.h"

class Game
{
public:
	Game() {
		//�����ݒ�̓K�p
		_game_state = GState::TITLE;

		//�t�B�[���h�N���X�̕ێ�
		C_Field = new Field();

		//�G�f�[�^�̐����Ə�����
		C_EnemyController = new EnemyController();
	}
	~Game() {}

	static enum GState {
		TITLE,
		PLAYING,
		GAMEOVER,
		MAX
	};

	void SetState(GState s) {
		_game_state = s;
	}

	void PlayGame() {
		std::cout << "press any key to break" << std::endl;
		clock_t lastClock = clock();

		while (1) {
			clock_t nowClock = clock();

			clock_t clockDiff = nowClock - lastClock;
			if (clockDiff >= 1000 / _FPS) {
				lastClock = nowClock;

				C_Field->ResetFieldState();

				if (_kbhit()) {
					SetState(GState::GAMEOVER);
				}

				//�v���[���[�̒e���ړ�������

				//�C���x�[�_�[�ړ�����


				//�C���x�[�_�[�̒e�̔��ˏ���

				//canvas�X�V
				C_EnemyController->SetEnemyIntoField(C_Field);
				C_Field->Draw();


				//�C���x�[�_�[�̒e������������Q�[���I�[�o�[�I�I

			}
		}
	}


private:
	static const int _FPS = 5;
	GState _game_state;

	Field* C_Field;
	EnemyController* C_EnemyController;
};