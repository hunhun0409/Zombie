#pragma once

class Knight : public Character
{
private:
	const float MAX_WALKSPEED = 10.0f;
	const float MAX_RUNSPEED = 20.0f;

private:
	enum State
	{
		IDLE, 
		WALKF, WALKB, WALKL, WALKR,
		RUNF, RUNB, RUNL, RUNR,
		SLASH,
		DIE
	};
public:
	Knight();
	~Knight();

	virtual void Update() override;
	virtual void Render() override;
	virtual void GUIRender() override;
	virtual void PostRender() override;

	void Move();


	virtual void Idle() override;
	virtual void Action() override;
	virtual void RButton() override;
	virtual void Shift() override;
	virtual void Ctrl() override;


	void EnableAttack();
	void DisableAttack();

	virtual void TakeDamage(float damage) override;

	void Dead();

public:
	MeleeWeapon* GetWeapon() { return sword; }

private:
	void ReadClips();

	void SetAnimation();
private:
	bool isDash = false;

	Vector3 velocity;

	Transform* rightHand;
	Transform* leftHand;

	MeleeWeapon* sword;

	PlayerHUD* playerHud;
};
