#pragma once

class RecoveryUpgrade : public PassiveSkill
{
public:
	RecoveryUpgrade();
	~RecoveryUpgrade() = default;


	virtual void Activate() override;
	virtual void Deactivate() override;
	virtual void LevelUp() override;
	virtual void Update() override;
	virtual void Render() override;
private:

};
