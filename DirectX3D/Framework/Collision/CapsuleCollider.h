#pragma once
class CapsuleCollider : public Collider
{
public:
	CapsuleCollider(float radius = 1.0f, float height = 4.0f, int stackCount = 24, int sliceCount = 24);
	~CapsuleCollider() = default;

	bool IsRayCollision(IN Ray ray, OUT Contact* contact) override;
	bool IsBoxCollision(BoxCollider* collider) override;
	bool IsSphereCollision(SphereCollider* collider) override;
	bool IsCapsuleCollision(CapsuleCollider* collider) override;

	float Radius() { return radius * max(GlobalScale().x, max(GlobalScale().y, GlobalScale().z)); }
	float Height() { return height * GlobalScale().y; }

	virtual void Update() override;
	virtual void GUIRender() override;

	void Save();
	void Load();
private:
	void MakeMesh() override;

protected:
	float height;
	float radius;
	int stackCount;
	int sliceCount;

	bool isChanged = false;
};
