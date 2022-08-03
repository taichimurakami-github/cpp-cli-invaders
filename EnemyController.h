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
		////PoolAllocator < Enemy, _NUM_OF_ENEMIES > palloc;


		//struct POINT {
		//	int x;
		//	int  y;
		//};
		//const int TEST_SIZE = 100;
		//PoolAllocator<POINT, TEST_SIZE> a;
		//POINT* ptr[TEST_SIZE];
		//for (int i = 0; i < TEST_SIZE; i++) {
		//	ptr[i] = a.Alloc();
		//}
		for (int row = 0; row < _ENEMIES_ROW; row++) {
			for (int col = 0; col < _ENEMIES_COL; col++) {
				_enemies_state[_getIndexFromPosition(col, row)] = new Enemy(col, row);
			}
		}

		std::cout << "enemy�p���������蓖�Ċ���" << std::endl;
	}


	void SetEnemyIntoField(Field* field) {
		for (int i = 0; i < _NUM_OF_ENEMIES; i++) {
			Enemy* _enemy = _enemies_state[i];

			if (_enemy == nullptr) {
				//����ł���C���x�[�_�[�ɂ��Ă̏���
				field->SetFieldState(_enemy->getX(), _enemy->getY(), Field::FValue::NONE);
				continue;
			}

			//�����Ă���C���x�[�_�[�ɂ��Ă̏���
			field->SetFieldState(_enemy->getX(), _enemy->getY(), Field::FValue::ENEMY);
		}
	}

private:
	static const int _ENEMIES_ROW = 5;
	static const int _ENEMIES_COL = 11;
	static const int _NUM_OF_ENEMIES = _ENEMIES_ROW * _ENEMIES_COL;
	Enemy* _enemies_state[_NUM_OF_ENEMIES];

	int _getIndexFromPosition(int x, int y) {
		return x + _ENEMIES_COL * y;
	}
};

