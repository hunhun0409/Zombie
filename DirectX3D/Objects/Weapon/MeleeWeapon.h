#pragma once

class MeleeWeapon : public Weapon
{
public:
	MeleeWeapon(string name);
	~MeleeWeapon();

	virtual void Update() override;
	virtual void Render() override;
	virtual void GUIRender() override;

	void EnableAttack();
	void DisableAttack();

	//Transform* GetStartPos() { return start; }
	//Transform* GetEndPos() { return end; }

	void Load();

private:
	CapsuleCollider* collider;

	set<Collider*> overlappedColliders;

	SphereCollider* start;
	SphereCollider* end;

	Trail* trail;
};
