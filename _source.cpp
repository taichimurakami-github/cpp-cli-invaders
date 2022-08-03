// cpp-cli-invaders.cpp : ���̃t�@�C���ɂ� 'main' �֐����܂܂�Ă��܂��B�v���O�������s�̊J�n�ƏI���������ōs���܂��B
//

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <time.h>
#include <string.h>

namespace _source {

	constexpr int SCREEN_WIDTH = 32;
	constexpr int SCREEN_HEIGHT = 20;
	constexpr int INVADER_COLUMN = 11;
	constexpr int INVADER_ROW = 5;
	int screen_canvas[SCREEN_HEIGHT][SCREEN_WIDTH];

	//set fps
	constexpr int FPS = 60;
	constexpr int INVADER_REFRESH_FPS = 2;
	constexpr int INVADER_BULLET_REFRESH_FPS = 2;


	enum
	{
		TILE_NONE,
		TILE_INVADER,
		TILE_PLAYER,
		TILE_PLAYER_BULLET,
		TILE_INVADER_BULLET,
		TILE_MAX
	};

	enum {
		DIR_RIGHT,//�s���P�F�E�ɍs��
		DIR_DOWN,//�s���Q�F���ɉ�����
		DIR_LEFT,//�s���R�F���ɍs��
		DIR_MAX, //��ӂɒ��n������~�܂�H
	};

	typedef struct {
		int x, y;
	}VEC2;

	const char* tileAA[TILE_MAX] = {
		"�@",//TILE_NONE
		"��",//TILE_INVADER,
		"��",//TILE_PLAYER
		"��",//TILE_PLAYYER_BULLET
		"��", //TILE_INVADER_BULLET
	};

	typedef struct {
		int x, y;
		bool isDead;
	} INVADER;


	VEC2 directions[] = {//�C���x�[�_�[�̓����x�N�g��
		{1,0},//DIR_RIGHT
		{0,1},//DIR_DOWN
		{-1,0},//DIR_LEFT
	};
	int invaderDirection = 0;
	INVADER invaders[INVADER_ROW][INVADER_COLUMN];

	typedef struct {
		int x, y;
		bool isFired;
	}INVADERBULLET;
	INVADERBULLET invaderBullet[INVADER_COLUMN];//�e��Ɉ�Ƃ���


	typedef struct {
		int x, y;
	} PLAYER;

	PLAYER player;

	typedef struct {
		int x, y;
		bool isFired;
	}PLAYERBULLET;

	PLAYERBULLET playerBullet;

	void DrawScreen() {
		//�R���\�[���̃o�b�t�@�폜
		memset(screen_canvas, 0, sizeof screen_canvas);

		//�C���x�[�_�[�`��
		for (int y = 0; y < INVADER_ROW; y++)
			for (int x = 0; x < INVADER_COLUMN; x++) {
				//����ł�����`�悵�Ȃ�
				if (invaders[y][x].isDead) continue;

				INVADER _temp = invaders[y][x];
				screen_canvas[_temp.y][_temp.x] = TILE_INVADER;
			}

		//�C���x�[�_�[�̒e�`��
		for (int x = 0; x < INVADER_COLUMN; x++) {
			if (invaderBullet[x].isFired) {
				INVADERBULLET _temp = invaderBullet[x];
				screen_canvas[_temp.y][_temp.x] = TILE_INVADER_BULLET;
			}
		}

		//�v���[���[�`��
		screen_canvas[player.y][player.x] = TILE_PLAYER;

		//�v���[���[�̒e����
		if (playerBullet.isFired) {
			screen_canvas[playerBullet.y][playerBullet.x] = TILE_PLAYER_BULLET;
		}

		//�R���\�[���̃��t���b�V��
		system("cls");

		for (int y = 0; y < SCREEN_HEIGHT; y++)
		{
			for (int x = 0; x < SCREEN_WIDTH; x++) {
				std::cout << tileAA[screen_canvas[y][x]];
				//std::cout << screen_canvas[y][x];
			}
			std::cout << std::endl;
		}

	}

