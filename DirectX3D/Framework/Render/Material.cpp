#include "Framework.h"

Material::Material()
{
    diffuseMap = Texture::Add(L"Textures/Color/White.png", L"DM");
    specularMap = Texture::Add(L"Textures/Color/White.png", L"SM");
    normalMap = Texture::Add(L"Textures/Color/White.png", L"NM");

    buffer = new MaterialBuffer();

    char path[128];
    GetCurrentDirectoryA(128, path);
    projectPath = path;
}

Material::Material(wstring shaderFile)
{
    SetShader(shaderFile);

    diffuseMap = Texture::Add(L"Textures/Color/White.png", L"DM");
    specularMap = Texture::Add(L"Textures/Color/White.png", L"SM");
    normalMap = Texture::Add(L"Textures/Color/White.png", L"NM");

    buffer = new MaterialBuffer();

    char path[128];
    GetCurrentDirectoryA(128, path);
    projectPath = path;
}

Material::~Material()
{
    delete buffer;
}

void Material::Set()
{
    //texture2D
    diffuseMap->PSSet(0);
    specularMap->PSSet(1);
    normalMap->PSSet(2);

    //constBuffer
    buffer->SetPS(3);

    //shader
    vertexShader->Set();
    pixelShader->Set();
}

void Material::GUIRender()
{
    string title = name + "_Material";

    if (ImGui::TreeNode(title.c_str()))
    {
        char str[128];
        strcpy_s(str, 128, editName.c_str());
        ImGui::PushItemWidth(100);
        ImGui::InputText("Name", str, 128);
        ImGui::PopItemWidth();
        editName = str;

        ImGui::SameLine();
        if (ImGui::Button("Edit"))
            name = editName;

        ImGui::ColorEdit3("Diffuse", (float*)&buffer->Get().diffuse);
        ImGui::ColorEdit3("Specular", (float*)&buffer->Get().specular);
        ImGui::ColorEdit3("Ambient", (float*)&buffer->Get().ambient);
        ImGui::ColorEdit4("Emissive", (float*)&buffer->Get().emissive);

        ImGui::SliderFloat("Shininess", &buffer->Get().shininess, 1, 50);

        SelectMap("DM", DIFFUSE);
        ImGui::SameLine();
        UnselectMap(DIFFUSE);
        SelectMap("SM", SPECULAR);
        ImGui::SameLine();
        UnselectMap(SPECULAR);
        SelectMap("NM", NORMAL);
        ImGui::SameLine();
        UnselectMap(NORMAL);

        SaveDialog();
        LoadDialog();

        ImGui::TreePop();
    }
}

void Material::SetShader(wstring shaderFile)
{
    vertexShader = Shader::AddVS(shaderFile);
    pixelShader = Shader::AddPS(shaderFile);
}

void Material::SetDiffuseMap(wstring textureFile)
{
    if (textureFile.length() > 0)
        diffuseMap = Texture::Add(textureFile);
    else
        diffuseMap = Texture::Add(L"Textures/Color/White.png", L"DM");
}

void Material::SetSpecularMap(wstring textureFile)
{
    if (textureFile.length() > 0)
        specularMap = Texture::Add(textureFile);
    else
        specularMap = Texture::Add(L"Textures/Color/White.png", L"SM");
}

void Material::SetNormalMap(wstring textureFile)
{
    if (textureFile.length() > 0)
    {
        normalMap = Texture::Add(textureFile);
        buffer->Get().hasNormalMap = 1;
    }
    else
    {
        normalMap = Texture::Add(L"Textures/Color/White.png", L"NM");
        buffer->Get().hasNormalMap = 0;
    }
}

