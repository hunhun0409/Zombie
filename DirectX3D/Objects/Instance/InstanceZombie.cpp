

#include "Framework.h"

InstanceZombie::InstanceZombie(string name, Transform* transform, ModelAnimatorInstancing* instancing, UINT index)
	:InstanceCharacter(name, transform, instancing, index)
{
	totalEvents.resize(instancing->GetClipSize());
	eventIters.resize(instancing->GetClipSize());

	SetEvent(SCREAM, bind(&InstanceZombie::ScreamEnd, this), 0.7f);
	SetEvent(ATTACK, bind(&InstanceZombie::EnableAttack, this), 0.3f);
	SetEvent(ATTACK, bind(&InstanceZombie::DisableAttack, this), 0.8f);
	SetEvent(ATTACK, bind(&InstanceZombie::Idle, this), 0.9f);
	SetEvent(BITE_START, bind(&InstanceZombie::BiteLoop, this), 0.7f);
	SetEvent(BITE_END, bind(&InstanceZombie::Idle, this), 0.7f);
	SetEvent(HIT, bind(&InstanceZombie::Idle, this), 0.8f);
	SetEvent(DIE, bind(&InstanceZombie::Dead, this), 0.9f);

	Observer::Get()->AddEvent("ZombieBiteEnd", bind(&InstanceZombie::BiteEnd, this));

	FOR(totalEvents.size())
	{
		eventIters[i] = totalEvents[i].begin();
	}

	rightHandTransform = new Transform();
	rightHand = new SphereCollider();
	rightHand->SetTag(name + "_RightHand_Collider");
	rightHand->SetParent(rightHandTransform);
	rightHand->Load();
	rightHand->UpdateWorld();
	rightHand->SetActive(false);

	attackRange *= collider->Height() / collider->GlobalScale().y;
}

InstanceZombie::~InstanceZombie()
{
	delete rightHandTransform;
	delete rightHand;
}

void InstanceZombie::Update()
{
	InstanceCharacter::Update();

	ProcessBehavior();

	rightHandTransform->SetWorld(instancing->GetTransformByNode(index, rightHandIndex));
	rightHand->UpdateWorld();
}

void InstanceZombie::Render()
{
	InstanceCharacter::Render();
	rightHand->Render();
}

void InstanceZombie::GUIRender()
{
	InstanceCharacter::GUIRender();
	rightHand->GUIRender();
}

void InstanceZombie::Spawn(Vector3 pos)
{
	InstanceCharacter::Spawn(pos);

	Vector3 velocity = (target->GlobalPos() - transform->GlobalPos()).GetNormalized();

	curHp = maxHp;

	//Audio::Get()->Play("move", transform->GlobalPos(), velocity);
}

void InstanceZombie::TakeDamage(float damage)
{
	InstanceCharacter::TakeDamage(damage);

	if (curHp <= 0)
	{
		SetState(DIE);
	}
}

void InstanceZombie::ProcessBehavior()
{
	if (target)
	{
		stateTimer -= DELTA;

		velocity = target->GlobalPos() - transform->GlobalPos();
		float targetLength = velocity.Length();

		if (stateTimer <= 0)
		{
			if (targetLength > detectionRange)
			{
				SetBehavior(NORMAL);
			}
			else if (targetLength <= detectionRange && targetLength > alertRange)
			{
				if (curState != ALERT)
				{
					SetBehavior(CHASE);
				}
			}
			else if (targetLength <= alertRange)
			{
				SetBehavior(ALERT);
			}
		}
	}
	else
	{
		SetBehavior(NORMAL);
	}


	if (curState >= HIT) return;
	switch (curBehavior)
	{
	case InstanceZombie::NORMAL:
		Patrol();
		break;
	case InstanceZombie::CHASE:
		Chase();
		break;
	case InstanceZombie::ALERT:
		Alert();
		break;
	}
}

void InstanceZombie::Rotate()
{
	Vector3 forward = transform->Forward();
	Vector3 cross = Cross(forward, velocity);

	if (cross.y < 0)
	{
		transform->Rot().y += rotSpeed * DELTA;
	}
	else if (cross.y > 0)
	{
		transform->Rot().y -= rotSpeed * DELTA;
	}
}



