#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <time.h>
#include <string.h>


#include "Field.h"
#include "EnemyController.h"
#include "Player.h"
#include "EnemyBulletController.h"
#include "CLIOutput.h"
//#include "PoolAllocator.h"

class Game
{
public:
	Game() {
		//�t�B�[���h�N���X�̕ێ�
		C_Field = new Field();

		//�G�f�[�^�̐����Ə�����
		C_EnemyController = new EnemyController();

		//�v���[���[�̐����Ə�����
		C_Player = new Player(C_Field->GetFieldWidth() / 2, C_Field->GetFieldHeight() - 1);

		//�G�̒e�f�[�^�̐����Ə�����
		C_EnemyBulletController = new EnemyBulletController(C_Field, C_EnemyController);

		C_CLIOutput = new CLIOutput();

		//�^�C�g����ʕ\��
		SetGState(GState::TITLE);

		_enemy_refresh_interval = R_Interval::ENEMY_LV1;
		_enemy_bullet_refresh_interval = R_Interval::ENEMY_BULLET_LV1;
	}
	~Game() {}


	static enum GState {
		TITLE,
		PLAYING,
		GAMEOVER,
		GAMECLEAR,
		MAX
	};

	//�Q�[����FPS
	static const int GAME_FPS = 80;

	//���t���b�V���p�C���^�[�o���ꗗ
	static enum R_Interval : int {
		GAME = 1000 / 60,

		//�G�̍X�V�p�x
		ENEMY_LV1 = 1000 / 1,
		ENEMY_LV2 = 1000 / 2,
		ENEMY_LV3 = 1000 / 4,
		ENEMY_LV4 = 1000 / 5,

		//�G�e�̍X�V�p�x
		ENEMY_BULLET_LV1 = 1000 / 3,
		ENEMY_BULLET_LV2 = 1000 / 5,
	};

	void SetGState(GState s) {
		_game_state = s;

		switch (s) {
		case GState::TITLE: {
			_ShowTitle();
			break;
		}

		case GState::PLAYING: {
			_PlayGame();
			break;
		}

		case GState::GAMEOVER: {
			_ShowGameOver();
			break;
		}

		case GState::GAMECLEAR: {
			_ShowGameClear();
			break;
		}
		}
	}


private:
	clock_t _lastClockForEnemyInterval;
	clock_t _lastClockForEnemyBulletInterval;
	clock_t _lastClock;
	GState _game_state;

	R_Interval _enemy_refresh_interval;
	R_Interval _enemy_bullet_refresh_interval;

	Field* C_Field;
	EnemyController* C_EnemyController;
	Player* C_Player;
	EnemyBulletController* C_EnemyBulletController;
	CLIOutput* C_CLIOutput;

	void _InitGame() {
		//�C���X�^���X�̏�����
		C_Field->Init();
		C_EnemyController->Init(C_Field);
		C_Player->Init(C_Field);
		C_EnemyBulletController->Init();

		//�e��ϐ��̏�����
		_enemy_refresh_interval = R_Interval::ENEMY_LV1;
		_enemy_bullet_refresh_interval = R_Interval::ENEMY_BULLET_LV1;
	}

	void _ShowTitle() {
		system("cls");
		C_CLIOutput->DrawTitle();

		while (_game_state == GState::TITLE) {
			//�L�[���͑҂�
			switch (_getch()) {
			case ' ': {
				SetGState(GState::PLAYING);
				break;
			}
			}
		}
	}

	void _ShowGameOver() {
		system("cls");
		C_CLIOutput->DrawGameOver();

		while (_game_state == GState::GAMEOVER) {
			//�L�[���͑҂�
			switch (_getch()) {
			case 'q': {
				SetGState(GState::TITLE);
				break;
			}
			case ' ': {
				SetGState(GState::PLAYING);
				break;
			}
			}
		}
	}

	void _ShowGameClear() {
		system("cls");
		C_CLIOutput->DrawGameClear();

		while (_game_state == GState::GAMECLEAR) {
			//�L�[���͑҂�
			switch (_getch()) {
			case 'q': {
				SetGState(GState::TITLE);
				break;
			}
			case ' ': {
				SetGState(GState::PLAYING);
				break;
			}
			}
		}
	}

	void _PlayGame() {
		int update_interval = 1000 / GAME_FPS;//60fps�Œ�
		int max_hit_count = C_EnemyController->GetNumOfEnemies();
		clock_t nowClock = clock();


		//����������
		_InitGame();

		_lastClock = clock();
		_lastClockForEnemyInterval = _lastClock;
		_lastClockForEnemyBulletInterval = _lastClock;


		C_CLIOutput->DrawReady();

		//�Q�[�����[�v
		while (_game_state == GState::PLAYING) {
			nowClock = clock();
			clock_t clockDiff = nowClock - _lastClock;
			clock_t clockDiffForInvaderInterval = nowClock - _lastClockForEnemyInterval;
			clock_t clockDiffForInvaderBulletinterval = nowClock - _lastClockForEnemyBulletInterval;

			if (clockDiff >= R_Interval::GAME) {
				_lastClock = nowClock;

				if (_kbhit()) {

					//�v���[���[�̋@�̑���
					switch (_getch()) {
					case 'a': {
						if (C_Player->MoveLeft(C_Field)) {
							//�ړ���ɓG�e�����������e
							SetGState(GState::GAMEOVER);
						}
						break;
					}
					case 'd': {
						if (C_Player->MoveRight(C_Field)) {
							//�ړ���ɓG�e�����������e
							SetGState(GState::GAMEOVER);
						}
						break;
					}
					default: {
						C_Player->Shoot();
						break;
					}
					}

				}
				//�v���[���[�̒e���ړ�������&�����蔻��
				C_Player->Update(C_Field, C_EnemyController);

				//�C���x�[�_�[�ړ�����
				if (clockDiffForInvaderInterval >= _enemy_refresh_interval) {
					_lastClockForEnemyInterval = nowClock;
					C_EnemyController->Update(C_Field);
					if (C_EnemyController->GetIsEnemyOnTheBottom()) {
						//�C���x�[�_�[�����ɓ��B���Ă�����Q�[���I�[�o�[
						SetGState(GState::GAMEOVER);
					}
				}

				//�C���x�[�_�[�̒e�̔��ˏ���
				if (_lastClockForEnemyBulletInterval >= _enemy_bullet_refresh_interval) {
					_lastClockForEnemyBulletInterval = nowClock;
					C_EnemyBulletController->Update();

					if (C_EnemyBulletController->GetIsHit()) {
						//�C���x�[�_�[�̒e���v���[���[�ɓ���������Q�[���I�[�o�[
						SetGState(GState::GAMEOVER);
					}
				}

				//canvas�X�V
				//C_EnemyController->SetEnemiesIntoField(C_Field);
				C_Field->Draw();

				//�G��S�ł������ǂ����̔���
				if (C_Player->GetHitCount() >= max_hit_count) {
					//�Q�[���N���A�I
					SetGState(GState::GAMECLEAR);
				}

				//���x���A�b�v���邩�ǂ����̔���
			}
		}
	}
};