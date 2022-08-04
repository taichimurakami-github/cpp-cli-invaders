#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>

//�C���x�[�_�[�B���ꊇ�ŊǗ�����N���X
//- �t�B�[���h�f�[�^�̍X�V�͌X�̃C���x�[�_�[�N���X�ōs��
//- �K�萔�̃C���x�[�_�[�𐶐�
//- ���ׂẴC���x�[�_�[�����ɑ΂��Ďw�߂𑗂�

#include "PoolAllocator.h"
#include "Enemy.h"
#include "Field.h"
#include "CLIOutput.h"

class EnemyController
{
public:
	EnemyController(int _enemy_level = 0) {
		//TODO: PoolAllocator���g���������ɕύX

		_AllocateEnemies();

		_next_dir = Enemy::MoveDir::RIGHT;
		is_enemy_on_the_bottom = false;
		SetEnemiesLevel(_enemy_level);
	}

	void Init(Field* field) {
		//_enemies_state�����Z�b�g
		_enemies_level = 0;

		_FreeEnemies();

		_AllocateEnemies();
	}

	//void Redraw(Field* field) {
	//	for (int i = 0; i < _NUM_OF_ENEMIES; i++) {
	//		_enemies_state[i]->DrawSelf(field);
	//	}
	//}

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
					is_enemy_on_the_bottom = true;
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

	bool GetIsEnemyOnTheBottom() {
		return is_enemy_on_the_bottom;
	}

	int GetEnemiesRow() {
		return _ENEMIES_ROW;
	}

	int GetEnemiesCol() {
		return _ENEMIES_COL;
	}

	int GetNumOfEnemies() {
		return _NUM_OF_ENEMIES;
	}

	int GetEnemiesLevel() {
		return _enemies_level;
	}

	std::string GetFontColorset() {
		switch (_enemies_level) {
		case 0://LV:1
			return CLIOutput::colorset_green;
		case 1://LV:2
			return CLIOutput::colorset_yellow;
		case 2://LV:3
			return CLIOutput::colorset_red;
		case 3://LV:4
			return CLIOutput::colorset_cian;
		}
	}

	int GetRefreshInterval() {
		switch (_enemies_level) {
		case 0://LV:1
			return 1000 / 1;
		case 1://LV:2
			return 1000 / 2;
		case 2://LV:3
			return 1000 / 4;
		case 3://LV:4
			return 1000 / 5;
		}

		throw "E_INVALID_ENEMY_LEVEL";
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

	void SetEnemiesLevel(int new_level) {
		_enemies_level = new_level;
	}

private:
	static const int _ENEMIES_ROW = 4;
	static const int _ENEMIES_COL = 11;
	static const int _NUM_OF_ENEMIES = _ENEMIES_ROW * _ENEMIES_COL;
	Enemy* _enemies_state[_NUM_OF_ENEMIES];
	Enemy::MoveDir _next_dir;
	PoolAllocator<Enemy, _NUM_OF_ENEMIES> _palloc;

	int _enemies_level;
	bool is_enemy_on_the_bottom; //�C���x�[�_�[�����[�ɓ��B�����t���O

	void _AllocateEnemies() {
		for (int row = 0; row < _ENEMIES_ROW; row++) {
			for (int col = 0; col < _ENEMIES_COL; col++) {
				int id = col + _ENEMIES_COL * row;
				_enemies_state[id] = new(_palloc.Alloc()) Enemy(2 * col, 2 * row);
			}
		}
	}

	void _FreeEnemies() {
		for (int row = 0; row < _ENEMIES_ROW; row++) {
			for (int col = 0; col < _ENEMIES_COL; col++) {
				int id = col + _ENEMIES_COL * row;
				_palloc.Free(_enemies_state[id]);
			}
		}
	}

	int _GetEnemyIdFromFieldPosition(int x, int y) {
		//�v�Z�̊�ɂ��鍶��̃C���x�[�_�[
		//�i�|����Ă��S��Ƃ��ē�����������̂Ŋ�Ƃ��Ďg����j
		int x_base = _enemies_state[0]->GetX();
		int y_base = _enemies_state[0]->GetY();

		return (x - x_base) / 2 + _ENEMIES_COL * (y - y_base) / 2;
	}

};

