#pragma once

class Mannequin : public Character
{
private:
	const float MAX_WALKSPEED = 10.0f;
	const float MAX_RUNSPEED = 20.0f;

private:
	enum State
	{
		UNARM_STANDING_IDLE, UNARM_WALK_FORWARD, EQUIP,
		ARM_STANDING_IDLE, STANDING_WALK_FORWARD, STANDING_WALK_BACK,
		STANDING_WALK_LEFT, STANDING_WALK_RIGHT, STANDING_DRAW_ARROW,
		STANDING_AIM_IDLE, STANDING_AIM_FORWARD, STANDING_AIM_BACK, 
		STANDING_AIM_LEFT, STANDING_AIM_RIGHT, STANDING_AIM_RECOIL, UNEQUIP
	};
public:
	Mannequin();
	~Mannequin();

	virtual void Update() override;
	virtual void Render() override;
	virtual void GUIRender() override;
	virtual void PostRender() override;

	void Move();

	virtual void Idle() override;
	void Aim();
	virtual void WeaponA() override;
	virtual void Action() override;

public:
	Model* GetWeapon() { return bow; }

private:
	void ReadClips();

	void SetAnimation();
private:
	bool isArmed = false;
	bool isAim = false;

	Vector3 velocity;

	Model* bow;

	Transform* leftHand;

	POINT clientCenterPos = { WIN_WIDTH >> 1, WIN_HEIGHT >> 1 };


	Quad* crossHair;
};