void Material::Save(string file)
{
    tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
    tinyxml2::XMLElement* material = document->NewElement("Material");
    material->SetAttribute("Name", name.c_str());
    document->InsertFirstChild(material);

    tinyxml2::XMLElement* shader = document->NewElement("Shader");

    tinyxml2::XMLElement* vertex = document->NewElement("Vertex");
    string shaderFile;
    if (vertexShader)
    {
        shaderFile = ToString(vertexShader->GetFile());
    }
    vertex->SetAttribute("Vertex", shaderFile.c_str());
    shader->InsertFirstChild(vertex);

    tinyxml2::XMLElement* pixel = document->NewElement("Pixel");
    if (pixelShader)
    {
        shaderFile = ToString(pixelShader->GetFile());
    }
    pixel->SetAttribute("Pixel", shaderFile.c_str());
    shader->InsertEndChild(pixel);

    material->InsertFirstChild(shader);


    tinyxml2::XMLElement* texture = document->NewElement("Texture");
    tinyxml2::XMLElement* diffuseTexture = document->NewElement("DiffuseMap");
    string textureFile = ToString(diffuseMap->GetFile());
    diffuseTexture->SetAttribute("Diffuse", textureFile.c_str());
    texture->InsertFirstChild(diffuseTexture);

    tinyxml2::XMLElement* specularTexture = document->NewElement("SpecularMap");
    textureFile = ToString(specularMap->GetFile());
    specularTexture->SetAttribute("Specular", textureFile.c_str());
    texture->InsertEndChild(specularTexture);

    tinyxml2::XMLElement* normalTexture = document->NewElement("NormalMap");
    textureFile = ToString(normalMap->GetFile());
    normalTexture->SetAttribute("Normal", textureFile.c_str());
    texture->InsertEndChild(normalTexture);
    material->InsertEndChild(texture);

    tinyxml2::XMLElement* property = document->NewElement("Property");
    tinyxml2::XMLElement* diffuse = document->NewElement("Diffuse");
    diffuse->SetAttribute("R", buffer->Get().diffuse.x);
    diffuse->SetAttribute("G", buffer->Get().diffuse.y);
    diffuse->SetAttribute("B", buffer->Get().diffuse.z);
    diffuse->SetAttribute("A", buffer->Get().diffuse.w);
    property->InsertEndChild(diffuse);

    tinyxml2::XMLElement* specular = document->NewElement("Specular");
    specular->SetAttribute("R", buffer->Get().specular.x);
    specular->SetAttribute("G", buffer->Get().specular.y);
    specular->SetAttribute("B", buffer->Get().specular.z);
    specular->SetAttribute("A", buffer->Get().specular.w);
    property->InsertEndChild(specular);

    tinyxml2::XMLElement* ambient = document->NewElement("Ambient");
    ambient->SetAttribute("R", buffer->Get().ambient.x);
    ambient->SetAttribute("G", buffer->Get().ambient.y);
    ambient->SetAttribute("B", buffer->Get().ambient.z);
    ambient->SetAttribute("A", buffer->Get().ambient.w);
    property->InsertEndChild(ambient);

    tinyxml2::XMLElement* emissive = document->NewElement("Emissive");
    emissive->SetAttribute("R", buffer->Get().emissive.x);
    emissive->SetAttribute("G", buffer->Get().emissive.y);
    emissive->SetAttribute("B", buffer->Get().emissive.z);
    emissive->SetAttribute("A", buffer->Get().emissive.w);
    property->InsertEndChild(emissive);

    tinyxml2::XMLElement* shininess = document->NewElement("Shininess");
    shininess->SetAttribute("Shininess", buffer->Get().shininess);
    property->InsertEndChild(shininess);
    tinyxml2::XMLElement* hasNormalMap = document->NewElement("HasNormalMap");
    hasNormalMap->SetAttribute("HasNormalMap", buffer->Get().hasNormalMap);
    property->InsertEndChild(hasNormalMap);
    //property->SetAttribute("Shininess", buffer->Get().shininess);
    //property->SetAttribute("HasNormalMap", buffer->Get().hasNormalMap);

    material->InsertEndChild(property);

    document->SaveFile(file.c_str());

    this->file = file;
    delete document;
}

void Material::Load(string file)
{
    this->file = file;

    tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
    document->LoadFile(file.c_str());
    tinyxml2::XMLElement* material = document->FirstChildElement();
    name = material->Attribute("Name");

    tinyxml2::XMLElement* shader = material->FirstChildElement();
    tinyxml2::XMLElement* vertex = shader->FirstChildElement();
    wstring shaderFile = ToWString(vertex->Attribute("Vertex"));
    shaderFile = shaderFile.substr(shaderFile.find_first_of('/') + 1, shaderFile.length());
    vertexShader = Shader::AddVS(shaderFile);

    tinyxml2::XMLElement* pixel = vertex->NextSiblingElement();
    shaderFile = ToWString(pixel->Attribute("Pixel"));
    shaderFile = shaderFile.substr(shaderFile.find_first_of('/') + 1, shaderFile.length());
    pixelShader = Shader::AddPS(shaderFile);


    tinyxml2::XMLElement* texture = shader->NextSiblingElement();
    //tinyxml2::XMLElement* texture = material->FirstChildElement("Texture");
    tinyxml2::XMLElement* diffuseTexture = texture->FirstChildElement();

    wstring textureFile = ToWString(diffuseTexture->Attribute("Diffuse"));
    SetDiffuseMap(textureFile);

    tinyxml2::XMLElement* specularTexture = diffuseTexture->NextSiblingElement();
    textureFile = ToWString(specularTexture->Attribute("Specular"));
    SetSpecularMap(textureFile);

    tinyxml2::XMLElement* normalTexture = specularTexture->NextSiblingElement();
    textureFile = ToWString(normalTexture->Attribute("Normal"));
    SetNormalMap(textureFile);


    tinyxml2::XMLElement* property = texture->NextSiblingElement();
    tinyxml2::XMLElement* diffuse = property->FirstChildElement();
    buffer->Get().diffuse.x = diffuse->FloatAttribute("R");
    buffer->Get().diffuse.y = diffuse->FloatAttribute("G");
    buffer->Get().diffuse.z = diffuse->FloatAttribute("B");
    buffer->Get().diffuse.w = diffuse->FloatAttribute("A");

    tinyxml2::XMLElement* specular = diffuse->NextSiblingElement();
    buffer->Get().specular.x = specular->FloatAttribute("R");
    buffer->Get().specular.y = specular->FloatAttribute("G");
    buffer->Get().specular.z = specular->FloatAttribute("B");
    buffer->Get().specular.w = specular->FloatAttribute("A");

    tinyxml2::XMLElement* ambient = specular->NextSiblingElement();
    buffer->Get().ambient.x = ambient->FloatAttribute("R");
    buffer->Get().ambient.y = ambient->FloatAttribute("G");
    buffer->Get().ambient.z = ambient->FloatAttribute("B");
    buffer->Get().ambient.w = ambient->FloatAttribute("A");

    tinyxml2::XMLElement* emissive = ambient->NextSiblingElement();
    buffer->Get().emissive.x = emissive->FloatAttribute("R");
    buffer->Get().emissive.y = emissive->FloatAttribute("G");
    buffer->Get().emissive.z = emissive->FloatAttribute("B");
    buffer->Get().emissive.w = emissive->FloatAttribute("A");

    tinyxml2::XMLElement* shininess = emissive->NextSiblingElement();
    buffer->Get().shininess = shininess->FloatAttribute("Shininess");

    tinyxml2::XMLElement* hasNormalMap = shininess->NextSiblingElement();
    buffer->Get().hasNormalMap = hasNormalMap->IntAttribute("HasNormalMap");

    delete document;
}



