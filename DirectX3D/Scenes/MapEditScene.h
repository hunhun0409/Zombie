#pragma once

class MapEditScene : public Scene
{
public:
	MapEditScene();
	~MapEditScene();

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:

};
