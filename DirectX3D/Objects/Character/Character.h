#pragma once

class CharacterController;
class CharacterMovement;

class Character : public Transform
{
public:
	struct StatusInitialInfo
	{
		float initialHp = 50.0f;
		float initialAttack = 150.0f;
		float initialRecovery = 1.0f;
		float initialArmor = 0.0f;
		float initialMagnet = 0.0f;
	};
	struct StatusUpgradeInfo
	{
		float upgradeHp = 0;
		float upgradeAttack = 0;
		float upgradeRecovery = 0;
		float upgradeArmor = 0;
		float upgradeMagnet = 0;
	};
	struct FinalStatus
	{
		float finalHp = 0;
		float finalAttack = 0;
		float finalRecovery = 0;
		float finalArmor = 0;
		float finalMagnet = 0;
	};
private:
	const float DEAD_TIME = 3.0f;
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

	void Heal(float amount);

	virtual void TakeDamage(float damage);

	StatusUpgradeInfo& GetUpgradeStatus() { return upgradeStatus; }
	FinalStatus& GetFinalStatus() { return finalStatus; }
	void UpdateStatus();

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
	void LevelUpEnd();

	Transform* GetPivot() { return pivot; }

protected:
	CharacterController* controller = nullptr;
	CapsuleCollider* collider;

	ModelAnimator* model;
	CharacterMovement* characterMovement;

	int curState = 0;
	unordered_map<int, string> states;

	Camera* camera;

	PlayerHUD* playerHud;


	StatusInitialInfo initialStatus;
	StatusUpgradeInfo upgradeStatus;
	FinalStatus finalStatus;

	float curHP = 50.0f;

	float maxExp = 100.0f;
	float curExp = 0.0f;

	float recoverTime = 1.0f;


public:
	bool isPlayingRootMotion = false;
protected:
	Transform* pivot;

	bool isDead = false;
	float deadTime = DEAD_TIME;

};
