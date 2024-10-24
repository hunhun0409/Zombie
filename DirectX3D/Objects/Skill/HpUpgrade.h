#pragma once

class HpUpgrade : public PassiveSkill
{
public:
	HpUpgrade();
	~HpUpgrade() = default;

	virtual void Activate() override;
	virtual void Deactivate() override;
	virtual void LevelUp() override;
	virtual void Update() override;
	virtual void Render() override;
private:

};
