#pragma once

class Projectile : public BasicObject
{
public:
	Projectile(string name, Transform* transform, float damage);
	~Projectile();

	void Update();
	void Render();
	void GUIRender();

	void Shoot(Vector3 startPos, Vector3 dir, float speed, float maxLifeTime = 10.0f);

	CapsuleCollider* GetCollider() { return collider; }
	Transform* GetTransform() { return transform; }

	float GetDamage() { return damage; }

	void OnCollision(Collider* other);

private:
	string name;

	Transform* transform;
	CapsuleCollider* collider;

	float speed = 1.0f;       // �̵� �ӵ�
	float maxLifeTime = 10.0f;    // �߻�ü�� �ִ� ����
	float curLifeTime = 0.0f;    // �߻�ü�� ������ �ð�

	Vector3 direction;

	SphereCollider* start;
	SphereCollider* end;
	Trail* trail;

	float damage = 0.0f;
};
