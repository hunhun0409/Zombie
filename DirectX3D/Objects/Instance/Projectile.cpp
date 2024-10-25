#include "Framework.h"


Projectile::Projectile(string name, Transform* transform, float damage)
    :name(name), transform(transform), damage(damage)
{
    transform->SetTag(name);
    transform->Load();
    transform->Pos() = Vector3(0, 10, 0);

    collider = new CapsuleCollider();
    collider->SetTag(name + "_Collider");
    collider->SetParent(transform);
    collider->SetOwner(this);
    collider->Load();
    collider->Update();
    
    ColliderManager::Get()->Add(collider);

    start = new SphereCollider();
    start->SetTag(name + "_start");
    start->SetParent(transform);
    start->Pos().x = collider->Radius();
    start->UpdateWorld();

    end = new SphereCollider();
    end->SetTag(name + "_end");
    end->SetParent(transform);
    end->Pos().x = -collider->Radius();
    start->UpdateWorld();

    trail = new Trail(L"Textures/Effect/trail.png", start, end, 10, 50.0f);
}

Projectile::~Projectile()
{
	delete collider;

    delete start;
    delete end;
    delete trail;
}

void Projectile::Update()
{
    if (!transform->Active()) return;

    curLifeTime += DELTA;

    if (curLifeTime >= maxLifeTime)
    {
        transform->SetActive(false);
        collider->SetActive(false);
    }

    transform->Pos() += transform->Forward() * speed * DELTA;
    transform->UpdateWorld();

    collider->Update();

    start->UpdateWorld();
    end->UpdateWorld();

    trail->Update();
}

void Projectile::Render()
{
    collider->Render();
    trail->Render();
}

void Projectile::GUIRender()
{
    if (!transform->Active()) return;

    transform->GUIRender();
    collider->GUIRender();
    trail->GUIRender();
}

void Projectile::Shoot(Vector3 startPos, Vector3 dir, float speed, float maxLifeTime)
{
    transform->SetActive(true);
    collider->SetActive(true);
    
    this->maxLifeTime = maxLifeTime;
    this->speed = speed;
    
    curLifeTime = 0.0f;
    transform->Pos() = startPos;
    transform->UpdateWorld();
    start->UpdateWorld();
    end->UpdateWorld();
    trail->Reset();

    dir.y *= -1;
    direction = dir.GetNormalized();

    float Yangle = atan2f(dir.x, dir.z);  // Y축 회전 각도 (Yaw)
    float Xangle = atan2f(dir.y, sqrtf(dir.x * dir.x + dir.z * dir.z)); // X축 회전 각도 (Pitch)

    // localRotation에 각도를 라디안으로 설정
    transform->Rot().x = Xangle;  // Pitch (상하 회전)
    transform->Rot().y = Yangle;  // Yaw (좌우 회전)
}

void Projectile::OnCollision(Collider* other)
{
    string collideName = other->GetTag();

    if (collideName.find("Zombie") != string::npos)
    {
        BasicObject* own = other->Owner();
        InstanceZombie* zombie = dynamic_cast<InstanceZombie*>(own);
        if (zombie == nullptr) return;

        zombie->TakeDamage(damage);

        transform->SetActive(false);
        collider->SetActive(false);
    }
}