void InstanceZombie::Patrol()
{
	if (curState == BITE_END) return;

	if (idleTime > 0)
	{
		idleTime -= DELTA;

		SetState(IDLE);
		patrolTime = PATROL_TIME;
	}
	else
	{
		if (patrolTime == PATROL_TIME)
		{
			Vector3 dir;
			dir.x = Random(-1.0f, 1.0f);
			dir.z = Random(-1.0f, 1.0f);
			float range = Random(PATROL_MIN_RADIUS, PATROL_MAX_RADIUS);
			patrolPoint = transform->GlobalPos() + dir.GetNormalized() * range;

			SetState(WALK);
		}

		patrolTime -= DELTA;

		Vector3 direciton = patrolPoint - transform->GlobalPos();
		transform->Rot().y = atan2(direciton.x, direciton.z) + XM_PI;

		speed = PATROL_SPEED;
		transform->Pos() += direciton.GetNormalized() * speed * DELTA;

		if ((transform->GlobalPos() - patrolPoint).Length() < 1.0f || patrolTime <= 0.0f)
		{
			idleTime = IDLE_TIME;
			patrolTime = PATROL_TIME;
		}
	}
}

void InstanceZombie::Chase()
{
	pathUpdateInterval -= DELTA;

	if (pathUpdateInterval <= 0)
	{
		destPos = target->GlobalPos();
		SetPath();
		pathUpdateInterval += PATH_UPDATE_INTERVAL;
	}

	if (path.size() == 0) return;

	Vector3 dir = path.back() - transform->GlobalPos();
	dir.y = 0.0f;

	if (dir.Length() < 1.0f)
	{
		path.pop_back();
	}

	Vector3 vel = dir.GetNormalized();

	speed = CHASE_SPEED;
	SetState(WALK);

	Vector3 forward = transform->Forward();
	Vector3 cross = Cross(forward, dir);

	if (cross.y < 0)
	{
		transform->Rot().y += rotSpeed * DELTA;
	}
	else if (cross.y > 0)
	{
		transform->Rot().y -= rotSpeed * DELTA;
	}
	transform->Pos() += vel * speed * DELTA;
}

void InstanceZombie::Alert()
{
	if (velocity.Length() < attackRange)
	{
		if (curState < ATTACK)
		{
			target->IsDead() ? SetState(BITE_START) : SetState(ATTACK);
		}
	}
	else
	{
		if (curState == ATTACK) return;
		if (hasScreamed)
		{
			pathUpdateInterval -= DELTA;

			if (pathUpdateInterval <= 0)
			{
				destPos = target->GlobalPos();
				SetPath();
				pathUpdateInterval += PATH_UPDATE_INTERVAL;
			}


			if (path.size() == 0) return;

			Vector3 dir = path.back() - transform->GlobalPos();
			dir.y = 0.0f;

			if (dir.Length() < 1.0f)
			{
				path.pop_back();
			}

			Vector3 vel = dir.GetNormalized();

			speed = ALERT_SPEED;
			SetState(RUN);

			Vector3 forward = transform->Forward();
			Vector3 cross = Cross(forward, dir);

			if (cross.y < 0.1f)
			{
				transform->Rot().y += rotSpeed * DELTA;
			}
			else if (cross.y > 0.1f)
			{
				transform->Rot().y -= rotSpeed * DELTA;
			}
			transform->Pos() += vel * speed * DELTA;
		}
		else
		{
			SetState(SCREAM);
		}
	}

	if (rightHand->Active())
	{
		if (rightHand->IsCapsuleCollision(target->GetCollider()))
		{
			target->TakeDamage(damage);
			rightHand->SetActive(false);
		}
	}
}


void InstanceZombie::Idle()
{
	SetState(IDLE);
}

void InstanceZombie::Dead()
{
	transform->SetActive(false);
}

void InstanceZombie::ScreamEnd()
{
	Idle();
	hasScreamed = true;
}

void InstanceZombie::EnableAttack()
{
	rightHand->SetActive(true);
}

void InstanceZombie::DisableAttack()
{
	rightHand->SetActive(false);
}

void InstanceZombie::BiteLoop()
{
	SetState(BITE_LOOP);
}

void InstanceZombie::BiteEnd()
{
	SetBehavior(NORMAL);
	if (curState != BITE_LOOP) return;
	SetState(BITE_END);
}

void InstanceZombie::SetBehavior(Behavior behavior)
{
	if (curBehavior == behavior) return;

	stateTimer = STATE_CHANGE_COOLDOWN;
	idleTime = IDLE_TIME;
	patrolTime = PATROL_TIME;
	pathUpdateInterval = 0.0f;

	curBehavior = behavior;
}