	void Init() {
		//�C���x�[�_�[������
		for (int y = 0; y < INVADER_ROW; y++)
			for (int x = 0; x < INVADER_COLUMN; x++) {
				invaders[y][x].x = x * 2;
				invaders[y][x].y = y * 2;
				invaders[y][x].isDead = false;
			}

		//�C���x�[�_�[�̒e������
		for (int x = 0; x < INVADER_COLUMN; x++) {
			invaderBullet[x].isFired = false;
		}

		//�v���[���[�̈ʒu������
		player.x = SCREEN_WIDTH / 2;
		player.y = SCREEN_HEIGHT - 2;

		//�v���[���[�̒e������
		playerBullet.isFired = false;

		DrawScreen();
	}


	void MoveInvaders() {

		int nextDirection = invaderDirection;

		//�C���x�[�_�[�̈ʒu�X�V
		for (int y = 0; y < INVADER_ROW; y++)
			for (int x = 0; x < INVADER_COLUMN; x++) {
				invaders[y][x].x += directions[invaderDirection].x;
				invaders[y][x].y += directions[invaderDirection].y;

				//���̍�팈��
				switch (invaderDirection) {
				case DIR_RIGHT:
					if (invaders[y][x].x >= SCREEN_WIDTH - 1) {
						//�E�ɓ�������E�[�ɂ���ꍇ�͉��Ɉړ�������
						nextDirection = DIR_DOWN;
					}
					break;
				case DIR_DOWN:
					if (invaders[y][x].x >= SCREEN_WIDTH - 1) {
						//���ɓ�������E�[�ɂ���ꍇ�͍��Ɉړ�������
						nextDirection = DIR_LEFT;
					}
					else if (invaders[y][x].x <= 0) {
						//���ɓ������㍶�[�ɂ���ꍇ�͉E�Ɉړ�������
						nextDirection = DIR_RIGHT;
					}
					break;
				case DIR_LEFT:
					if (invaders[y][x].x <= 0) {
						//���ɓ������㍶�[�ɂ���ꍇ�͉E�Ɉړ�������
						nextDirection = DIR_DOWN;
					}
					break;
				}

			}

		invaderDirection = nextDirection;
	}
	//�C���x�[�_�[�̒e���ˏ���
	void ShootInvaderBullets() {

		for (int x = 0; x < INVADER_COLUMN; x++) {
			//�e�̔��ˊm���𑀍�
			if ((rand() % 100)) {
				continue;
			}

			if (!invaderBullet[x].isFired) {
				//invaderBullet[x].isFired = true;

				//���̗�̐����Ă���invader�̒��ň�ԉ��̍s�ɂ���Invader�����߂�
				int invaderRow = -1;
				for (int y = 0; y < INVADER_ROW; y++) {
					if (!invaders[y][x].isDead) {
						invaderRow = y;
					}
				}

				//�����Ă���C���x�[�_�[�����̗�ɂ����ꍇ
				if (invaderRow >= 0) {
					invaderBullet[x].isFired = true;
					invaderBullet[x].x = invaders[invaderRow][x].x;
					invaderBullet[x].y = invaders[invaderRow][x].y + 1;
				}
			}
		}

		for (int x = 0; x < INVADER_COLUMN; x++) {
			if (invaderBullet[x].isFired) {
				invaderBullet[x].y++;
				if (invaderBullet[x].y >= SCREEN_HEIGHT) {
					invaderBullet[x].isFired = false;
				}
			}
		}
	}

	//playerBullet��Invader�̓����蔻��
	bool playerBulletIntersectInvaders() {
		for (int y = 0; y < INVADER_ROW; y++)
			for (int x = 0; x < INVADER_COLUMN; x++) {
				//�C���x�[�_�[�̖S��ɒe���������Ă����蔲����悤�ɂ���
				if (invaders[y][x].isDead) continue;

				bool isXaxisEqual = (invaders[y][x].x == playerBullet.x);
				bool isYaxisEqual = (invaders[y][x].y == playerBullet.y);
				if (isXaxisEqual && isYaxisEqual) {
					playerBullet.isFired = false;
					invaders[y][x].isDead = true;
					return true;
				}
			}
		return false;
	}

	bool invaderBulletIntersectPlayer() {
		for (int x = 0; x < INVADER_COLUMN; x++) {
			if (!invaderBullet[x].isFired) continue;

			bool isXaxisEqual = (invaderBullet[x].x == player.x);
			bool isYaxisEqual = (invaderBullet[x].y == player.y);

			//�v���[���[�ƏՓ˂��܂���
			if (isXaxisEqual && isYaxisEqual) {
				return true;//�Q�[���I�[�o�[�I�I�I
			}
		}

		return false;
	}

