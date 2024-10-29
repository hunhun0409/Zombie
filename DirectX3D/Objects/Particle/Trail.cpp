#include "Framework.h"

Trail::Trail(wstring imageFile, Transform* start, Transform* end, UINT width, float speed)
    : start(start), end(end), width(width), speed(speed)
{
    material->SetShader(L"Effect/Trail.hlsl");
    material->SetDiffuseMap(imageFile);

    CreateMesh();

    rasterizerState[0] = new RasterizerState();
    rasterizerState[1] = new RasterizerState();
    rasterizerState[1]->CullMode(D3D11_CULL_NONE);

    blendState[0] = new BlendState();
    blendState[1] = new BlendState();
    //blendState[1]->Alpha(true);
    blendState[1]->Additive();
}

Trail::~Trail()
{
    delete mesh;

    delete rasterizerState[0];
    delete rasterizerState[1];

    delete blendState[0];
    delete blendState[1];
}

void Trail::Update()
{
    if (!Active()) return;

    vector<VertexUV>& vertices = mesh->Vertices();


    // 각 선들이 앞선 선을 따라가는 방식
    for (UINT i = 0; i <= width; i++)
    {
        Vector3 startPos = vertices[i * 2].pos;
        Vector3 endPos = vertices[(i * 2) + 1].pos;

        Vector3 startDestPos;
        Vector3 endDestPos;

        if (i == 0)
        {
            // 첫 번째 선은 실제 오브젝트의 현재 위치로
            startDestPos = start->GlobalPos();
            endDestPos = end->GlobalPos();

            startPos = start->GlobalPos();
            endPos = end->GlobalPos();
        }
        else
        {
            // 나머지 선들은 바로 앞의 선을 따라감
            startDestPos = vertices[(i - 1) * 2].pos;
            endDestPos = vertices[(i - 1) * 2 + 1].pos;

            startPos = Lerp(startPos, startDestPos, speed * DELTA);
            endPos = Lerp(endPos, endDestPos, speed * DELTA);
        }
        vertices[i * 2].pos = startPos;
        vertices[i * 2 + 1].pos = endPos;
    }

    mesh->UpdateVertex();
}

void Trail::Render()
{
    if (!Active()) return;

    worldBuffer->SetVS(0);

    material->Set();

    rasterizerState[1]->SetState();
    blendState[1]->SetState();

    mesh->Draw();

    rasterizerState[0]->SetState();
    blendState[0]->SetState();
}

void Trail::GUIRender()
{
    GameObject::GUIRender();
}

void Trail::Reset()
{
    vector<VertexUV>& vertices = mesh->Vertices();

    for (UINT i = 0; i <= width; i++)
    {
        vertices[i * 2].pos = start->GlobalPos();
        vertices[i * 2 + 1].pos = end->GlobalPos();
        UpdateWorld();
    }
}

void Trail::CreateMesh()
{
    mesh = new Mesh<VertexUV>();

    vector<VertexUV>& vertices = mesh->Vertices();

    // width 갯수만큼 start와 end를 잇는 선들을 생성
    // 각 선은 2개의 정점으로 구성 (start점, end점)
    vertices.reserve((width + 1) * 2);
    for (UINT i = 0; i <= width; i++)
    {
        VertexUV vertex;
        // start 점 추가 (윗부분)
        vertex.uv = { (float)i / width, 0.0f };
        vertices.push_back(vertex);

        // end 점 추가 (아랫부분)
        vertex.uv = { (float)i / width, 1.0f };
        vertices.push_back(vertex);
    }

    vector<UINT>& indices = mesh->Indices();

    indices.reserve(width * 6);
    FOR(width)
    {
        indices.push_back(i * 2);
        indices.push_back(i * 2 + 2);
        indices.push_back(i * 2 + 1);

        indices.push_back(i * 2 + 1);
        indices.push_back(i * 2 + 2);
        indices.push_back(i * 2 + 3);
    }

    mesh->CreateMesh();
}
