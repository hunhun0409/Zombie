#pragma once

class DeferredScene : public Scene
{
public:
	DeferredScene();
	~DeferredScene();

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	GBuffer* gBuffer;

	Mannequin* player;

	Material* material;
	VertexBuffer* vertexBuffer;

	Skybox* skybox;

	Texture* sky;

	Terrain* terrain;

	BlendState* blendState[2];

	Shadow* shadow;
};