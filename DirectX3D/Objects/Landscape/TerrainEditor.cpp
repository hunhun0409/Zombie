#include "Framework.h"

TerrainEditor::TerrainEditor()
    :GameObject(L"Landscape/TerrainEditor.hlsl"), width(MAX_SIZE), height(MAX_SIZE)
{
    tag = "TerrainEditor";

    material->SetDiffuseMap(L"Textures/Terrain/grass_d.tga");
    material->SetNormalMap(L"Textures/Terrain/grass_n.tga");
    
    maps[0] = material->GetDiffuseMap();
    maps[1] = Texture::Add(L"Textures/Terrain/ground_d.tga");
    maps[2] = Texture::Add(L"Textures/Terrain/grass1_d.tga");
    maps[3] = Texture::Add(L"Textures/Terrain/grass2_d.tga");

    normalMaps[0] = material->GetNormalMap();
    normalMaps[1] = Texture::Add(L"Textures/Terrain/ground_n.tga");
    normalMaps[2] = Texture::Add(L"Textures/Terrain/grass1_n.tga");
    normalMaps[3] = Texture::Add(L"Textures/Terrain/grass2_n.tga");

    mesh = new Mesh<VertexType>();
    MakeMesh();
    MakeNormal();
    MakeTangent();
    MakeComputeData();
    mesh->CreateMesh();

    computeShader = Shader::AddCS(L"Compute/ComputePicking.hlsl");
    brushBuffer = new BrushBuffer();
    rayBuffer = new RayBuffer();

    structuredBuffer = new StructuredBuffer(
        inputs.data(),
        sizeof(InputDesc), triangleSize,
        sizeof(OutputDesc), triangleSize);

    char path[128];
    GetCurrentDirectoryA(128, path);
    projectPath = path;
}

TerrainEditor::~TerrainEditor()
{
    delete mesh;
    delete brushBuffer;
    delete rayBuffer;
    delete structuredBuffer;
}

void TerrainEditor::Update()
{
    if (ComputePicking(pickingPos))
    {
        brushBuffer->Get().pickingPos = pickingPos;
    }
    else
    {
        return;
    }
    if (KEY_PRESS(VK_LBUTTON) && !ImGui::GetIO().WantCaptureMouse)
    {
        switch (editType)
        {
        case TerrainEditor::HEIGHT:
            AdjustHeight();
            break;
        case TerrainEditor::ALPHA:
            AdjustAlpha();
            break;
        default:
            break;
        }
    }
    if (KEY_UP(VK_LBUTTON))
    {
        UpdateHeight();
    }

    UpdateWorld();
}

void TerrainEditor::Render()
{
    GameObject::SetRender();

    brushBuffer->SetPS(10);

    maps[1]->PSSet(11);
    maps[2]->PSSet(12);
    maps[3]->PSSet(13);

    normalMaps[1]->PSSet(14);
    normalMaps[2]->PSSet(15);
    normalMaps[3]->PSSet(16);

    mesh->Draw();
}

