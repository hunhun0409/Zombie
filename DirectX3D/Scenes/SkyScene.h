#pragma once

class SkyScene : public Scene
{
public:
	SkyScene();
	~SkyScene();

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	Skybox* skybox;
};
