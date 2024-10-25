#include "Framework.h"
#include "DeferredScene.h"

DeferredScene::DeferredScene()
{
	gBuffer = new GBuffer();
	material = new Material(L"Light/Deferred.hlsl");
	UINT vertices[4] = { 0, 1, 2, 3 };
	vertexBuffer = new VertexBuffer(vertices, sizeof(UINT), 4);

	player = new Mannequin();
	player->Pos() = { 00, 0, 00 };

	skybox = new Skybox(L"Textures/skybox/Night.png");

	sky = Texture::Add(L"Textures/skybox/Night.png");

	terrain = new Terrain("test");
	terrain->Pos() = { -50, 0, -50 };
	terrain->UpdateWorld();

	blendState[0] = new BlendState();
	blendState[1] = new BlendState();
	blendState[1]->AlphaToCoverage(true);
}

DeferredScene::~DeferredScene()
{
	delete gBuffer;

	delete player;

	delete material;
	delete vertexBuffer;

	delete skybox;

	delete terrain;

	delete blendState[0];
	delete blendState[1];
}

void DeferredScene::Update()
{
	player->Update();
	terrain->UpdateWorld();
}

void DeferredScene::PreRender()
{
	gBuffer->SetMultiRenderTarget();

	terrain->SetShader(L"Light/GBuffer.hlsl");
	player->SetShader(L"Light/GBuffer.hlsl");
	player->GetWeapon()->SetShader(L"Light/GBuffer.hlsl");

	terrain->Render();
	player->Render();
}

void DeferredScene::Render()
{
	sky->PSSet(20);
	skybox->Render();

	blendState[1]->SetState();

	vertexBuffer->Set(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	gBuffer->SetSRVs();
	material->Set();

	DC->Draw(4, 0);

	blendState[0]->SetState();
}

void DeferredScene::PostRender()
{
	//gBuffer->PostRender();
}

void DeferredScene::GUIRender()
{
	player->GUIRender();
	terrain->GUIRender();
}