void TerrainEditor::GUIRender()
{
    ImGui::Text("MapName");

    string title = name + "_Map";
    char str[128];
    strcpy_s(str, 128, editName.c_str());
    ImGui::PushItemWidth(200);
    ImGui::InputText("Name", str, 128);
    ImGui::PopItemWidth();
    editName = str;

    ImGui::SameLine();
    if (ImGui::Button("Edit"))
        name = editName;

    ImGui::Text("TerrainEdit Option");
    if (ImGui::SliderInt("Width", (int*)&width, 2, MAX_SIZE))
        Resize();
    if (ImGui::SliderInt("Height", (int*)&height, 2, MAX_SIZE))
        Resize();

    const char* editList[] = { "Height", "Alpha" };
    ImGui::Combo("EditType", (int*)&editType, editList, 2);

    const char* brushList[] = { "Circle", "Rect" };
    if (ImGui::Combo("BrushType", (int*)&brushType, brushList, 2))
    {
        brushBuffer->Get().type = brushType;
    }
    const char* heightEditList[] = { "Solid", "Smooth" };
    ImGui::Combo("HeightEditType", (int*)&brushHardness, heightEditList, 2);

    ImGui::SliderFloat("Range", (float*)&brushBuffer->Get().range, 1, 40);
    ImGui::SliderFloat("AdjustValue", (float*)&adjustValue, -50.0f, 50.0f);
    ImGui::ColorEdit3("Color", (float*)&brushBuffer->Get().color);

    ImGui::SliderInt("SelectMap", (int*)&selectMap, 0, 3);

    FOR(4)
    {
        SelectMap(to_string(i), i);
        if (i %2 == 0)
        {
            ImGui::SameLine();
        }
    }

    //material->GUIRender();

    //SaveHeightMap();
    //ImGui::SameLine();
    //LoadHeightMap();

//    //SaveAlphaMap();
    //ImGui::SameLine();
    //LoadAlphaMap();

    SaveDialog();
    LoadDialog();
}

