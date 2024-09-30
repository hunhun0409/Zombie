#include "Framework.h"

InstanceObjectManager::InstanceObjectManager()
{

}

InstanceObjectManager::~InstanceObjectManager()
{
    for (pair<string, InstanceObjects> instanceObjects : totalObjects)
    {
        for (InstanceObject* instanceObject : instanceObjects.second)
        {
            delete instanceObject;
        }
    }

    for (pair<string, ModelInstancing*> instance : totalInstancies)
    {
        delete instance.second;
    }

    for (pair<string, Transform*> transform : totalModelTransforms)
    {
        delete transform.second;
    }

    for (pair<string, Transform*> transform : totalColliderTransforms)
    {
        delete transform.second;
    }
}

void InstanceObjectManager::Update()
{
    for (pair<string, InstanceObjects> instanceObjects : totalObjects)
    {
        for (InstanceObject* instanceObject : instanceObjects.second)
        {
            if (isChanged)
            {
                instanceObject->GetCollider()->Pos() = totalColliderTransforms[instanceObjects.first]->Pos();
                instanceObject->GetCollider()->Rot() = totalColliderTransforms[instanceObjects.first]->Rot();
                instanceObject->GetCollider()->Scale() = totalColliderTransforms[instanceObjects.first]->Scale();

                //instanceCharacter->GetTransform()->Pos() = totalModelTransforms[instanceCharacters.first]->Pos();
                //instanceCharacter->GetTransform()->Rot() = totalModelTransforms[instanceCharacters.first]->Rot();
                instanceObject->GetTransform()->Scale() = totalModelTransforms[instanceObjects.first]->Scale();
            }

            if (instanceObject->GetTransform()->Active())
            {
                instanceObject->Update();
            }
        }
    }

    isChanged = false;

    for (pair<string, ModelInstancing*> modelInstance : totalInstancies)
    {
        modelInstance.second->Update();
    }

}

void InstanceObjectManager::Render()
{
    for (pair<string, InstanceObjects> instanceObjects : totalObjects)
    {
        for (InstanceObject* instanceObject : instanceObjects.second)
        {
            if (instanceObject->GetTransform()->Active())
            {
                instanceObject->Render();
            }
        }
    }

    for (pair<string, ModelInstancing*> modelInstance : totalInstancies)
    {
        modelInstance.second->Render();
    }
}

void InstanceObjectManager::PostRender()
{
}

void InstanceObjectManager::GUIRender()
{
    ImGui::Text("InstanceObjectManager");

    for (iter = totalInstancies.begin(); iter != totalInstancies.end(); iter++)
    {
        if (ImGui::TreeNode(iter->first.c_str()))
        {
            bool tempChanged = false;

            string key = iter->first;

            string temp = key + "_Collider";


            //collider
            ImGui::Text(temp.c_str());

            temp = key + "_Pos";

            temp = key + "_Collider_Pos";
            tempChanged = ImGui::DragFloat3(temp.c_str(), (float*)&totalColliderTransforms[key]->Pos(), 0.1f);
            isChanged |= tempChanged;

            temp = key + "_Collider_Rot";
            Vector3 rot;
            rot.x = XMConvertToDegrees(totalColliderTransforms[key]->Rot().x);
            rot.y = XMConvertToDegrees(totalColliderTransforms[key]->Rot().y);
            rot.z = XMConvertToDegrees(totalColliderTransforms[key]->Rot().z);

            tempChanged = ImGui::DragFloat3(temp.c_str(), (float*)&rot, 1.0f, -180, 180);
            isChanged |= tempChanged;

            totalColliderTransforms[key]->Rot().x = XMConvertToRadians(rot.x);
            totalColliderTransforms[key]->Rot().y = XMConvertToRadians(rot.y);
            totalColliderTransforms[key]->Rot().z = XMConvertToRadians(rot.z);

            temp = key + "_Collider_Scale";
            tempChanged = ImGui::DragFloat3(temp.c_str(), (float*)&totalColliderTransforms[key]->Scale(), 0.1f);
            isChanged |= tempChanged;

            //mesh
            temp = key + "_Model";

            ImGui::Text(temp.c_str());

            temp = key + "_Pos";

            temp = key + "_Model_Pos";
            tempChanged = ImGui::DragFloat3(temp.c_str(), (float*)&totalModelTransforms[key]->Pos(), 0.1f);
            isChanged |= tempChanged;

            temp = key + "_Model_Rot";

            rot.x = XMConvertToDegrees(totalModelTransforms[key]->Rot().x);
            rot.y = XMConvertToDegrees(totalModelTransforms[key]->Rot().y);
            rot.z = XMConvertToDegrees(totalModelTransforms[key]->Rot().z);

            tempChanged = ImGui::DragFloat3(temp.c_str(), (float*)&rot, 1.0f, -180, 180);
            isChanged |= tempChanged;

            totalModelTransforms[key]->Rot().x = XMConvertToRadians(rot.x);
            totalModelTransforms[key]->Rot().y = XMConvertToRadians(rot.y);
            totalModelTransforms[key]->Rot().z = XMConvertToRadians(rot.z);

            temp = key + "_Model_Scale";
            tempChanged = ImGui::DragFloat3(temp.c_str(), (float*)&totalModelTransforms[key]->Scale(), 0.1f);
            isChanged |= tempChanged;



            if (ImGui::Button("Save"))
            {
                Save(key);
            }

            ImGui::SameLine();

            if (ImGui::Button("Load"))
            {
                Load(key);
            }

            ImGui::TreePop();
        }

    }
}

