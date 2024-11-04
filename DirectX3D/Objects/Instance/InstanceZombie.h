#pragma once

class InstanceZombie : public InstanceCharacter
{
public:
	enum Behavior
	{
		NORMAL, CHASE, ALERT
	};

private:
	const float IDLE_TIME = 5.0f;
	const float ATTACK_COOLDOWN = 1.0f;
	//const float STATE_CHANGE_COOLDOWN = 3.0f;

	const float PATROL_TIME = 5.0f;
	const float PATROL_MIN_RADIUS = 20.0f;
	const float PATROL_MAX_RADIUS = 40.0f;
	const float PATROL_SPEED = 4.0f;
	
	const float CHASE_SPEED = 4.0f;
	const float ALERT_SPEED = 20.0f;

	enum State
	{
		IDLE, WALK, RUN, SCREAM, ATTACK, BITE_START, BITE_LOOP, BITE_END, HIT, DIE
	};

public:
	InstanceZombie(string name, Transform* transform, ModelAnimatorInstancing* instancing, UINT index);
	~InstanceZombie();

	virtual void Update() override;
	virtual void Render() override;
	virtual void GUIRender() override;

	virtual void Spawn(Vector3 pos) override;

	void SetRightHandIndex(UINT index) { rightHandIndex = index; }
	virtual void TakeDamage(float damage) override;

	void SetDamage(float dmg) { damage = dmg; }
	void SetHp(float hp);
private:
	void ProcessBehavior();

	void Rotate();

	void Idle();
	void Dead();
	void ScreamEnd();

	void EnableAttack();
	void DisableAttack();

	void BiteLoop();
	void BiteEnd();

public:
	void SetBehavior(Behavior behavior);

	void Patrol();
	void Chase();
	void Alert();
	
private:

	Transform* rightHandTransform;
	SphereCollider* rightHand;
	UINT rightHandIndex;

	Behavior curBehavior = NORMAL;

	// Player Detection
	float detectionRange = 500.0f;
	float alertRange = 100.0f;
	float attackRange = 6.0f;

	// Patrol Behavior
	Vector3 patrolPoint = Vector3();
	float idleTime = 0.0f;
	float patrolTime = 0.0f;

	float rotSpeed = 5.0f;
	
	// Timers
	//float stateTimer = 0.0f;
	float attackCooldown = 0.0f;

	bool hasScreamed = false;

	float damage = 10.0f;
};
