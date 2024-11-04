#pragma once

class AIController : public CharacterController
{
private:
	const float STATE_CHANGE_COOLDOWN = 3.0f;

public:
	AIController(BasicObject* object = nullptr);
	~AIController() = default;

	virtual void Possess(BasicObject* object) override;
	virtual void UnPossess() override;

	void Update() override;
	void SetTarget(Transform* target) { this->target = target; }

private:
	Transform* target;

	// Player Detection
	float detectionRange = 500.0f;
	float alertRange = 100.0f;
	float attackRange = 6.0f;

	// Timers
	float stateTimer = 0.0f;
	float attackCooldown = 0.0f;

	bool hasScreamed = false;
};
