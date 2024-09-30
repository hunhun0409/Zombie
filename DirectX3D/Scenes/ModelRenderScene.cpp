#include "Framework.h"
#include "ModelRenderScene.h"

ModelRenderScene::ModelRenderScene()
{
    model = new Model("ZombieMutant");
    //model->GetMesh(0)->SetMaterial(model->AddMaterial("1"));
    //model->GetMesh(1)->SetMaterial(model->AddMaterial("2"));
    //model->GetMesh(2)->SetMaterial(model->AddMaterial("3"));
    model->Load();

}

ModelRenderScene::~ModelRenderScene()
{
    delete model;
}

void ModelRenderScene::Update()
{
    model->UpdateWorld();
}

void ModelRenderScene::PreRender()
{
}

void ModelRenderScene::Render()
{
    model->Render();
}

void ModelRenderScene::PostRender()
{
}

void ModelRenderScene::GUIRender()
{
    ImGui::SliderInt("Mesh", &selectMeshNum, 0, model->GetMeshNum());
    if (ImGui::Button("AddMaterial"))
    {
        model->GetMesh(selectMeshNum)->SetMaterial(model->AddMaterial());
    }

    ImGui::SameLine();

    if (ImGui::Button("SaveMaterial"))
    {
        SaveMaterial();
    }

    model->GUIRender();
}

void ModelRenderScene::SaveMaterial()
{
    string name = model->GetName();
    vector<Material*> materials = model->GetMaterials();

    string savePath = "Models/Materials/" + name + "/"; ;

    CreateFolders(savePath);

    for (Material* material : materials)
    {
        string path = savePath + material->Name() + ".mat";
        material->Save(path);
    }
}
