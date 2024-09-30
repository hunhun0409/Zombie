#pragma once

class QuadTreeScene : public Scene
{
public:
	QuadTreeScene();
	~QuadTreeScene();

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	QuadTreeTerrain* terrain;
};
