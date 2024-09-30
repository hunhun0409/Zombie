#pragma once

class ScatteringScene : public Scene
{
public:
	ScatteringScene();
	~ScatteringScene();

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	Scattering* scattering;

};

