#pragma once

class LevelUpPanel : public Panel
{
public:
	LevelUpPanel();
	~LevelUpPanel() = default;

	void Update();
	void Render();

	virtual void Show(Vector3 pos = {CENTER_X, CENTER_Y}) override;
	virtual void Hide() override;

	void SetSkills(vector<Skill*> skills) { upgradeSkills = skills; }

	void Select1();
	void Select2();
	void Select3();

private:
	vector<Skill*> upgradeSkills;
};

