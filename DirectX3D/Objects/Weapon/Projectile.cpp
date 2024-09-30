#include "Framework.h"


Projectile::Projectile(string name, Transform* transform)
    :name(name), transform(transform)
{
    collider = new CapsuleCollider();
    collider->SetTag(name + "_Collider");
    collider->SetParent(transform);
    collider->Load();

}

Projectile::~Projectile()
{
	delete collider;
}

void Projectile::Update()
{
    if (!transform->Active()) return;

    curLifeTime += DELTA;

    if (curLifeTime >= maxLifeTime)
    {
        transform->SetActive(false);
    }

    transform->Pos() += transform->Forward() * speed * DELTA;

    collider->UpdateWorld();
}

void Projectile::Render()
{
    collider->Render();
}

void Projectile::GUIRender()
{
    collider->GUIRender();
}

void Projectile::Shoot(Vector3 startPos, Vector3 dir, float speed, float maxLifeTime)
{
    transform->SetActive(true);
    this->maxLifeTime = maxLifeTime;
    this->speed = speed;
    
    curLifeTime = 0.0f;
    transform->Pos() = startPos;

    dir.y *= -1;
    direction = dir.GetNormalized();

    float Yangle = atan2f(dir.x, dir.z);  // Y�� ȸ�� ���� (Yaw)
    float Xangle = atan2f(dir.y, sqrtf(dir.x * dir.x + dir.z * dir.z)); // X�� ȸ�� ���� (Pitch)

    // localRotation�� ������ �������� ����
    transform->Rot().x = Xangle;  // Pitch (���� ȸ��)
    transform->Rot().y = Yangle;  // Yaw (�¿� ȸ��)
}
