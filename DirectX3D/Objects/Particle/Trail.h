#pragma once

class Trail : public GameObject
{
public:
	Trail(wstring imageFile, Transform* start, Transform* end, UINT width, float speed);
	~Trail();

	void Update();
	void Render();
	void GUIRender();

	void Reset();

private:
	void CreateMesh();

private:
	Mesh<VertexUV>* mesh;

	// start와 end 위치를 기반으로 만들어지는 선들의 집합
	Transform* start;  // 시작점 (예: 검의 시작점)
	Transform* end;    // 끝점 (예: 검의 끝점)
	UINT width;        // 잔상의 길이(선의 개수)
	float speed;       // 잔상이 따라오는 속도

	RasterizerState* rasterizerState[2];
	BlendState* blendState[2];
};

