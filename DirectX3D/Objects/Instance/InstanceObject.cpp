#include "Framework.h"

InstanceObject::InstanceObject(string name, Transform* transform)
	:name(name), transform(transform)
{
	collider = new BoxCollider();
	collider->SetTag(name + "_Collider");
	collider->SetParent(transform);
	collider->SetActive(false);
	collider->Load();

}

InstanceObject::~InstanceObject()
{
	delete collider;
}

void InstanceObject::Update()
{
	if (!transform->Active()) return;

	transform->UpdateWorld();
	collider->UpdateWorld();
}

void InstanceObject::Render()
{
	collider->Render();
}

void InstanceObject::Spawn(Vector3 pos, Vector3 rot, Vector3 scale)
{
	transform->Pos() = pos;
	transform->Rot() = rot;
	transform->Scale() = scale;

	transform->SetActive(true);
	collider->SetActive(true);
}
