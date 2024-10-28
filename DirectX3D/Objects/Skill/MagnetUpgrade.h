#pragma once

class MagnetUpgrade : public PassiveSkill
{
public:
	MagnetUpgrade();
	~MagnetUpgrade() = default;

	virtual void Activate() override;
	virtual void Deactivate() override;
	virtual void LevelUp() override;
	virtual void Update() override;
	virtual void Render() override;
private:

};
