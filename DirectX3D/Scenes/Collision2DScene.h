#pragma once

class Collision2DScene : public Scene
{
public:
	Collision2DScene();
	~Collision2DScene();

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	Collider2D* rect;
	Collider2D* circle;

	Button* btn;

	Panel* panel;
};

