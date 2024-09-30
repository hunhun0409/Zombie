#pragma once

class Skybox : public Sphere
{
public:
	Skybox(wstring textureFile);
	~Skybox();

	void Render();

	void SetShader(wstring file);

private:
	Texture* cubeMap;
	RasterizerState* rasterizerState[2];
	DepthStencilState* depthStencilState[2];
};
