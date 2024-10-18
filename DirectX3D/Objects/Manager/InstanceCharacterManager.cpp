#include "Framework.h"

InstanceCharacterManager::~InstanceCharacterManager()
{
    for (pair<string, InstanceCharacters> instanceCharacters : totalCharacters)
    {
        for (InstanceCharacter* instanceCharacter : instanceCharacters.second)
        {
            delete instanceCharacter;
        }
    }

    for (pair<string, ModelAnimatorInstancing*> instance : totalInstancies)
    {
        delete instance.second;
    }
}

void InstanceCharacterManager::Update()
{
    for (pair<string, InstanceCharacters> instanceCharacters : totalCharacters)
    {
        for (InstanceCharacter* instanceCharacter : instanceCharacters.second)
        {
            if (instanceCharacter->GetTransform()->Active())
            {
                instanceCharacter->Update();
            }
        }
    }

    ApplySeperation();

    isChanged = false;

    for (pair<string, ModelAnimatorInstancing*> modelInstance : totalInstancies)
    {
        modelInstance.second->Update();
    }
}

void InstanceCharacterManager::Render()
{
    for (pair<string, InstanceCharacters> instanceCharacters : totalCharacters)
    {
        for (InstanceCharacter* instanceCharacter : instanceCharacters.second)
        {
            if (instanceCharacter->GetTransform()->Active())
            {
                instanceCharacter->Render();
            }
        }
    }

    for (pair<string, ModelAnimatorInstancing*> modelInstance : totalInstancies)
    {
        modelInstance.second->Render();
    }
}

void InstanceCharacterManager::PostRender()
{
}

void InstanceCharacterManager::GUIRender()
{
    ImGui::Text("InstanceCharacterManager");

    for (pair<string, InstanceCharacters> instanceCharacters : totalCharacters)
    {
        for (InstanceCharacter* instanceCharacter : instanceCharacters.second)
        {
            if (instanceCharacter->GetTransform()->Active())
            {
                instanceCharacter->GUIRender();
            }
        }
    }
}


void InstanceCharacterManager::SetTarget(Character* target)
{
    for (pair<string, InstanceCharacters> instanceCharacters : totalCharacters)
    {
        for (InstanceCharacter* instanceCharacter : instanceCharacters.second)
        {
            instanceCharacter->target = target;
        }
    }
    this->target = target;
}

void InstanceCharacterManager::Add(string key, string name)
{
    if (totalInstancies.count(key) != 0) return;
    
    ModelAnimatorInstancing* instancing = new ModelAnimatorInstancing(name);
    totalInstancies[key] = instancing;

    InstanceCharacters instanceCharacters;
    instanceCharacters.reserve(SIZE);

    if (name == "ZombieWoman")
    {
        totalInstancies["ZombieWoman"]->ReadClip("Idle", true);
        totalInstancies["ZombieWoman"]->ReadClip("Walk", true);
        totalInstancies["ZombieWoman"]->ReadClip("Run", true);
        totalInstancies["ZombieWoman"]->ReadClip("Scream");
        totalInstancies["ZombieWoman"]->ReadClip("Attack");
        totalInstancies["ZombieWoman"]->ReadClip("BiteStart", false, 0, "mixamorig:Hips");
        totalInstancies["ZombieWoman"]->ReadClip("BiteLoop", true, 0, "mixamorig:Hips");
        totalInstancies["ZombieWoman"]->ReadClip("BiteEnd", false, 0, "mixamorig:Hips");
        totalInstancies["ZombieWoman"]->ReadClip("Hit");
        totalInstancies["ZombieWoman"]->ReadClip("Dying");


        for (int i = 0; i < SIZE; i++)
        {
            Transform* transform = instancing->Add();
            transform->SetActive(false);
            
            InstanceCharacter* instanceCharacter = new InstanceZombie(name, transform, instancing, i);
            if (target)
            {
                instanceCharacter->target = target;
            }
            if (terrain)
            {
                instanceCharacter->SetTerrain(terrain);
            }
            if (aStar)
            {
                instanceCharacter->SetAStar(aStar);
            }
            static_cast<InstanceZombie*>(instanceCharacter)->SetRightHandIndex(37);
            instanceCharacters.push_back(instanceCharacter);
        }
    }
    
    if (name == "ZombieMutant")
    {
        totalInstancies["ZombieMutant"]->ReadClip("Idle", true);
        totalInstancies["ZombieMutant"]->ReadClip("Walk", true);
        totalInstancies["ZombieMutant"]->ReadClip("Run", true);
        totalInstancies["ZombieMutant"]->ReadClip("Scream");
        totalInstancies["ZombieMutant"]->ReadClip("Attack");
        totalInstancies["ZombieMutant"]->ReadClip("BiteStart", false, 0, "mixamorig:Hips");
        totalInstancies["ZombieMutant"]->ReadClip("BiteLoop", true, 0, "mixamorig:Hips");
        totalInstancies["ZombieMutant"]->ReadClip("BiteEnd", false, 0, "mixamorig:Hips");
        totalInstancies["ZombieMutant"]->ReadClip("Hit");
        totalInstancies["ZombieMutant"]->ReadClip("Dying");

        for (int i = 0; i < SIZE; i++)
        {
            Transform* transform = instancing->Add();
            transform->SetActive(false);

            InstanceCharacter* instanceCharacter = new InstanceZombie(name, transform, instancing, i);
            if (target)
            {
                instanceCharacter->target = target;
            }
            if (terrain)
            {
                instanceCharacter->SetTerrain(terrain);
            }
            if (aStar)
            {
                instanceCharacter->SetAStar(aStar);
            }
            static_cast<InstanceZombie*>(instanceCharacter)->SetRightHandIndex(39);
            instanceCharacters.push_back(instanceCharacter);
        }
    }

    totalCharacters[key] = instanceCharacters;
}

