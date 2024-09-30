#include "Framework.h"

ProjectileManager::~ProjectileManager()
{
    for (pair<string, Projectiles> projectiles : totalProjectiles)
    {
        for (Projectile* projectile : projectiles.second)
        {
            delete projectile;
        }
    }

    for (pair<string, ModelInstancing*> modelInstance : totalInstancies)
    {
        delete modelInstance.second;
    }

    for (pair<string, Transform*> transform: totalModelTransforms)
    {
        delete transform.second;
    }

    for (pair<string, Transform*> transform : totalColliderTransforms)
    {
        delete transform.second;
    }
}

void ProjectileManager::Update()
{
    for (pair<string, Projectiles> projectiles : totalProjectiles)
    {
        for (Projectile* projectile : projectiles.second)
        {
            if (isChanged)
            {
                projectile->GetCollider()->Pos() = totalColliderTransforms[projectiles.first]->Pos();
                projectile->GetCollider()->Rot() = totalColliderTransforms[projectiles.first]->Rot();
                projectile->GetCollider()->Scale() = totalColliderTransforms[projectiles.first]->Scale();
            }

            if (projectile->GetTransform()->Active())
            {
                projectile->Update();
            }
        }
    }

    isChanged = false;

    for (pair<string, ModelInstancing*> modelInstance : totalInstancies)
    {
        modelInstance.second->Update();
    }
}

void ProjectileManager::Render()
{
    for (pair<string, Projectiles> projectiles : totalProjectiles)
    {
        for (Projectile* projectile : projectiles.second)
        {
            if (projectile->GetTransform()->Active())
            {
                projectile->Render();
            }
        }
    }

    for (pair<string, ModelInstancing*> modelInstance : totalInstancies)
    {
        modelInstance.second->Render();
    }
}

void ProjectileManager::GUIRender()
{
    ImGui::Text("ProjectileManager");

    for (iter = totalInstancies.begin(); iter != totalInstancies.end(); iter++)
    {
        if(ImGui::TreeNode(iter->first.c_str()))
        {
            bool tempChanged = false;

            string key = iter->first;

            string temp = key + "_Collider";

            ImGui::Text(temp.c_str());

            temp = key + "_Pos";

            temp = key + "_Collider_Pos";
            tempChanged = ImGui::DragFloat3(temp.c_str(), (float*)&totalColliderTransforms[key]->Pos(), 0.01f);
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
            tempChanged = ImGui::DragFloat3(temp.c_str(), (float*)&totalColliderTransforms[key]->Scale(), 0.01f);
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

bool ProjectileManager::Shoot(string key, Vector3 startPos, Vector3 dir, float speed, float maxLifeTime)
{
    for (Projectile* projectile : totalProjectiles[key])
    {
        if (!projectile->GetTransform()->Active())
        {
            projectile->Shoot(startPos, dir, speed, maxLifeTime);
            return true;
        }
    }
    return false;
}

void ProjectileManager::Add(string key, string name, Vector3 scale, Vector3 colliderScale)
{
    if (totalInstancies.count(key) != 0) return;

    ModelInstancing* instancing = new ModelInstancing(name);

    Projectiles projectiles;
    projectiles.reserve(SIZE);

    for (int i = 0; i < SIZE; i++)
    {
        Transform* transform = instancing->Add();
        transform->Scale() = scale;
        transform->SetActive(false);
        Projectile* projectile = new Projectile(name, transform);
        projectile->GetCollider()->Scale() = colliderScale;
        projectiles.push_back(projectile);

        if (i == 0)
        {
            Transform* colliderTransform = new Transform();
            colliderTransform->Scale() = colliderScale;
            totalColliderTransforms[key] = colliderTransform;
            Load(key);
        }
    }

    
    totalInstancies[key] = instancing;
    totalProjectiles[key] = projectiles;
}


void ProjectileManager::Remove(string key)
{
    if (totalInstancies.count(key) == 0) return;

    delete totalInstancies[key];
    totalInstancies.erase(key);

    for (auto projectile : totalProjectiles[key])
    {
        delete projectile;
    }
    totalProjectiles.erase(key);
}

void ProjectileManager::Clear()
{
    if (totalInstancies.size() == 0) return;

    for (auto& pair : totalInstancies)
    {
        delete pair.second;
    }
    totalInstancies.clear();

    for (auto& pair : totalProjectiles)
    {
        for (auto projectile : pair.second)
        {
            delete projectile;
        }
    }
    totalProjectiles.clear();
}

void ProjectileManager::SetScale(string key, Vector3 scale)
{
    if (totalInstancies.count(key) == 0) return;

    for (Projectile* projectile : totalProjectiles[key])
    {
        projectile->GetTransform()->Scale() = scale;
    }
    totalInstancies[key]->Update();
}

void ProjectileManager::SetColliderScale(string key, Vector3 scale)
{
    if (totalInstancies.count(key) == 0) return;

    for (Projectile* projectile : totalProjectiles[key])
    {
        projectile->GetCollider()->Scale() = scale;
    }
    totalInstancies[key]->Update();
}

bool ProjectileManager::IsCollision(Collider* collider)
{
    for (pair<string, Projectiles> projectiles : totalProjectiles)
    {
        for (Projectile* projectile : projectiles.second)
        {
            if (projectile->GetCollider()->IsCollision(collider))
            {
                projectile->GetTransform()->SetActive(false);
                return true;
            }
        }
    }
    return false;
}


void ProjectileManager::Save(string key)
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

    delete writer;
}

void ProjectileManager::Load(string key)
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

    isChanged = true;

    delete reader;
}


