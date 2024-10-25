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
		if (collider->IsCollision(target->GetMagnetCollider()))
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
	//to do : ���� �ݶ��̴���? �׳� �Ÿ� ��� �ϸ� �ȵ�?
	moveTime += DELTA;

	Vector3 destPos = target->GlobalPos();

	transform->Pos() = Lerp(destPos, transform->Pos(), moveTime);
}
