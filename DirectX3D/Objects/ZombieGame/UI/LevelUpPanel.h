#pragma once

class LevelUpPanel : public Panel
{
public:
	LevelUpPanel();
	~LevelUpPanel();

	void Update();
	void Render();

	virtual void Show(Vector3 pos = {CENTER_X, CENTER_Y}) override;

private:

};

