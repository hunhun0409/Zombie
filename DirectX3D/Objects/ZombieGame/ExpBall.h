#pragma once

class ExpBall : public InstanceObject
{
private:
	const float MOVE_TIME = 2.0f;
public:
	ExpBall(Transform* transform);
	~ExpBall() = default;

	virtual void Update() override;

	void SetValue(float value) { this->value = value; }
	void ApplyEffect();

	void SetTarget(void* target);

	void MoveTo();

private:
	Character* target = nullptr;

	float value = 10.0f;

	float moveTime = 0.0f;
};