void InstanceCharacterManager::Remove(string key)
{
    if (totalInstancies.count(key) == 0) return;

    delete totalInstancies[key];
    totalInstancies.erase(key);

    for (auto instanceCharacter : totalCharacters[key])
    {
        delete instanceCharacter;
    }
    totalCharacters.erase(key);
}

void InstanceCharacterManager::Clear()
{
    if (totalInstancies.size() == 0) return;

    for (auto& pair : totalInstancies)
    {
        delete pair.second;
    }
    totalInstancies.clear();

    for (auto& instanceCharacters : totalCharacters)
    {
        for (auto instanceCharacter : instanceCharacters.second)
        {
            delete instanceCharacter;
        }
    }
    totalCharacters.clear();
}

void InstanceCharacterManager::Spawn(string key, UINT spawnAmount)
{
    if (totalInstancies.count(key) == 0) return;

    Vector3 dir;
    dir.x = Random(-1.0f, 1.0f);
    dir.z = Random(-1.0f, 1.0f);

    float distance = Random(10.0f, SPAWN_RANGE);

    Vector3 randomPos = target->Pos() + dir.GetNormalized() * distance;
    randomPos.y = 0.0f;

    for (InstanceCharacter* instanceCharacter : totalCharacters[key])
    {
        if (instanceCharacter->GetTransform()->Active() == false)
        {
            instanceCharacter->Spawn(randomPos);
            spawnAmount--;
            if (spawnAmount <= 0)
            {
                break;
            }
        }
    }

}

void InstanceCharacterManager::Spawn(string key, Vector3 pos, UINT spawnAmount)
{
    if (totalInstancies.count(key) == 0) return;

    for (InstanceCharacter* instanceCharacter : totalCharacters[key])
    {
        if (instanceCharacter->GetTransform()->Active() == false)
        {
            instanceCharacter->Spawn(pos);
            spawnAmount--;
            if (spawnAmount <= 0)
            {
                break;
            }
        }
    }
}

void InstanceCharacterManager::Collision(Collider* collider, float damage)
{
    for (pair<string, InstanceCharacters> instanceCharacters : totalCharacters)
    {
        for (InstanceCharacter* instanceCharacter : instanceCharacters.second)
        {
            if (!instanceCharacter->GetTransform()->Active()) continue;

            if (collider->IsCapsuleCollision(instanceCharacter->GetCollider()))
            {
                instanceCharacter->TakeDamage(damage);
            }
        }
    }
}

//void InstanceCharacterManager::Collision(Collider* collider, float damage, set<Collider*>& overlappedCollider)
//{
//    for (pair<string, InstanceCharacters> instanceCharacters : totalCharacters)
//    {
//        for (InstanceCharacter* instanceCharacter : instanceCharacters.second)
//        {
//            if (!instanceCharacter->GetTransform()->Active()) continue;
//
//            if (collider->IsCapsuleCollision(instanceCharacter->GetCollider()))
//            {
//                if (overlappedCollider.count(instanceCharacter->GetCollider()) == 0)
//                {
//                    instanceCharacter->TakeDamage(damage);
//                    overlappedCollider.insert(instanceCharacter->GetCollider());
//                }
//            }
//        }
//    }
//}

void InstanceCharacterManager::SetAstar(AStar* aStar)
{
    this->aStar = aStar;
    for (pair<string, InstanceCharacters> instanceCharacters : totalCharacters)
    {
        for (InstanceCharacter* instanceCharacter : instanceCharacters.second)
        {
            instanceCharacter->SetAStar(aStar);
        }
    }
}

void InstanceCharacterManager::SetTerrain(Terrain* terrain)
{
    this->terrain = terrain;
    for (pair<string, InstanceCharacters> instanceCharacters : totalCharacters)
    {
        for (InstanceCharacter* instanceCharacter : instanceCharacters.second)
        {
            instanceCharacter->SetTerrain(terrain);
        }
    }
}

void InstanceCharacterManager::ApplySeperation()
{
    for (pair<string, InstanceCharacters> instanceCharacters : totalCharacters)
    {
        for (InstanceCharacter* instanceCharacter : instanceCharacters.second)
        {
            for (pair<string, InstanceCharacters> otherInstanceCharacters : totalCharacters)
            {
                instanceCharacter->ApplySeperation(otherInstanceCharacters.second);
            }
        }
    }
}

