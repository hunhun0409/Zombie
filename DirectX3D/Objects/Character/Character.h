#pragma once

class CharacterController;
class CharacterMovement;

class Character : public Transform
{
private:
	const float DEAD_TIME = 5.0f;
public:
	Character(string name);
	~Character();

	virtual void Update();
	virtual void Render();
	virtual void PostRender() {};
	virtual void GUIRender();

public:
	virtual void Move();
	virtual void Rotate();

	virtual void Idle() {};
	virtual void WeaponA() {};
	virtual void Action() {};
	virtual void RButton() {};
	virtual void Shift() {};
	virtual void Ctrl() {};

	void SetShader(wstring file) { model->SetShader(file); }

	virtual void TakeDamage(float damage);

protected:

	void SetState(int state, float playRate = 1.0f, float takeTime = 0.2f);

public:
	void SetController(CharacterController* controller) { this->controller = controller; }
	CharacterController* GetController() { return controller; }

	void SetCamera(Camera* camera) { this->camera = camera; }

	float GetHeight() { return collider->Height() + collider->Radius() * 2; }
	CharacterMovement* GetCharacterMovement() { return characterMovement; }
	CapsuleCollider* GetCollider() { return collider; }

	bool IsDead() { return isDead; }

	void GetExp(float amount);
	void LevelUp();

protected:
	CharacterController* controller = nullptr;
	CapsuleCollider* collider;

	ModelAnimator* model;
	CharacterMovement* characterMovement;

	int curState = 0;
	unordered_map<int, string> states;

	Camera* camera;

	float maxHP = 100.0f;
	float curHP = 100.0f;

	float maxExp = 100.0f;
	float curExp = 0.0f;

public:
	bool isPlayingRootMotion = false;
protected:
	bool isDead = false;
	float deadTime = DEAD_TIME;
};
