#pragma once

class ParticleScene : public Scene
{
public:
	ParticleScene();
	~ParticleScene();

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	ParticleSystem* particle;

	SphereCollider* collider;
};