void TerrainEditor::MakeMesh()
{
    vector<Float4> pixels(width * height, Float4(0, 0, 0, 0));

    if (heightMap)
    {
        width = (UINT)heightMap->GetSize().x;
        height = (UINT)heightMap->GetSize().y;

        heightMap->ReadPixels(pixels);

    }
    //Vertices
    vector<VertexType>& vertices = mesh->Vertices();
    vertices.clear();


    vertices.reserve(width * height);
    for (UINT z = 0; z < height; z++)
    {
        for (UINT x = 0; x < width; x++)
        {
            VertexType vertex;
            vertex.pos = { (float)x, 0.0f, (float)(height - z - 1) };
            vertex.uv.x = x / (float)(width - 1) * uvScale;
            vertex.uv.y = z / (float)(height - 1) * uvScale;

            UINT index = width * z + x;
            vertex.pos.y = MIN_HEIGHT + pixels[index].x * (MAX_HEIGHT - MIN_HEIGHT);

            vertex.alpha[0] = 1.0f;
            vertices.push_back(vertex);
        }
    }

    //Indices
    vector<UINT>& indices = mesh->Indices();
    indices.clear();

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

void TerrainEditor::MakeNormal()
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

void TerrainEditor::MakeTangent()
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

void TerrainEditor::MakeComputeData()
{
    vector<VertexType>& vertices = mesh->Vertices();
    vector<UINT> indices = mesh->Indices();


    triangleSize = indices.size() / 3;

    inputs.resize(triangleSize);
    outputs.resize(triangleSize);

    for (UINT i = 0; i < triangleSize; i++)
    {
        UINT index0 = indices[i * 3 + 0];
        UINT index1 = indices[i * 3 + 1];
        UINT index2 = indices[i * 3 + 2];

        inputs[i].v0 = vertices[index0].pos;
        inputs[i].v1 = vertices[index1].pos;
        inputs[i].v2 = vertices[index2].pos;

    }
}

Vector3 TerrainEditor::Picking()
{
    /* Ray ray = CAM->ScreenPointToRay(mousePos);

     for (UINT z = 0; z < height - 1; z++)
     {
         for (UINT x = 0; x < width - 1; x++)
         {
             UINT index[4];
             index[0] = width * z + x;
             index[1] = width * z + x + 1;
             index[2] = width * (z+1) + x;
             index[3] = width * (z+1) + x + 1;

             vector<VertexType> vertices = mesh->Vertices();

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
     */
    return 0.0f;
}

bool TerrainEditor::ComputePicking(Vector3& pos)
{
    Ray ray = CAM->ScreenPointToRay(mousePos);

    Vector3 a = ray.pos;
    Vector3 b = ray.dir;
    rayBuffer->Get().pos = ray.pos;
    rayBuffer->Get().dir = ray.dir;
    rayBuffer->Get().triangleSize = triangleSize;

    rayBuffer->SetCS(0);

    DC->CSSetShaderResources(0, 1, &structuredBuffer->GetSRV());
    DC->CSSetUnorderedAccessViews(0, 1, &structuredBuffer->GetUAV(), nullptr);

    computeShader->Set();

    UINT x = ceil((float)triangleSize / 64.0f);

    DC->Dispatch(x, 1, 1);

    structuredBuffer->Copy(outputs.data(), sizeof(OutputDesc) * triangleSize);


    float minDistance = FLT_MAX;
    int minIndex = -1;

    UINT index = 0;


    for (OutputDesc output : outputs)
    {
        if (output.picked)
        {
            if (minDistance > output.distance)
            {
                minDistance = output.distance;
                minIndex = index;
            }
        }
        index++;
    }

    if (minIndex >= 0)
    {
        pos = ray.pos + ray.dir * minDistance;
        return true;
    }

    return false;
}

void TerrainEditor::Resize()
{
    MakeMesh();
    MakeNormal();
    MakeTangent();
    MakeComputeData();

    mesh->UpdateVertex();
    mesh->UpdateIndex();
    structuredBuffer->UpdateInput(inputs.data());
}

void TerrainEditor::UpdateHeight()
{
    vector<VertexType>& vertices = mesh->Vertices();
    for (VertexType& vertex : vertices)
    {
        vertex.normal = {};
        vertex.tangent = {};
    }

    MakeNormal();
    MakeTangent();
    MakeComputeData();

    mesh->UpdateVertex();
    structuredBuffer->UpdateInput(inputs.data());
}

void TerrainEditor::AdjustHeight()
{
    vector<VertexType>& vertices = mesh->Vertices();

    switch (brushType)
    {
    case TerrainEditor::CIRCLE:
        if (brushHardness == SOLID)
        {
            for (VertexType& vertex : vertices)
            {
                Vector3 pos = Vector3(vertex.pos.x, 0, vertex.pos.z);
                pickingPos.y = 0.0f;

                float distance = Distance(pos, pickingPos);

                if (distance < brushBuffer->Get().range)
                {
                    vertex.pos.y += adjustValue * DELTA;
                    vertex.pos.y = Clamp(MIN_HEIGHT, MAX_HEIGHT, vertex.pos.y);
                }
            }
        }
        else if (brushHardness == SMOOTH)
        {
            for (VertexType& vertex : vertices)
            {
                Vector3 pos = Vector3(vertex.pos.x, 0, vertex.pos.z);
                pickingPos.y = 0.0f;

                float distance = Distance(pos, pickingPos);

                float  temp = adjustValue * max(0, pow(cos(distance / brushBuffer->Get().range), 3));

                if (distance < brushBuffer->Get().range)
                {
                    vertex.pos.y += temp * DELTA;
                    vertex.pos.y = Clamp(MIN_HEIGHT, MAX_HEIGHT, vertex.pos.y);
                }

            }
        }
        break;
    case TerrainEditor::SQUARE:
    {
        if (brushHardness == SOLID)
        {
            float size = brushBuffer->Get().range * 0.5f;

            float left = max(0, pickingPos.x - size);
            float right = max(0, pickingPos.x + size);
            float top = max(0, pickingPos.z + size);
            float bottom = max(0, pickingPos.z - size);

            for (UINT z = (UINT)bottom; z <= (UINT)top; z++)
            {
                for (UINT x = (UINT)left; x <= (UINT)right; x++)
                {
                    UINT index = width * (height - z - 1) + x;

                    if (index >= vertices.size()) continue;

                    vertices[index].pos.y += adjustValue * DELTA;
                    vertices[index].pos.y = Clamp(MIN_HEIGHT, MAX_HEIGHT, vertices[index].pos.y);
                }
            }
        }

        else if (brushHardness == SMOOTH)
        {
            float size = brushBuffer->Get().range * 0.5f;

            float left = max(0, pickingPos.x - size);
            float right = max(0, pickingPos.x + size);
            float top = max(0, pickingPos.z + size);
            float bottom = max(0, pickingPos.z - size);

            for (UINT z = (UINT)bottom; z <= (UINT)top; z++)
            {
                for (UINT x = (UINT)left; x <= (UINT)right; x++)
                {
                    UINT index = width * (height - z - 1) + x;

                    if (index >= vertices.size()) continue;

                    float distance = Distance(vertices[index].pos, pickingPos);

                    float temp = adjustValue * max(0, pow(cos(distance / brushBuffer->Get().range), 3));

                    vertices[index].pos.y += temp * DELTA;
                    vertices[index].pos.y = Clamp(MIN_HEIGHT, MAX_HEIGHT, vertices[index].pos.y);
                }
            }
        }
    }
    break;
    }

    //UpdateHeight();
    mesh->UpdateVertex();

}

void TerrainEditor::AdjustAlpha()
{
    vector<VertexType>& vertices = mesh->Vertices();

    switch (brushType)
    {
    case TerrainEditor::CIRCLE:
        if (brushHardness == SOLID)
        {
            for (VertexType& vertex : vertices)
            {
                Vector3 pos = Vector3(vertex.pos.x, 0, vertex.pos.z);
                pickingPos.y = 0.0f;

                float distance = Distance(pos, pickingPos);

                if (distance < brushBuffer->Get().range)
                {
                    // 선택된 맵의 알파 값 증가
                    float alphaChange = adjustValue * DELTA;

                    // 현재 selectMap의 알파 값을 증가시킴
                    vertex.alpha[selectMap] += alphaChange;
                    vertex.alpha[selectMap] = Clamp(0.0f, 1.0f, vertex.alpha[selectMap]);

                    // 나머지 알파 값들의 합을 계산
                    float otherAlphasTotal = 0.0f;
                    for (int i = 0; i < 4; i++)
                    {
                        if (i != selectMap)
                        {
                            otherAlphasTotal += vertex.alpha[i];
                        }
                    }

                    // 전체 알파 값이 1을 넘지 않도록 조정
                    if (otherAlphasTotal > 0.0f)
                    {
                        // selectMap을 제외한 나머지 알파 값을 비율에 따라 감소시킴
                        float remainingAlpha = 1.0f - vertex.alpha[selectMap];
                        float scaleFactor = remainingAlpha / otherAlphasTotal;

                        for (int i = 0; i < 4; i++)
                        {
                            if (i != selectMap)
                            {
                                vertex.alpha[i] *= scaleFactor;  // 비율에 따라 감소
                                vertex.alpha[i] = Clamp(0.0f, 1.0f, vertex.alpha[i]);
                            }
                        }
                    }
                }
            }
        }
        else if (brushHardness == SMOOTH)
        {
            for (VertexType& vertex : vertices)
            {
                Vector3 pos = Vector3(vertex.pos.x, 0, vertex.pos.z);
                pickingPos.y = 0.0f;

                float distance = Distance(pos, pickingPos);

                float  temp = adjustValue * max(0, pow(cos(distance / brushBuffer->Get().range), 3));

                if (distance < brushBuffer->Get().range)
                {
                    vertex.alpha[selectMap] += temp * DELTA;
                    vertex.alpha[selectMap] = Clamp(0.0f, 1.0f, vertex.alpha[selectMap]);
                }

            }
        }
        break;
    case TerrainEditor::SQUARE:
    {
        if (brushHardness == SOLID)
        {
            float size = brushBuffer->Get().range * 0.5f;

            float left = max(0, pickingPos.x - size);
            float right = max(0, pickingPos.x + size);
            float top = max(0, pickingPos.z + size);
            float bottom = max(0, pickingPos.z - size);

            for (UINT z = (UINT)bottom; z <= (UINT)top; z++)
            {
                for (UINT x = (UINT)left; x <= (UINT)right; x++)
                {
                    UINT index = width * (height - z - 1) + x;

                    if (index >= vertices.size()) continue;

                    vertices[index].alpha[selectMap] += adjustValue * DELTA;
                    vertices[index].alpha[selectMap] = Clamp(0.0f, 1.0f, vertices[index].alpha[selectMap]);
                }
            }
        }

        else if (brushHardness == SMOOTH)
        {
            float size = brushBuffer->Get().range * 0.5f;

            float left = max(0, pickingPos.x - size);
            float right = max(0, pickingPos.x + size);
            float top = max(0, pickingPos.z + size);
            float bottom = max(0, pickingPos.z - size);

            for (UINT z = (UINT)bottom; z <= (UINT)top; z++)
            {
                for (UINT x = (UINT)left; x <= (UINT)right; x++)
                {
                    UINT index = width * (height - z - 1) + x;

                    if (index >= vertices.size()) continue;

                    float distance = Distance(vertices[index].pos, pickingPos);

                    float temp = adjustValue * max(0, pow(cos(distance / brushBuffer->Get().range), 3));

                    vertices[index].alpha[selectMap] += temp * DELTA;
                    vertices[index].alpha[selectMap] = Clamp(0.0f, 1.0f, vertices[index].alpha[selectMap]);
                }
            }
        }
    }
    break;
    }
    mesh->UpdateVertex();
}

void TerrainEditor::SaveHeight(string file)
{
    UINT size = width * height * 4;
    uint8_t* pixels = new uint8_t[size];

    vector<VertexType> vertices = mesh->Vertices();

    for (UINT i = 0; i < size / 4; i++)
    {
        float y = vertices[i].pos.y;

        uint8_t height = (y - MIN_HEIGHT) / (MAX_HEIGHT - MIN_HEIGHT) * 255;

        pixels[i * 4 + 0] = height;
        pixels[i * 4 + 1] = height;
        pixels[i * 4 + 2] = height;
        pixels[i * 4 + 3] = 255;
    }

    Image image;
    image.width = width;
    image.height = height;
    image.format = DXGI_FORMAT_R8G8B8A8_UNORM;
    image.rowPitch = width * 4;
    image.slicePitch = size;
    image.pixels = pixels;

    SaveToWICFile(image, WIC_FLAGS_FORCE_RGB, GetWICCodec(WIC_CODEC_PNG), ToWString(file).c_str());
    heightMapFile = file;
    delete[] pixels;
}

void TerrainEditor::SaveHeightMap()
{
    if (ImGui::Button("SaveHeight"))
    {
        DIALOG->OpenDialog("SaveHeight", "SaveHeight", ".png", ".");
    }
    if (DIALOG->Display("SaveHeight"))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            file = file.substr(projectPath.size() + 1, file.size());
        }

        DIALOG->Close();
    }

}

