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
	Knight* player;

	Model* house;
	Terrain* terrain;
	AStar* aStar;

	ActiveSkill* skill;

	LightBuffer::Light* light;

};
