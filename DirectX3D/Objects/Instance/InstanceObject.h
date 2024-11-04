#pragma once

class InstanceObject
{
public:
	enum ColliderType
	{
		SPHERE, BOX, CAPSULE
	};
public:
	InstanceObject(string name, Transform* transform, ColliderType type = SPHERE);
	~InstanceObject();

	virtual void Update();
	void Render();
	void GUIRender();
	Collider* GetCollider() { return collider; }
	Transform* GetTransform() { return transform; }

	virtual void Spawn(Vector3 pos, Vector3 rot = Vector3(), Vector3 scale = Vector3(1, 1, 1));

protected:
	string name;

	Collider* collider;
	Transform* transform;
};
