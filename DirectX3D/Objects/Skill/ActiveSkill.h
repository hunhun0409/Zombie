#pragma once

class ActiveSkill : public Skill
{
public:
	ActiveSkill(string id, string name, float cooldown, float duration);
	~ActiveSkill() = default;

	virtual void Update();
	virtual void Render() = 0;
	virtual void GUIRender();

	virtual void Activate() override;
	virtual void Deactivate() override;

	virtual void LevelUp() override;
	virtual string GetDescription() const override;
	
protected:
	bool isActive = false;

	float cooldown;
	float duration;

	float curCooldown;
	float curDuration;
};

