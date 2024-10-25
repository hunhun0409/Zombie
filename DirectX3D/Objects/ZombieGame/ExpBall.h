#pragma once

class ExpBall : public InstanceObject
{
public:
	ExpBall(Transform* transform);
	~ExpBall() = default;

	virtual void Update() override;

	void SetValue(float value) { this->value = value; }
	void ApplyEffect();

	void SetTarget(void* target);

private:
	Character* target = nullptr;

	float value = 50.0f;
};
