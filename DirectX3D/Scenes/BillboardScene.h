#pragma once

class BillboardScene : public Scene
{
private:
	const UINT COUNT = 1000;
public:
	BillboardScene();
	~BillboardScene();

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	//vector<Quad*> trees;

	Material* material;
	VertexBuffer* vertexBuffer;

	GeometryShader* geometryShader;
	Terrain* terrain;

	BlendState* bs[2];
	DepthStencilState* depthState[2];

	vector<VertexUV> vertices;
};
