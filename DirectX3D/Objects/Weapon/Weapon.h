#pragma once

class Weapon
{
public:
	Weapon(string name);
	~Weapon();

	virtual void Update();
	virtual void Render();
	virtual void GUIRender();

	void SetParent(Transform* transform) { mesh->SetParent(transform); }
	void SetOwner(Character* owner) { this->owner = owner; }
protected:
	Model* mesh;
	Character* owner;

	float damage = 10;
};
