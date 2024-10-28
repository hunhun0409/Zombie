#include "Framework.h"

ExpBall::ExpBall(Transform* transform)
	:InstanceObject("sphere", transform)
{
	Observer::Get()->AddParamEvent("ExpSetTarget", bind(&ExpBall::SetTarget, this, placeholders::_1));
}

void ExpBall::Update()
{
	if (!transform->Active()) return;
	InstanceObject::Update();
	if (target)
	{
		if (target->IsDead()) return;

		float distance = Distance(target->GlobalPos(), transform->GlobalPos());
		if (distance <= target->GetFinalStatus().finalMagnet)
		{
			MoveTo();
		}

		if(collider->IsCollision(target->GetCollider()))
		{
			moveTime = 0.0f;
			ApplyEffect();
		}
	}
}

void ExpBall::ApplyEffect()
{
	target->GetExp(value);
	transform->SetActive(false);
}

void ExpBall::SetTarget(void* target)
{
	Character* character = (Character*)target;

	this->target = character;
}

void ExpBall::MoveTo()
{
	moveTime += DELTA;

	
	Vector3 startPos = transform->GlobalPos();
	Vector3 destPos = target->GlobalPos();

	transform->Pos() = Lerp(startPos, destPos, (moveTime * moveTime) / MOVE_TIME);
}
