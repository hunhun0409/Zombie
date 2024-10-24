#pragma once

class PassiveSkill : public Skill
{
public:
	PassiveSkill(SkillInfo info, float baseEffectValue, float effectPerLevel);
	PassiveSkill(string id, string name, wstring iconPath, float baseEffectValue, float effectPerLevel);
	~PassiveSkill() = default;

	virtual void Activate() override;
	virtual void Deactivate() override;
	virtual void LevelUp() override;
	virtual void Update() override;
	virtual void Render() override;
private:
	float baseEffectValue; // 기본 효과 수치
	float effectPerLevel;//레벨당 효과 증가량

	float effectValue;//최종 수치 : 기본 효과 수치 + 레벨당 효과 증가량 * 레벨-1;
};