void TerrainEditor::LoadHeight(string file)
{
    if (file.length() == 0) return;
    heightMap = Texture::Add(ToWString(file));

    Resize();
}

void TerrainEditor::LoadHeightMap()
{
    if (ImGui::Button("LoadHeight"))
    {
        DIALOG->OpenDialog("LoadHeight", "LoadHeight", ".png", ".");
    }
    if (DIALOG->Display("LoadHeight"))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            file = file.substr(projectPath.size() + 1, file.size());

            LoadHeight(file);
        }

        DIALOG->Close();
    }

}

void TerrainEditor::SaveAlpha(string file)
{
    UINT size = width * height * 4;
    uint8_t* pixels = new uint8_t[size];

    vector<VertexType> vertices = mesh->Vertices();

    for (UINT i = 0; i < size / 4; i++)
    {
        float y = vertices[i].pos.y;

        uint8_t height = (y - MIN_HEIGHT) / (MAX_HEIGHT - MIN_HEIGHT) * 255;

        pixels[i * 4 + 0] = vertices[i].alpha[0] * 255;
        pixels[i * 4 + 1] = vertices[i].alpha[1] * 255;
        pixels[i * 4 + 2] = vertices[i].alpha[2] * 255;
        pixels[i * 4 + 3] = 255;
    }

    Image image;
    image.width = width;
    image.height = height;
    image.format = DXGI_FORMAT_R8G8B8A8_UNORM;
    image.rowPitch = width * 4;
    image.slicePitch = size;
    image.pixels = pixels;

    SaveToWICFile(image, WIC_FLAGS_FORCE_RGB, GetWICCodec(WIC_CODEC_PNG), ToWString(file).c_str());
    alphaMapFile = file;
    delete[] pixels;
}