void InstanceObjectManager::Add(string key, string name, Vector3 scale, Vector3 colliderScale)
{
    /*if (totalInstancies.count(key) != 0) return;

    ModelInstancing* instancing = new ModelInstancing(name);

    InstanceObjects instanceObjects;
    instanceObjects.reserve(SIZE);

    for (int i = 0; i < SIZE; i++)
    {
        Transform* transform = instancing->Add();
        transform->Scale() = scale;
        transform->SetActive(false);
        InstanceObject* instanceObject = InstanceObjectFactory::Get()->CreateObject(key, name, transform);
        instanceObject->GetCollider()->Scale() = colliderScale;
        instanceObjects.push_back(instanceObject);

        if (i == 0)
        {
            Transform* colliderTransform = new Transform();
            colliderTransform->Scale() = colliderScale;
            totalColliderTransforms[key] = colliderTransform;

            Transform* modelTransform = new Transform();
            modelTransform->Scale() = colliderScale;
            totalModelTransforms[key] = modelTransform;
            Load(key);
        }
    }


    totalInstancies[key] = instancing;
    totalObjects[key] = instanceObjects;*/
}

void InstanceObjectManager::Remove(string key)
{
    if (totalInstancies.count(key) == 0) return;

    delete totalInstancies[key];
    totalInstancies.erase(key);

    for (auto instanceCharacter : totalObjects[key])
    {
        delete instanceCharacter;
    }
    totalObjects.erase(key);
}

void InstanceObjectManager::Clear()
{
    if (totalInstancies.size() == 0) return;

    for (auto& pair : totalInstancies)
    {
        delete pair.second;
    }
    totalInstancies.clear();

    for (auto& instanceObjects : totalObjects)
    {
        for (auto instanceObject : instanceObjects.second)
        {
            delete instanceObject;
        }
    }
    totalObjects.clear();
}


void InstanceObjectManager::Spawn(string key, Vector3 pos, Vector3 rot, Vector3 scale)
{
    if (totalInstancies.count(key) == 0) return;


    for (InstanceObject* instanceObject : totalObjects[key])
    {
        if (instanceObject->GetTransform()->Active() == false)
        {
            instanceObject->Spawn(pos, rot, scale);
            break;
        }
    }
}

void InstanceObjectManager::Collision()
{
}

void InstanceObjectManager::Save(string key)
{
    BinaryWriter* writer = new BinaryWriter("TextData/Transforms/Instance_" + key + "_Collider.srt");

    writer->Float(totalColliderTransforms[key]->Pos().x);
    writer->Float(totalColliderTransforms[key]->Pos().y);
    writer->Float(totalColliderTransforms[key]->Pos().z);

    writer->Float(totalColliderTransforms[key]->Rot().x);
    writer->Float(totalColliderTransforms[key]->Rot().y);
    writer->Float(totalColliderTransforms[key]->Rot().z);

    writer->Float(totalColliderTransforms[key]->Scale().x);
    writer->Float(totalColliderTransforms[key]->Scale().y);
    writer->Float(totalColliderTransforms[key]->Scale().z);

    writer->Float(totalModelTransforms[key]->Pos().x);
    writer->Float(totalModelTransforms[key]->Pos().y);
    writer->Float(totalModelTransforms[key]->Pos().z);

    writer->Float(totalModelTransforms[key]->Rot().x);
    writer->Float(totalModelTransforms[key]->Rot().y);
    writer->Float(totalModelTransforms[key]->Rot().z);

    writer->Float(totalModelTransforms[key]->Scale().x);
    writer->Float(totalModelTransforms[key]->Scale().y);
    writer->Float(totalModelTransforms[key]->Scale().z);

    delete writer;
}

void InstanceObjectManager::Load(string key)
{
    BinaryReader* reader = new BinaryReader("TextData/Transforms/Instance_" + key + "_Collider.srt");

    if (reader->IsFailed()) return;

    totalColliderTransforms[key]->Pos().x = reader->Float();
    totalColliderTransforms[key]->Pos().y = reader->Float();
    totalColliderTransforms[key]->Pos().z = reader->Float();

    totalColliderTransforms[key]->Rot().x = reader->Float();
    totalColliderTransforms[key]->Rot().y = reader->Float();
    totalColliderTransforms[key]->Rot().z = reader->Float();

    totalColliderTransforms[key]->Scale().x = reader->Float();
    totalColliderTransforms[key]->Scale().y = reader->Float();
    totalColliderTransforms[key]->Scale().z = reader->Float();

    totalModelTransforms[key]->Pos().x = reader->Float();
    totalModelTransforms[key]->Pos().y = reader->Float();
    totalModelTransforms[key]->Pos().z = reader->Float();

    totalModelTransforms[key]->Rot().x = reader->Float();
    totalModelTransforms[key]->Rot().y = reader->Float();
    totalModelTransforms[key]->Rot().z = reader->Float();

    totalModelTransforms[key]->Scale().x = reader->Float();
    totalModelTransforms[key]->Scale().y = reader->Float();
    totalModelTransforms[key]->Scale().z = reader->Float();

    isChanged = true;

    delete reader;
}
