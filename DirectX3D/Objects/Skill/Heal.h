#pragma once

class Heal : public ActiveSkill
{
public:
	Heal();
	~Heal() = default;

	virtual void Update() override;
	virtual void Render() override;
	virtual void GUIRender();

	virtual void Activate() override;
	virtual void Deactivate() override;

	virtual void LevelUp() override;

private:

};