void TerrainEditor::SaveAlphaMap()
{

    if (ImGui::Button("SaveAlpha"))
    {
        DIALOG->OpenDialog("SaveAlpha", "SaveAlpha", ".png", ".");
    }
    if (DIALOG->Display("SaveAlpha"))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            file = file.substr(projectPath.size() + 1, file.size());

            SaveAlpha(file);
        }

        DIALOG->Close();
    }
}

void TerrainEditor::LoadAlpha(string file)
{
    if (file.length() == 0) return;
    Texture* alphaMap = Texture::Add(ToWString(file));

    vector<Float4> pixels;
    alphaMap->ReadPixels(pixels);

    vector<VertexType>& vertices = mesh->Vertices();

    for (UINT i = 0; i < vertices.size(); i++)
    {
        vertices[i].alpha[0] = pixels[i].z;
        vertices[i].alpha[1] = pixels[i].y;
        vertices[i].alpha[2] = pixels[i].x;
        vertices[i].alpha[3] = pixels[i].w;
    }

    mesh->UpdateVertex();
}

void TerrainEditor::LoadAlphaMap()
{
    if (ImGui::Button("LoadAlpha"))
    {
        DIALOG->OpenDialog("LoadAlpha", "LoadAlpha", ".png", ".");
    }
    if (DIALOG->Display("LoadAlpha"))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            file = file.substr(projectPath.size() + 1, file.size());

            LoadAlpha(file);
        }

        DIALOG->Close();
    }
}

