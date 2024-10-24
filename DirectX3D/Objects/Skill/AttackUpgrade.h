#pragma once

class AttackUpgrade : public PassiveSkill
{
public:
	AttackUpgrade();
	~AttackUpgrade() = default;

	virtual void Activate() override;
	virtual void Deactivate() override;
	virtual void LevelUp() override;
	virtual void Update() override;
	virtual void Render() override;
private:

};
