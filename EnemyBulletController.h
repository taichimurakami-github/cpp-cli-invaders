#pragma once
#include <stdlib.h>
#include "Enemy.h"
#include "EnemyBullet.h"
#include "EnemyController.h"


class EnemyBulletController
{
public:
	EnemyBulletController(Field* field, EnemyController* enemy_controller) {
		C_EnemyController = enemy_controller;
		C_Field = field;
		_is_hit = false;


		for (int x = 0; x < 11; x++) {
			_enemy_bullets_state[x] = new EnemyBullet(x);
		}
	}

	void Update() {
		for (int x = 0; x < C_EnemyController->GetEnemiesCol(); x++) {
			EnemyBullet* bullet = _enemy_bullets_state[x];

			if (bullet->GetIsFired()) {
				//���ɔ��˂��ꂽ�e�̏ꍇ�F�e���ړ� or ����
				bullet->MoveDown(C_Field);

				if (bullet->GetIsHit()) {
					//�e���q�b�g�����ꍇ
					_is_hit = true;
					return;
				}
			}
			else {
				//�܂����˂���Ă��Ȃ��e�̏ꍇ�F�m���ŐV�������𐶐�
				ShootNewBulletsRandomly(x);
			}
		}
	}

	//�m���ŐV�������𐶐�
	void ShootNewBulletsRandomly(int x) {

		bool flg = (rand() % 1000) > 990;

		if (flg) {
			int y = C_EnemyController->GetEnemiesRow() - 1;

			//col: x��y = enemy_controller - 1�̕���
			Enemy* enemy_shoot_from = C_EnemyController->GetEnemyBottomOfCol(x);

			if (enemy_shoot_from == nullptr) {
				//���̗�ɓ|����Ă��Ȃ��C���x�[�_�[�����݂��Ȃ��ꍇ��
				return;
			}

			int x_shoot_from = enemy_shoot_from->GetX() + 1;//enemy��1���}�X
			int y_shoot_from = enemy_shoot_from->GetY();

			_enemy_bullets_state[x]->Init(x_shoot_from, y_shoot_from);
		}
	}

	bool GetIsHit() {
		return _is_hit;
	}

private:
	EnemyBullet* _enemy_bullets_state[11];
	EnemyController* C_EnemyController;
	Field* C_Field;
	bool _is_hit;
};
