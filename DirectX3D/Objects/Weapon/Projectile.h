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

	float speed = 1.0f;       // �̵� �ӵ�
	float maxLifeTime = 10.0f;    // �߻�ü�� �ִ� ����
	float curLifeTime = 0.0f;    // �߻�ü�� ������ �ð�

	Vector3 direction;
};
