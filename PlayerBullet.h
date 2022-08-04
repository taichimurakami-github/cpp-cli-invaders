#pragma once
#include "FieldObjBase.h"
#include "Field.h"
#include "Enemy.h"
#include "EnemyController.h"


class PlayerBullet : public FieldObjBase
{
public:
	static enum Attribute {//©’e‚Ì‘®«
		TYPE_P,
		TYPE_M
	};

	PlayerBullet(int x, int y) :FieldObjBase(x, y) {
		_is_fired = false;
		_is_hit = false;
		_attribute = Attribute::TYPE_M;
	}

	//Fire!!!!
	void Fire(int x_shoot_from, int y_shoot_from, Attribute attr) {
		_x = x_shoot_from;
		_y = y_shoot_from;
		_attribute = attr;
		_is_fired = true;
	}


	void Destroy() {
		_is_fired = false;
	}

	//“G–{‘Ì‚É–½’†‚µ‚½ê‡‚Ì‚İtrue,‚»‚êˆÈŠO‚Ífalse‚ğ•Ô‚·
	bool MoveUp(Field* field, EnemyController* enemy_controller) {
		field->SetFieldState(_x, _y, Field::FValue::NONE);

		if (_y - 1 <= 0) {
			//Šù‚É‰º’[‚É“’B‚µ‚Ä‚¢‚½ê‡F’e‚ğÁ‹
			Destroy();
			return false;
		}

		Field::FValue fvalue = field->GetFieldValue(_x, _y - 1);

		//“G‚Ì’e‚Æd‚È‚Á‚½‚çŒ‚‚¿•‰‚¯‚é
		if (_IntersectEnemyBullet(fvalue)) {
			Destroy();
			return false;
		}

		//“G‚Æd‚È‚Á‚½‚ç‘®«‘Š«‚©‚ç“G‚ğŒ‚”j‚Å‚«‚é‚©‚Ç‚¤‚©”»’è
		if (_IntersectEnemy(fvalue)) {
			Enemy* enemy = enemy_controller->GetEnemyFromFieldPosition(_x, _y - 1);

			//—LŒø‚È‘®«‘Š«‚ÌUŒ‚‚Ìê‡‚Í”j‰ó‚Å‚«‚é
			if (_IsValidAttack(enemy->GetAttribute())) {
				enemy->Dead(field);
			}

			Destroy();
			return true;
		}

		//ã’[‚É“’B‚µ‚Ä‚¢‚È‚¢ê‡F’e‚ğã‚ÉˆÚ“®iÄ•`‰æj
		field->SetFieldState(_x, --_y, _GetFValue());
		return false;
	}

	bool GetIsFired() {
		return _is_fired;
	}

	bool GetIsHit() {
		return _is_hit;
	}

	Attribute GetAttribute() {
		return _attribute;
	}

	Attribute SetAttribute(Attribute attr) {
		_attribute = attr;
	}

private:
	bool _is_fired;
	bool _is_hit;
	Attribute _attribute;

	Field::FValue _GetFValue() {
		return (_attribute == Attribute::TYPE_M) ? Field::FValue::PLAYER_BULLET_M : Field::FValue::PLAYER_BULLET_P;
	}

	bool _IntersectEnemy(Field::FValue fvalue) {
		//“–‚½‚Á‚½æ‚ª“G‚Ì’e‚©‚Ç‚¤‚©
		return ((fvalue == Field::FValue::ENEMY_P) || (fvalue == Field::FValue::ENEMY_M));
	}

	bool _IntersectEnemyBullet(Field::FValue fvalue) {
		//“–‚½‚Á‚½æ‚ª“G‚Ì’e‚©‚Ç‚¤‚©
		return (fvalue == Field::FValue::ENEMY_BULLET);
	}

	bool _IsValidAttack(Enemy::Attribute e_attribute) {

		//‘Šè‚à©g‚àPƒ^ƒCƒv‚È‚ç—LŒø‘Å
		bool validWhenEnemyIsTypeP = (e_attribute == Enemy::Attribute::TYPE_P) && (_attribute == Attribute::TYPE_P);

		//‘Šè‚à©g‚àMƒ^ƒCƒv‚È‚ç—LŒø‘Å
		bool validWhenEnemyIsTypeM = (e_attribute == Enemy::Attribute::TYPE_M) && (_attribute == Attribute::TYPE_M);

		return (validWhenEnemyIsTypeP || validWhenEnemyIsTypeM);
	}

};

