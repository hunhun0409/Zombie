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
protected:
	Model* mesh;

	float damage = 10;
};