void TerrainEditor::Save(string file)
{
    material->Save("TextData/Materials/" + name + "_Material1.mat");
    SaveHeight("Textures/Heightmaps/" + name + "_HeightMap.png");
    SaveAlpha("Textures/AlphaMaps/" + name + "_AlphaMap.png");

    tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
    tinyxml2::XMLElement* map = document->NewElement("Map");
    map->SetAttribute("Name", name.c_str());
    document->InsertFirstChild(map);

    tinyxml2::XMLElement* materials = document->NewElement("Materials");

    tinyxml2::XMLElement* material1 = document->NewElement("Material1");
    string materialFile = material->GetFile();
    material1->SetAttribute("Material1", materialFile.c_str());
    materials->InsertFirstChild(material1);

    map->InsertFirstChild(materials);

    tinyxml2::XMLElement* Maps = document->NewElement("Maps");

    tinyxml2::XMLElement* secondmap = document->NewElement("SecondMap");

    tinyxml2::XMLElement* seconddiffusemap = document->NewElement("DiffuseMap");
    string secondDiffuseMapFile = ToString(maps[1]->GetFile());
    seconddiffusemap->SetAttribute("DiffuseMap", secondDiffuseMapFile.c_str());
    secondmap->InsertFirstChild(seconddiffusemap);

    tinyxml2::XMLElement* secondnormalmap = document->NewElement("NormalMap");
    string secondNormalMapFile = ToString(normalMaps[1]->GetFile());
    secondnormalmap->SetAttribute("NormalMap", secondNormalMapFile.c_str());
    secondmap->InsertEndChild(secondnormalmap);
    Maps->InsertFirstChild(secondmap);

    tinyxml2::XMLElement* thirdmap = document->NewElement("ThirdMap");

    tinyxml2::XMLElement* thirddiffusemap = document->NewElement("DiffuseMap");
    string thirdDiffuseMapFile = ToString(maps[2]->GetFile());
    thirddiffusemap->SetAttribute("DiffuseMap", thirdDiffuseMapFile.c_str());
    thirdmap->InsertFirstChild(thirddiffusemap);

    tinyxml2::XMLElement* thirdnormalmap = document->NewElement("NormalMap");
    string thirdNormalMapFile = ToString(normalMaps[2]->GetFile());
    thirdnormalmap->SetAttribute("NormalMap", thirdNormalMapFile.c_str());
    thirdmap->InsertEndChild(thirdnormalmap);
    Maps->InsertEndChild(thirdmap);

    tinyxml2::XMLElement* fourthmap = document->NewElement("FourthMap");

    tinyxml2::XMLElement* fourthdiffusemap = document->NewElement("DiffuseMap");
    string fourthDiffuseMapFile = ToString(maps[3]->GetFile());
    fourthdiffusemap->SetAttribute("DiffuseMap", fourthDiffuseMapFile.c_str());
    fourthmap->InsertFirstChild(fourthdiffusemap);

    tinyxml2::XMLElement* fourthnormalmap = document->NewElement("NormalMap");
    string fourthNormalMapFile = ToString(normalMaps[3]->GetFile());
    fourthnormalmap->SetAttribute("NormalMap", fourthNormalMapFile.c_str());
    fourthmap->InsertEndChild(fourthnormalmap);
    Maps->InsertEndChild(fourthmap);

    tinyxml2::XMLElement* heightmap = document->NewElement("HeightMap");
    heightmap->SetAttribute("HeightMap", heightMapFile.c_str());
    Maps->InsertEndChild(heightmap);

    tinyxml2::XMLElement* alphamap = document->NewElement("AlphaMap");
    alphamap->SetAttribute("AlphaMap", alphaMapFile.c_str());
    Maps->InsertEndChild(alphamap);

    map->InsertEndChild(Maps);

    document->SaveFile(file.c_str());

    delete document;
}

