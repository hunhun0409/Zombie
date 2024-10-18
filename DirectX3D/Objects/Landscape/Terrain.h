#pragma once

class Terrain : public GameObject
{
private:
    typedef VertexUVNormalTangent VertexType;
    const float MAX_HEIGHT = 20.0f;

public:
    Terrain();
    Terrain(string name);
    ~Terrain();

    void Render();

public:
    float GetHeight(const Vector3& pos, Vector3* normal = nullptr);
    Vector2 GetSize() { return Vector2( width * Scale().x, height * Scale().z); }
    Vector3 GetNormal(Vector2 pos);

    Vector3 Picking();

    void Load(string file);

    void SetShader(wstring file) { GetMaterial()->SetShader(file); }
private:

    void MakeMesh();
    void MakeNormal();
    void MakeTangent();

private:
    string name;

    UINT width, height;

    Mesh<VertexType>* mesh;

    Texture* heightMap;
    Texture* alphaMap;
    Texture* maps[4];
    Texture* normalMaps[4];
};