#pragma once

class ZombieScene : public Scene
{
public:
	ZombieScene();
	~ZombieScene();

	// Scene��(��) ���� ��ӵ�
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

	Terrain* terrain;
	AStar* aStar;

	QuadTree* qt;
};
