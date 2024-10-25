#pragma once

class OrbitalSniper : public ActiveSkill
{
public:
	OrbitalSniper();
	~OrbitalSniper();

	virtual void Update() override;
	void Render();
	virtual void GUIRender() override;

	virtual void Activate() override;
	virtual void Deactivate() override;

	virtual void SetOwner(Character* owner) override;

	virtual void LevelUp() override;

	virtual void UpdateChange() override;
private:
	Transform* pivot;
	vector<RangeWeapon*> snipers;

	float distance = 20.0f;
	float damage = 10.0f;

	float damageRatio = 1.5f;
};
