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
            
            if (projectile->GetTransform()->Active())
            {
                projectile->Update();
            }
        }
    }

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

    for (auto projectiles : totalProjectiles)
    {
        for (auto projectile : projectiles.second)
        {
            projectile->GUIRender();
        }
    }
}

void ProjectileManager::UpdateDamage(string key, float damage)
{
    if (totalProjectiles.count(key) == 0) return;
    for (auto projectile : totalProjectiles[key])
    {
        projectile->GetDamage() = damage;
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

void ProjectileManager::Add(string key, string name, float damage)
{
    if (totalInstancies.count(key) != 0) return;

    ModelInstancing* instancing = new ModelInstancing(name);

    Projectiles projectiles;
    projectiles.reserve(SIZE);

    for (int i = 0; i < SIZE; i++)
    {
        Transform* transform = instancing->Add();
        transform->SetActive(false);
        Projectile* projectile = new Projectile(name, transform, damage);
        projectiles.push_back(projectile);

        if (i == 0)
        {
            Transform* colliderTransform = new Transform();
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


