#pragma once

class Projectile
{
public:
	Projectile(string name, Transform* transform);
	~Projectile();

	void Update();
	void Render();
	void GUIRender();

	void Shoot(Vector3 startPos, Vector3 dir, float speed, float maxLifeTime = 10.0f);

	CapsuleCollider* GetCollider() { return collider; }
	Transform* GetTransform() { return transform; }

private:
	string name;

	Transform* transform;
	CapsuleCollider* collider;

	float speed = 1.0f;       // 이동 속도
	float maxLifeTime = 10.0f;    // 발사체의 최대 수명
	float curLifeTime = 0.0f;    // 발사체가 존재한 시간

	Vector3 direction;
};
