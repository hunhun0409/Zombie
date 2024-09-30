#pragma once

class ShadowScene : public Scene
{
public:
	ShadowScene();
	~ShadowScene();

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	Model* forest;
	Mannequin* player;

	Shadow* shadow;
};
