#include "Framework.h"

Skybox::Skybox(wstring textureFile)
{
	material->SetShader(L"Landscape/SkyboxPanormaic.hlsl");

	cubeMap = Texture::Add(textureFile);

	rasterizerState[0] = new RasterizerState();
	rasterizerState[1] = new RasterizerState();
	rasterizerState[1]->FrontCounterClockwise(true);

	depthStencilState[0] = new DepthStencilState();
	depthStencilState[1] = new DepthStencilState();
	depthStencilState[1]->DepthEnable(false);
}

Skybox::~Skybox()
{
	delete rasterizerState[0];
	delete rasterizerState[1];

	delete depthStencilState[0];
	delete depthStencilState[1];
}

void Skybox::Render()
{
	rasterizerState[1]->SetState();
	depthStencilState[1]->SetState();

	cubeMap->PSSet(20);

	Sphere::Render();

	rasterizerState[0]->SetState();
	depthStencilState[0]->SetState();
}

void Skybox::SetShader(wstring file)
{
	material->SetShader(file);
}
