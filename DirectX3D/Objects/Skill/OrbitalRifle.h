#pragma once

class ObitalRifle : public ActiveSkill
{
public:
	ObitalRifle();
	~ObitalRifle();

	virtual void Update() override;
	void Render();

	virtual void Activate() override;
	virtual void Deactivate() override;

	virtual void SetOwner(Character* owner) override;
private:
	Transform* pivot;
	vector<RangeWeapon*> rifles;

	float distance = 10.0f;
	float damage = 10.0f;

};
