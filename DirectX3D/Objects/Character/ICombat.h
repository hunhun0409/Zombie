#pragma once

class ICombat
{
public:
	virtual ~ICombat() = default;

	//���� ���� �޼���
	virtual void Attack(ICombat* target) = 0;
	virtual void TakeDamage(float damage, ICombat* damageCauser) = 0;
	virtual bool IsAlive() = 0;
	virtual float GetCurHealth() = 0;
	virtual float GetMaxHealth() = 0;

public:
	float maxHp;
	float curHp;
};
