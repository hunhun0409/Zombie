#pragma once

class BaseballBat : public Model
{
public:
	BaseballBat();
	~BaseballBat();

	void Update();
	void Render();
	void GUIRender();

private:
	BoxCollider* collider;
};
