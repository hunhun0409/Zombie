#pragma once

class ZombieScene : public Scene
{
public:
	ZombieScene();
	~ZombieScene();

	// Scene을(를) 통해 상속됨
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

	void Start() override;
	void End() override;

private:
	bool isTutorial = true;
	float stopTime = 0.5f;
	Panel* tutorialPanel;

	Knight* player;
	Knight* player2;

	Terrain* terrain;
	AStar* aStar;

	QuadTree* qt;
};
