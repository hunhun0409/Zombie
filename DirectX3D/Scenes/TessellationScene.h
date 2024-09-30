#pragma once

class TessellationScene : public Scene
{
public:
	TessellationScene();
	~TessellationScene();

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	Material* material;
	HullShader* hullShader;
	DomainShader* domainShader;

	Mesh<Vertex>* mesh;

	FloatValueBuffer* edgeBuffer;
	FloatValueBuffer* insideBuffer;
};
