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

	void SetGState(GState s) {
		_game_state = s;
	}

	void PlayGame() {
		std::cout << "press any key to break" << std::endl;
		clock_t lastClock = clock();
		clock_t lastClockForInvaderInterval = lastClock;
		clock_t lastClockForInvaderBulletInterval = lastClock;

		int update_interval = 1000 / _game_fps;

		while (_game_state == GState::PLAYING) {
			clock_t nowClock = clock();

			clock_t clockDiff = nowClock - lastClock;
			clock_t clockDiffForInvaderInterval = nowClock - lastClockForInvaderInterval;
			clock_t clockDiffForInvaderBulletinterval = nowClock - lastClockForInvaderBulletInterval;

			if (clockDiff >= update_interval) {
				lastClock = nowClock;

				if (_kbhit()) {
					SetGState(GState::GAMEOVER);
				}

				//�v���[���[�̒e���ړ�������

				//�C���x�[�_�[�ړ�����
				if (clockDiffForInvaderInterval >= 1000 / 2) {
					lastClockForInvaderInterval = nowClock;
					C_EnemyController->MoveAllEnemies(C_Field);
					if (C_EnemyController->GetIsEnemyOnTheBottom()) {
						//�C���x�[�_�[�����ɓ��B���Ă�����Q�[���I�[�o�[
						SetGState(GState::GAMEOVER);
						break;
					}
				}



				//�C���x�[�_�[�̒e�̔��ˏ���


				//canvas�X�V
				//C_EnemyController->SetEnemiesIntoField(C_Field);
				C_Field->Draw();


				//�C���x�[�_�[�̒e������������Q�[���I�[�o�[�I�I

			}
		}
	}


private:
	static const int _game_fps = 60;

	GState _game_state;

	Field* C_Field;
	EnemyController* C_EnemyController;
};