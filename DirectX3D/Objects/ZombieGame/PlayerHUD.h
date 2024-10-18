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

private:
	Character* owner = nullptr;

	vector<Quad*> skillIcon;
	ProgressBar* ExpBar;
	float amount;
};
