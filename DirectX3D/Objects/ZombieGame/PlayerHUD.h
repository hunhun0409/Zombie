#pragma once

class PlayerHUD
{
public:
	PlayerHUD(class Character* owner);
	~PlayerHUD();

	void Update();
	void PostRender();
	void GUIRender();

	void SetOwner(class Character* owner) { this->owner = owner; }

	void UpdateExp(float ratio);
	void UpdateHp(float ratio);

private:
	Character* owner = nullptr;

	vector<Quad*> skillIcon;
	ProgressBar* expBar;
	ProgressBar* hpBar;
};
