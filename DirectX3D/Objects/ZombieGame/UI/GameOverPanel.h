#pragma once

class GameOverPanel : public Panel
{
public:
	GameOverPanel();
	~GameOverPanel() = default;

	void Update();
	void Render();

	virtual void Show(Vector3 pos = { CENTER_X, CENTER_Y }) override;
	virtual void Hide() override;

	void Retry();
private:
	
};
