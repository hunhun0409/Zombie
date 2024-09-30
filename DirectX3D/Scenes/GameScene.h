#pragma once

class GameScene : public Scene
{
public:
	GameScene();
	~GameScene();

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	void Start() override;
	void End() override;

private:
	Model* forest;

	Naruto* naruto;

	RasterizerState* rs[2];
	BlendState* blendState[2];

	Skybox* skybox;

	FogBuffer* fogBuffer;
};
