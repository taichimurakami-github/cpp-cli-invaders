#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>

//�C���x�[�_�[�B���ꊇ�ŊǗ�����N���X
//- �t�B�[���h�f�[�^�̍X�V�͌X�̃C���x�[�_�[�N���X�ōs��
//- �K�萔�̃C���x�[�_�[�𐶐�
//- ���ׂẴC���x�[�_�[�����ɑ΂��Ďw�߂𑗂�

//#include "PoolAllocator.h"
#include "Enemy.h"
#include "Field.h"

class EnemyController
{
public:
	EnemyController() {
		//TODO: PoolAllocator���g���������ɕύX
		for (int row = 0; row < _ENEMIES_ROW; row++) {
			for (int col = 0; col < _ENEMIES_COL; col++) {
				int ptr = col + _ENEMIES_COL * row;
				_enemies_state[ptr] = new Enemy(col * 2, row * 2);
			}
		}

		std::cout << "enemy�p���������蓖�Ċ���" << std::endl;

		_next_dir = Enemy::MoveDir::RIGHT;
		isEnemyOnTheBottom = false;
	}

	void Init(Field* field) {
		for (int i = 0; i < _NUM_OF_ENEMIES; i++) {
			_enemies_state[i]->Init(field);
		}
	}

	bool GetIsEnemyOnTheBottom() {
		return isEnemyOnTheBottom;
	}

	int GetEnemiesRow() {
		return _ENEMIES_ROW;
	}

	int GetEnemiesCol() {
		return _ENEMIES_COL;
	}

	Enemy* GetEnemyFromFieldPosition(int x, int y) {

		int enemy_id = _GetEnemyIdFromFieldPosition(x, y);
		return _enemies_state[enemy_id];
	}

	//�w�肳�ꂽ��col�̒��ŁA�ł���ӂɋ߂�enemy��Ԃ�
	Enemy* GetEnemyBottomOfCol(int col) {

		for (int y = _ENEMIES_ROW - 1; y >= 0; y--) {
			Enemy* enemy = _enemies_state[col + _ENEMIES_COL * y];

			if (enemy->GetIsDead() == false) {
				return enemy;
			}
		}

		return nullptr;
	}

	void SetEnemiesIntoField(Field* field) {
		for (int i = 0; i < _NUM_OF_ENEMIES; i++) {
			Enemy* enemy = _enemies_state[i];

			if (enemy->GetIsDead()) {
				//����ł���C���x�[�_�[�ɂ��Ă̏���
				field->SetFieldState(enemy->GetX(), enemy->GetY(), Field::FValue::NONE);
				continue;
			}

			//�����Ă���C���x�[�_�[�ɂ��Ă̏���

			field->SetFieldState(enemy->GetX(), enemy->GetY(), Field::FValue::ENEMY);
		}
	}

	void SetEnemyIntoField(Field* field, Enemy* enemy) {
		field->SetFieldState(enemy->GetX(), enemy->GetY(), Field::FValue::ENEMY);
	}

	void SetEnemiesMoveDir(Enemy::MoveDir next_move_dir) {
		_next_dir = next_move_dir;
	}

	//�����Ă���C���x�[�_�[��S��������
	//�`�揈���p�̃o�b�t�@�������݂��S��
	void Update(Field* field) {
		int field_width = field->GetFieldWidth();
		int field_height = field->GetFieldHeight();
		Enemy::MoveDir new_next_dir = _next_dir;


		for (int i = 0; i < _NUM_OF_ENEMIES; i++) {
			Enemy* enemy = _enemies_state[i];

			//enemy���ړ�������
			switch (_next_dir) {
			case Enemy::MoveDir::RIGHT: {
				enemy->MoveRight(field);//�E�Ɉړ�

				//�E�ɓ����Ȃ������牺�ɓ���
				if (enemy->GetX() == field_width - 1) {
					new_next_dir = Enemy::MoveDir::DOWN;
				}
				break;
			}
			case Enemy::MoveDir::DOWN: {
				enemy->MoveDown(field);//���Ɉړ�
				//bool isMovableToRight = enemy->IsMovable(Enemy::MoveDir::RIGHT, field_width, field_height);

				//�C���x�[�_�[����ӂɂ��ǂ蒅������Q�[���I�[�o�[
				if (enemy->GetY() == field_height - 1) {
					isEnemyOnTheBottom = true;
					return;
				}

				//���ɍ��E�ǂ���ɓ���������
				if (enemy->GetX() == field_width - 1) {
					new_next_dir = Enemy::MoveDir::LEFT;
				}
				else if (enemy->GetX() == 0) {
					new_next_dir = Enemy::MoveDir::RIGHT;
				}
				break;
			}

			case Enemy::MoveDir::LEFT: {
				enemy->MoveLeft(field);//���Ɉړ�

				//���ɓ����Ȃ������牺�ɓ���
				if (enemy->GetX() == 0) {
					new_next_dir = Enemy::MoveDir::DOWN;
				}
				break;
			}
			}

		}

		//�������������X�V
		_next_dir = new_next_dir;
	}


private:
	static const int _ENEMIES_ROW = 5;
	static const int _ENEMIES_COL = 11;
	static const int _NUM_OF_ENEMIES = _ENEMIES_ROW * _ENEMIES_COL;
	Enemy* _enemies_state[_NUM_OF_ENEMIES];
	Enemy::MoveDir _next_dir;

	bool isEnemyOnTheBottom; //�C���x�[�_�[�����[�ɓ��B�����t���O

	int _GetEnemyIdFromFieldPosition(int x, int y) {
		//�v�Z�̊�ɂ��鍶��̃C���x�[�_�[
		//�i�|����Ă��S��Ƃ��ē�����������̂Ŋ�Ƃ��Ďg����j
		int x_base = _enemies_state[0]->GetX();
		int y_base = _enemies_state[0]->GetY();

		return (x - x_base) / 2 + _ENEMIES_COL * (y - y_base) / 2;
	}

};

