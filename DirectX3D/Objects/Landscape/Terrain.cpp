#include "Framework.h"

Terrain::Terrain() : GameObject(L"Landscape/terrain.hlsl"), width(256), height(256)
{
    SetTag("Terrain");

    //material->Load("TextData/MAterials/First.mat");
    //material->SetShader(L"Landscape/Terrain1.hlsl");

    material->SetDiffuseMap(L"Textures/Landscape/Dirt2.png");
    //material->SetSpecularMap(L"Textures/Landscape/Fieldstone_SM.tga");
    //material->SetNormalMap(L"Textures/Landscape/Fieldstone_NM.tga");

    heightMap = Texture::Add(L"Textures/HeightMaps/AStarMap.png");
    //alphaMap = Texture::Add(L"Textures/AlphaMaps/TestAlphaMap.png");
    //secondMap = Texture::Add(L"Textures/Landscape/Dirt.png");
    //thirdMap = Texture::Add(L"Textures/Landscape/Dirt3.png");

    mesh = new Mesh<VertexType>();
    MakeMesh();
    MakeNormal();
    MakeTangent();
    mesh->CreateMesh();
}

Terrain::Terrain(string name)
{
    string file = "TextData/Terrains/" + name + ".terrain";

    Load(file);
    material->SetShader(L"Landscape/Terrain.hlsl");

    SetTag(name);

    mesh = new Mesh<VertexType>();
    MakeMesh();
    MakeNormal();
    MakeTangent();
    mesh->CreateMesh();
}

Terrain::~Terrain()
{
    delete mesh;
}

void Terrain::Render()
{
    GameObject::SetRender();

    /*wstring path = alphaMap->GetFile();
    alphaMap->PSSet(10);
    maps[1]->PSSet(11);
    maps[2]->PSSet(12);
    maps[3]->PSSet(13);

    normalMaps[1]->PSSet(14);
    normalMaps[2]->PSSet(15);
    normalMaps[3]->PSSet(16);*/

    mesh->Draw();
}

float Terrain::GetHeight(const Vector3& pos, Vector3* normal)
{
    int x = (int)pos.x;
    int z = (int)(height - pos.z);

    if (x < 0 || x >= width - 1)return 0.0f;
    if (z < 0 || z >= width - 1)return 0.0f;

    UINT index[4];

    index[0] = width * z + x;
    index[1] = width * (z + 1) + x;
    index[2] = width * z + x + 1;
    index[3] = width * (z + 1) + x + 1;

    vector<VertexType> vertices = mesh->Vertices();

    Vector3 p[4];
    for (UINT i = 0; i < 4; i++)
        p[i] = vertices[index[i]].pos;

    float u = pos.x - p[0].x;
    float v = pos.z - p[0].z;

    Vector3 result;

    if (u + v <= 1.0f)
    {
        result = ((p[2] - p[0]) * u + (p[1] - p[0]) * v) + p[0];

        if (normal)
        {
            (*normal) = GetNormalFromPolygon(p[0], p[1], p[2]);
        }
        return result.y + 1;
    }
    else
    {
        result = ((p[1] - p[3]) * (1 - u) + (p[2] - p[3]) * (1 - v)) + p[0];

        if (normal)
        {
            (*normal) = GetNormalFromPolygon(p[3], p[1], p[2]);
        }
        return result.y + 1;
    }


    return 0.0f;
}

Vector3 Terrain::GetNormal(Vector2 pos)
{
    return Vector3();
}

Vector3 Terrain::Picking()
{
     Ray ray = CAM->ScreenPointToRay(mousePos);

     vector<VertexType> vertices = mesh->Vertices();

     for (UINT z = 0; z < height - 1; z++)
     {
         for (UINT x = 0; x < width - 1; x++)
         {
             UINT index[4];
             index[0] = width * z + x;
             index[1] = width * z + x + 1;
             index[2] = width * (z+1) + x;
             index[3] = width * (z+1) + x + 1;

             Vector3 p[4];
             for (UINT i = 0; i < 4; i++)
                 p[i] = vertices[index[i]].pos;

             float distance = 0.0f;
             if (Intersects(ray.pos, ray.dir, p[0], p[1], p[2], distance))
             {
                 return ray.pos + ray.dir * distance;
             }
             if (Intersects(ray.pos, ray.dir, p[3], p[2], p[1], distance))
             {
                 return ray.pos + ray.dir * distance;
             }
         }
     }
     
     return Vector3();
}

