#pragma once

class TrailEffectScene : public Scene
{
public:
	TrailEffectScene();
	~TrailEffectScene();

private:


	// Scene��(��) ���� ��ӵ�
	void Update() override;

	void PreRender() override;

	void Render() override;

	void PostRender() override;

	void GUIRender() override;

};
