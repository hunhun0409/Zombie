#pragma once

class RangeWeapon : public Weapon
{
public:
	RangeWeapon(string name, float interval);
	~RangeWeapon();

	virtual void Update() override;
	virtual void Render() override;
	virtual void GUIRender() override;

	void Shoot(float speed = 100.0f, float lifeTime = 2.0f);

	void SetProjectile(string name, string id, float damage);

	void Load();
	string GetProjectileName() { return projectileName; }
private:
	float interval;
	float cooldown;

	SphereCollider* start;
	SphereCollider* end;

	string projectileName;
};
