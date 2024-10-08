#pragma once

class PassiveSkill : public Skill
{
public:
	PassiveSkill(string id, string name, int level, float cooldown);
	~PassiveSkill();

	void Activate() override;
	void Deactivate() override;
	void LevelUp() override;
	string GetDescription() const override;
private:
	float effectValue;
};
