#pragma once

class LevelUpButton : public Button
{
public:
	struct UpgradeData
	{
		string title;
		string description;
		wstring iconTextureFile;
	};

public:
	LevelUpButton();
	~LevelUpButton();
	
	virtual void Update() override;
	virtual void Render() override;
	virtual void GUIRender() override;


	void SetData(UpgradeData data);


	virtual void Load() override;
private:
	Quad* icon;

	UpgradeData data;

	Vector2 titleOffset;
	Vector2 descriptionOffset;
};
