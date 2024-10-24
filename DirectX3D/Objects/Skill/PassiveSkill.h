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
	float baseEffectValue; // �⺻ ȿ�� ��ġ
	float effectPerLevel;//������ ȿ�� ������

	float effectValue;//���� ��ġ : �⺻ ȿ�� ��ġ + ������ ȿ�� ������ * ����-1;
};