void Terrain::Load(string file)
{
    tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
    document->LoadFile(file.c_str());
    tinyxml2::XMLElement* map = document->FirstChildElement();
    name = map->Attribute("Name");

    tinyxml2::XMLElement* materials = map->FirstChildElement();
    tinyxml2::XMLElement* material1 = materials->FirstChildElement();
    material->Load(material1->Attribute("Material1"));

    tinyxml2::XMLElement* Maps = materials->NextSiblingElement();
    tinyxml2::XMLElement* secondMap = Maps->FirstChildElement();
    tinyxml2::XMLElement* seconddiffusemap = secondMap->FirstChildElement();
    maps[1] = Texture::Add(ToWString(seconddiffusemap->Attribute("DiffuseMap")));
    tinyxml2::XMLElement* secondnormalmap = seconddiffusemap->NextSiblingElement();
    normalMaps[1] = Texture::Add(ToWString(secondnormalmap->Attribute("NormalMap")));

    tinyxml2::XMLElement* thirdMap = secondMap->NextSiblingElement();
    tinyxml2::XMLElement* thirddiffusemap = thirdMap->FirstChildElement();
    maps[2] = Texture::Add(ToWString(thirddiffusemap->Attribute("DiffuseMap")));
    tinyxml2::XMLElement* thirdnormalmap = thirddiffusemap->NextSiblingElement();
    normalMaps[2] = Texture::Add(ToWString(thirdnormalmap->Attribute("NormalMap")));

    tinyxml2::XMLElement* fourthMap = thirdMap->NextSiblingElement();
    tinyxml2::XMLElement* fourthdiffusemap = fourthMap->FirstChildElement();
    maps[3] = Texture::Add(ToWString(fourthdiffusemap->Attribute("DiffuseMap")));
    tinyxml2::XMLElement* fourthnormalmap = fourthdiffusemap->NextSiblingElement();
    normalMaps[3] = Texture::Add(ToWString(fourthnormalmap->Attribute("NormalMap")));

    tinyxml2::XMLElement* heightmap = fourthMap->NextSiblingElement();
    heightMap = Texture::Add(ToWString(heightmap->Attribute("HeightMap")));

    tinyxml2::XMLElement* alphamap = heightmap->NextSiblingElement();
    alphaMap = Texture::Add(ToWString(alphamap->Attribute("AlphaMap")));

    delete document;
}

void Terrain::MakeMesh()
{
    width = (UINT)heightMap->GetSize().x;
    height = (UINT)heightMap->GetSize().y;

    vector<Float4> pixels;
    heightMap->ReadPixels(pixels);

    //Vertices
    vector<VertexType>& vertices = mesh->Vertices();

    vertices.reserve(width * height);
    for (UINT z = 0; z < height; z++)
    {
        for (UINT x = 0; x < width; x++)
        {
            VertexType vertex;
            vertex.pos = { (float)x, 0.0f, (float)(height - z - 1) };
            vertex.uv.x = x / (float)(width - 1);
            vertex.uv.y = z / (float)(height - 1);

            UINT index = width * z + x;
            vertex.pos.y = pixels[index].x * MAX_HEIGHT;

            vertices.push_back(vertex);
        }
    }

    //Indices
    vector<UINT>& indices = mesh->Indices();

    indices.reserve((width - 1) * (height - 1) * 6);

    for (UINT z = 0; z < height - 1; z++)
    {
        for (UINT x = 0; x < width - 1; x++)
        {
            indices.push_back(width * z + x);//0
            indices.push_back(width * z + x + 1);//2
            indices.push_back(width * (z + 1) + x);//1

            indices.push_back(width * z + x + 1);//2
            indices.push_back(width * (z + 1) + x + 1);//3
            indices.push_back(width * (z + 1) + x);//1

        }
    }
}

void Terrain::MakeNormal()
{
    vector<VertexType>& vertices = mesh->Vertices();
    vector<UINT> indices = mesh->Indices();

    for (UINT i = 0; i < indices.size() / 3; i++)
    {
        UINT index0 = indices[i * 3 + 0];
        UINT index1 = indices[i * 3 + 1];
        UINT index2 = indices[i * 3 + 2];

        Vector3 v0 = vertices[index0].pos;
        Vector3 v1 = vertices[index1].pos;
        Vector3 v2 = vertices[index2].pos;

        Vector3 e0 = v1 - v0;
        Vector3 e1 = v2 - v0;

        Vector3 normal = Cross(e0, e1).GetNormalized();

        vertices[index0].normal = normal + vertices[index0].normal;
        vertices[index1].normal = normal + vertices[index1].normal;
        vertices[index2].normal = normal + vertices[index2].normal;
    }
}

void Terrain::MakeTangent()
{
    vector<VertexType>& vertices = mesh->Vertices();
    vector<UINT> indices = mesh->Indices();

    for (UINT i = 0; i < indices.size() / 3; i++)
    {
        UINT index0 = indices[i * 3 + 0];
        UINT index1 = indices[i * 3 + 1];
        UINT index2 = indices[i * 3 + 2];

        Vector3 p0 = vertices[index0].pos;
        Vector3 p1 = vertices[index1].pos;
        Vector3 p2 = vertices[index2].pos;

        Vector3 e0 = p1 - p0;
        Vector3 e1 = p2 - p0;

        Vector2 uv0 = vertices[index0].uv;
        Vector2 uv1 = vertices[index1].uv;
        Vector2 uv2 = vertices[index2].uv;

        float u0 = uv1.x - uv0.x;
        float v0 = uv1.y - uv0.y;
        float u1 = uv2.x - uv0.x;
        float v1 = uv2.y - uv0.y;

        float det = 1 / (u0 * v1 - u1 * v0);

        Vector3 tangent = det * (v1 * e0 - v0 * e1);

        vertices[index0].tangent += tangent;
        vertices[index1].tangent += tangent;
        vertices[index2].tangent += tangent;
    }

}