void TerrainEditor::Load(string file)
{
    this->file = file;

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
    LoadHeight(heightmap->Attribute("HeightMap"));

    tinyxml2::XMLElement* alphamap = heightmap->NextSiblingElement();
    LoadAlpha(alphamap->Attribute("AlphaMap"));

    delete document;
}

void TerrainEditor::SaveDialog()
{
    string key = "SaveMap";


    if (ImGui::Button("Save"))
    {
        if (file.empty())
        {
            Save("TextData/Terrains/" + name + ".terrain");
        }
        else
        {
            Save(file);
        }
    }
    ImGui::SameLine();

    if (ImGui::Button("SaveAs"))
    {
        DIALOG->OpenDialog(key.c_str(), "SaveTerrain", ".terrain", ".");
    }

    if (DIALOG->Display(key.c_str()))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            file = file.substr(projectPath.size() + 1, file.size());

            Save(file);
        }

        DIALOG->Close();
    }
}

void TerrainEditor::LoadDialog()
{
    string key = "LoadMap";


    if (ImGui::Button("Load"))
    {
        DIALOG->OpenDialog(key.c_str(), "LoadMap", ".terrain", ".");
    }

    if (DIALOG->Display(key.c_str()))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            file = file.substr(projectPath.size() + 1, file.size());

            Load(file);
        }

        DIALOG->Close();
    }
}

void TerrainEditor::SelectMap(string name, UINT index)
{
    ImGui::SetWindowFontScale(1.2);
    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.7f, 1.0f), name.c_str());
    ImGui::SameLine();
    ImGui::SetWindowFontScale(1);


    ImTextureID textureID = maps[index]->GetSRV();

    if (ImGui::ImageButton(textureID, ImVec2(50, 50)))
    {
        DIALOG->OpenDialog(this->name + name, name, ".png,.jpg,.tga,.dds,.bmp,{.png,.jpg,.tga,.dds,.bmp},", ".");
    }
    if (DIALOG->Display(this->name + name))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            char path[128];
            GetCurrentDirectoryA(128, path);

            file = file.substr(strlen(path) + 1, file.length());

            maps[index] = Texture::Add(ToWString(file));
        }

        DIALOG->Close();
    }
}