void Material::SelectMap(string name, MapType type)
{
    ImGui::SetWindowFontScale(2.0f);
    ImGui::TextColored(ImVec4(1, 0.5f, 0.8f, 1), name.c_str());
    ImGui::SetWindowFontScale(1.0f);
    ImGui::SameLine();

    ImTextureID textureID = nullptr;

    switch (type)
    {
    case Material::DIFFUSE:
        textureID = diffuseMap->GetSRV();
        break;
    case Material::SPECULAR:
        textureID = specularMap->GetSRV();
        break;
    case Material::NORMAL:
        textureID = normalMap->GetSRV();
        break;
    default:
        break;
    }

    if (ImGui::ImageButton(textureID, ImVec2(50, 50)))
    {
        DIALOG->OpenDialog(this->name + name, name, ".png,.jpg,.tga", ".");
    }

    if (DIALOG->Display(this->name + name))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            char path[128];
            GetCurrentDirectoryA(128, path);

            file = file.substr(strlen(path) + 1, file.length());

            switch (type)
            {
            case Material::DIFFUSE:
                SetDiffuseMap(ToWString(file));
                break;
            case Material::SPECULAR:
                SetSpecularMap(ToWString(file));
                break;
            case Material::NORMAL:
                SetNormalMap(ToWString(file));
                break;
            default:
                break;
            }
        }

        DIALOG->Close();
    }

    /*ImGui::SetWindowFontScale(1.2);
    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.7f, 1.0f), name.c_str());
    ImGui::SameLine();
    ImGui::SetWindowFontScale(1);


    ImTextureID textureID = nullptr;

    switch (type)
    {
    case Material::DIFFUSE:
        textureID = diffuseMap->GetSRV();
        break;
    case Material::SPECULAR:
        textureID = specularMap->GetSRV();
        break;
    case Material::NORMAL:
        textureID = normalMap->GetSRV();
        break;
    default:
        break;
    }

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

            switch (type)
            {
            case Material::DIFFUSE:
                SetDiffuseMap(ToWString(file));
                break;
            case Material::SPECULAR:
                SetSpecularMap(ToWString(file));
                break;
            case Material::NORMAL:
                SetNormalMap(ToWString(file));
                break;
            default:
                break;
            }
        }

        DIALOG->Close();
    }*/
}

void Material::UnselectMap(MapType type)
{
    ImTextureID textureID = nullptr;

    switch (type)
    {
    case Material::DIFFUSE:
        textureID = Texture::Add(L"Textures/UI/Cancel.png", L"DMCancel")->GetSRV();
        break;
    case Material::SPECULAR:
        textureID = Texture::Add(L"Textures/UI/Cancel.png", L"SMCancel")->GetSRV();
        break;
    case Material::NORMAL:
        textureID = Texture::Add(L"Textures/UI/Cancel.png", L"NMCancel")->GetSRV();
        break;
    default:
        break;
    }

    if (ImGui::ImageButton(textureID, ImVec2(20, 20)))
    {
        switch (type)
        {
        case Material::DIFFUSE:
            SetDiffuseMap(L"");
            break;
        case Material::SPECULAR:
            SetSpecularMap(L"");

            break;
        case Material::NORMAL:
            SetNormalMap(L"");

            break;
        default:
            break;
        }
    }
}

void Material::SaveDialog()
{
    string key = "SaveMaterial";


    if (ImGui::Button("Save"))
    {
        if (file.empty())
        {
            Save("TextData/Materials/" + name + ".mat");
        }
        else
        {
            Save(file);
        }
    }
    ImGui::SameLine();

    if (ImGui::Button("SaveAs"))
    {
        DIALOG->OpenDialog(key.c_str(), "SaveMaterial", ".mat", ".");
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

void Material::LoadDialog()
{
    string key = "LoadMaterial";


    if (ImGui::Button("Load"))
    {
        DIALOG->OpenDialog(key.c_str(), "LoadMaterial", ".mat", ".");
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
