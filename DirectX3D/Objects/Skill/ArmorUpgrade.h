#pragma once

class ArmorUpgrade : public PassiveSkill
{
public:
	ArmorUpgrade();
	~ArmorUpgrade() = default;

	virtual void Activate() override;
	virtual void Deactivate() override;
	virtual void LevelUp() override;
	virtual void Update() override;
	virtual void Render() override;
private:

};
