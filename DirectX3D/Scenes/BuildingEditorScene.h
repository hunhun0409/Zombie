#pragma once

class BuildingEditorScene : public Scene
{
public:
	BuildingEditorScene();
	~BuildingEditorScene();

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:

};
