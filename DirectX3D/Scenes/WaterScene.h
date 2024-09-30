#pragma once

class WaterScene : public Scene
{
public:
	WaterScene();
	~WaterScene();

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	Skybox* skybox;
	Model* forest;
	Human* human;

	//Reflection* reflection;
	//Refraction* refraction;
	Water* water;

	Terrain* terrain;
};
