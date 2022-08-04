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
	}
	~Game() {}


	enum GState {
		TITLE,
		PLAYING,
		GAMEOVER,
		GAMECLEAR,
		MAX
	};

	//���t���b�V���p�C���^�[�o���ꗗ
	enum class ENEMY_BULLET_LEVEL {
		LV1,
		LV2,
		LV3,
	};

	enum class ENEMY_LEVEL {
		LV1,
		LV2,
		LV3,
		LV4,
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
	static const int _GAME_FPS = 60;

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
		int update_interval = 1000 / _GAME_FPS;//60fps�Œ�
		int max_hit_count = C_EnemyController->GetNumOfEnemies();
		const int game_master_interval = 1000 / _GAME_FPS;
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

			if (clockDiff >= game_master_interval) {
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
					case 'q': {
						SetGState(GState::TITLE);
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

				//�C���x�[�_�[�{�̊֘A�̏���
				if (clockDiffForInvaderInterval >= C_EnemyController->GetRefreshInterval()) {
					//�C���x�[�_�[�ړ�����
					_lastClockForEnemyInterval = nowClock;
					C_EnemyController->Update(C_Field);
					if (C_EnemyController->GetIsEnemyOnTheBottom()) {
						//�C���x�[�_�[�����ɓ��B���Ă�����Q�[���I�[�o�[
						SetGState(GState::GAMEOVER);
					}
				}
				//else {
				//	//�C���x�[�_�[�ĕ`�揈��
				//	C_EnemyController->Redraw(C_Field);
				//}

				//�C���x�[�_�[�̒e�̔��ˏ���
				if (_lastClockForEnemyBulletInterval >= C_EnemyBulletController->GetRefreshInterval()) {
					_lastClockForEnemyBulletInterval = nowClock;
					C_EnemyBulletController->Update();

					if (C_EnemyBulletController->GetIsHit()) {
						//�C���x�[�_�[�̒e���v���[���[�ɓ���������Q�[���I�[�o�[
						SetGState(GState::GAMEOVER);
					}
				}

				//canvas�X�V
				//C_EnemyController->SetEnemiesIntoField(C_Field);
				C_Field->Draw(C_EnemyController->GetFontColorset());

				//�G��S�ł������ǂ����̔���
				int now_hit_count = C_Player->GetHitCount();
				if (now_hit_count >= max_hit_count) {
					//�Q�[���N���A�I
					SetGState(GState::GAMECLEAR);
				}

				//���x���A�b�v���邩�ǂ����̔���

				if (now_hit_count >= max_hit_count * 0.75) {
					//��i�K���x���A�b�v�F�S�̂̓G������3���ȉ��Ɍ������Ƃ�
					C_EnemyController->SetEnemiesLevel((int)ENEMY_LEVEL::LV3);
					C_EnemyBulletController->SetEnemyBulletLevel((int)ENEMY_BULLET_LEVEL::LV3);
				}
				else if (now_hit_count >= max_hit_count * 0.4) {
					//��i�K���x���A�b�v�F�S�̂̓G������5���ȉ��Ɍ������Ƃ�
					C_EnemyController->SetEnemiesLevel((int)ENEMY_LEVEL::LV2);
					C_EnemyBulletController->SetEnemyBulletLevel((int)ENEMY_BULLET_LEVEL::LV2);
				}
			}
		}
	}
};