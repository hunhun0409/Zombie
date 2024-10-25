#pragma once

class ActiveSkill : public Skill
{
public:
	ActiveSkill(SkillInfo info, float cooldown, float duration);
	ActiveSkill(string id, string name, wstring iconPath, float cooldown, float duration);
	~ActiveSkill() = default;

	virtual void Update() override;
	virtual void Render() = 0;
	virtual void GUIRender();

	virtual void Activate() override;
	virtual void Deactivate() override;

	virtual void LevelUp() override;

	bool IsOnCooldown() const { return curCooldown > 0.0f; }
	float GetRemainingCooldown() const { return curCooldown; }
	bool IsActive() const { return isActive; }

	virtual void UpdateChange() {};
protected:
	bool isActive = false;

	//기존 쿨다운& 지속시간
	float baseCooldown;
	float baseDuration;
	
	//쿨타임 감소효과 처리된 쿨다운 & 지속시간
	float cooldown;
	float duration;

	//현재 쿨타임
	float curCooldown = 0.0f;
	float curDuration = 0.0f;
};

