#pragma once

class Weapon
{
public:
	Weapon(string name);
	~Weapon();

	virtual void Update();
	virtual void Render();
	virtual void GUIRender();


	Model* GetMesh() { return mesh; }
	void SetParent(Transform* transform) { mesh->SetParent(transform); }
	void SetOwner(class Character* owner) { this->owner = owner; }

	float& Damage() { return damage; }
protected:
	Model* mesh;
	class Character* owner;

	float damage = 10;
};