	int main()
	{
		//std::cout << "Hello World!\n";

		bool isContinue = true;

		std::cout << "this is _source.cpp" << std::endl;

		//calc interval from fps
		constexpr int refresh_interval = 1000 / FPS;
		constexpr int invader_refresh_interval = 1000 / INVADER_REFRESH_FPS;
		constexpr int invader_bullet_refresh_interval = 1000 / INVADER_BULLET_REFRESH_FPS;

		Init();
		//int a = _getch(screen_canvas);
		clock_t lastClock = clock();
		clock_t lastClockForInvaderInterval = lastClock;
		clock_t lastClockForInvaderBulletInterval = lastClock;
		while (isContinue) {
			clock_t nowClock = clock();

			clock_t clockDiff = nowClock - lastClock;
			clock_t clockDiffForInvaderInterval = nowClock - lastClockForInvaderInterval;
			clock_t clockDiffForInvaderBulletinterval = nowClock - lastClockForInvaderBulletInterval;
			if (clockDiff >= refresh_interval) {
				lastClock = nowClock;

				if (_kbhit()) {
					switch (_getch()) {
					case 'a': player.x--; break;
					case 'd': player.x++; break;
					default:
						if (!playerBullet.isFired) {
							playerBullet.isFired = true;
							playerBullet.x = player.x;
							playerBullet.y = player.y - 1;
						}
						break;
					}

					if (player.x < 0) player.x = 0;
					if (player.x >= SCREEN_WIDTH) player.x = SCREEN_WIDTH - 1;
					if (invaderBulletIntersectPlayer()) {
						Init();
						continue;
					}
				}

				//�v���[���[�̒e���ړ�������
				if (playerBullet.isFired) {
					playerBullet.y--;
					if (playerBullet.y < 0) {
						playerBullet.isFired = false;
					}

					playerBulletIntersectInvaders();
				}

				//�C���x�[�_�[�ړ�����
				if (clockDiffForInvaderInterval >= invader_refresh_interval) {
					lastClockForInvaderInterval = nowClock;
					MoveInvaders();
				}


				//�C���x�[�_�[�̒e�̔��ˏ���
				if (clockDiffForInvaderBulletinterval >= invader_bullet_refresh_interval) {
					lastClockForInvaderBulletInterval = nowClock;
					ShootInvaderBullets();
				}

				//canvas�X�V
				DrawScreen();

				//�C���x�[�_�[�̒e������������Q�[���I�[�o�[�I�I
				if (invaderBulletIntersectPlayer()) {
					Init();
					lastClock = clock();
				}

			}
		}
		return 0;
	}

	// �v���O�����̎��s: Ctrl + F5 �܂��� [�f�o�b�O] > [�f�o�b�O�Ȃ��ŊJ�n] ���j���[
	// �v���O�����̃f�o�b�O: F5 �܂��� [�f�o�b�O] > [�f�o�b�O�̊J�n] ���j���[

	// ��Ƃ��J�n���邽�߂̃q���g: 
	//    1. �\�����[�V���� �G�N�X�v���[���[ �E�B���h�E���g�p���ăt�@�C����ǉ�/�Ǘ����܂� 
	//   2. �`�[�� �G�N�X�v���[���[ �E�B���h�E���g�p���ă\�[�X�Ǘ��ɐڑ����܂�
	//   3. �o�̓E�B���h�E���g�p���āA�r���h�o�͂Ƃ��̑��̃��b�Z�[�W��\�����܂�
	//   4. �G���[�ꗗ�E�B���h�E���g�p���ăG���[��\�����܂�
	//   5. [�v���W�F�N�g] > [�V�������ڂ̒ǉ�] �ƈړ����ĐV�����R�[�h �t�@�C�����쐬���邩�A[�v���W�F�N�g] > [�����̍��ڂ̒ǉ�] �ƈړ����Ċ����̃R�[�h �t�@�C�����v���W�F�N�g�ɒǉ����܂�
	//   6. ��قǂ��̃v���W�F�N�g���ĂъJ���ꍇ�A[�t�@�C��] > [�J��] > [�v���W�F�N�g] �ƈړ����� .sln �t�@�C����I�����܂�
}
