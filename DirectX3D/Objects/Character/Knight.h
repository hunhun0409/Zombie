#pragma once

class Knight : public Character
{
private:
	const float MAX_WALKSPEED = 10.0f;
	const float MAX_RUNSPEED = 20.0f;

private:
	enum State
	{
		UNARM_IDLE, 
		IDLE, 
		WALKF, WALKB, WALKL, WALKR,
		RUNF, RUNB, RUNL, RUNR,
		KICK, SLASH, STRONG_ATTACK,
		SHEATH1, SHEATH2, DRAW1, DRAW2,
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
	virtual void WeaponA() override;
	virtual void Action() override;
	virtual void RButton() override;
	virtual void Shift() override;
	virtual void Ctrl() override;

	void Draw();
	void DrawEnd();
	void Sheath();
	void SheathEnd();

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
	bool isArm = false;
	bool isDash = false;
	bool isBlock = false;
	bool isCrouch = false;

	Vector3 velocity;

	Transform* rightHand;
	Transform* leftHand;

	MeleeWeapon* sword;

	Trail* trail;

	POINT clientCenterPos = { WIN_WIDTH >> 1, WIN_HEIGHT >> 1 };

	Quad* crossHair;
};
