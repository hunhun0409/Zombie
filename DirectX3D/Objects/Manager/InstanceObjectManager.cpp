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
}

void InstanceObjectManager::Update()
{
    
    for (pair<string, InstanceObjects> instanceObjects : totalObjects)
    {
        for (InstanceObject* instanceObject : instanceObjects.second)
        {
            if (instanceObject->GetTransform()->Active())
            {
                instanceObject->Update();
            }
        }
    }

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
    
    //ImGui::Text("InstanceObjectManager");

    /*for (pair<string, InstanceObjects> instanceObjects : totalObjects)
    {
        for (InstanceObject* instanceObject : instanceObjects.second)
        {
            if (instanceObject->GetTransform()->Active())
            {
                instanceObject->GUIRender();
            }
        }
    }*/
}

void InstanceObjectManager::Add(string key, string name)
{
    if (totalInstancies.count(key) != 0) return;

    ModelInstancing* instancing = new ModelInstancing(name);
    totalInstancies[key] = instancing;

    InstanceObjects instanceObjects;
    instanceObjects.reserve(SIZE);

    for (int i = 0; i < SIZE; i++)
    {
        Transform* transform = instancing->Add();
        transform->SetActive(false);
        if (name == "sphere")
        {
            ExpBall* instanceObject = new ExpBall(transform);
            instanceObjects.push_back(instanceObject);
        }
        else
        {
            InstanceObject* instanceObject = new InstanceObject(name, transform);
            instanceObjects.push_back(instanceObject);
        }

    }

    totalInstancies[key] = instancing;
    totalObjects[key] = instanceObjects;
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

void InstanceObjectManager::SetShader(wstring file)
{
    for (pair<string, ModelInstancing*> instance : totalInstancies)
    {
        instance.second->SetShader(file);
    }
}
