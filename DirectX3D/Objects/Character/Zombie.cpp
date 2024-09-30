#include "Framework.h"

Zombie::Zombie(Transform* transform, ModelAnimatorInstancing* instancing, UINT index)
	:transform(transform), instancing(instancing), index(index)
{
	root = new Transform();

	collider = new CapsuleCollider(40, 90);
	collider->SetParent(root);
	collider->Pos().y = -5.0f;
	collider->SetActive(false);

	motion = instancing->GetMotion(index);
	totalEvents.resize(instancing->GetClipSize());
	eventIters.resize(instancing->GetClipSize());

	curState = STAND_UP;

	SetEvent(STAND_UP, bind(&Zombie::EndStandUp, this), 0.8f);
	SetEvent(HIT, bind(&Zombie::EndHit, this), 0.9f);
	SetEvent(DIE, bind(&Zombie::EndDying, this), 0.9f);

	FOR(eventIters.size())
	{
		eventIters[i] = totalEvents[i].begin();
	}

	hpBar = new ProgressBar(L"textures/UI/hp_bar.png", L"textures/UI/hp_bar_bg.png");
}

Zombie::~Zombie()
{
	delete collider;
	delete root;
	delete hpBar;
}

void Zombie::Update()
{
	if (!transform->Active()) return;

	velocity = target->GlobalPos() - transform->GlobalPos();

	ExecuteEvent();
	Move();
	UpdateHpBar();

	root->SetWorld(instancing->GetTransformByNode(index, 2));
	collider->UpdateWorld();

	
}

void Zombie::Render()
{
	collider->Render();
}

void Zombie::PostRender()
{
	if (CAM->ContainPoint(transform->GlobalPos()))
	{
		hpBar->Render();
	}
}

void Zombie::SetEvent(int clip, Event event, float timeRatio)
{
	if (totalEvents[clip].count(timeRatio) > 0) return;

	totalEvents[clip][timeRatio] = event;
}

void Zombie::ExecuteEvent()
{
	int index = curState;

	if (totalEvents[curState].empty()) return;

	if (eventIters[curState] == totalEvents[curState].end()) return;

	float ratio = motion->runningTime / motion->duration;

	if (eventIters[curState]->first > ratio) return;

	eventIters[index]->second();
	eventIters[index]++;
}

void Zombie::EndStandUp()
{
	collider->SetActive(true);

	SetState(WALK);
}

void Zombie::EndHit()
{
	SetState(WALK);
}

void Zombie::EndDying()
{
	transform->SetActive(false);
	collider->SetActive(false);
}

void Zombie::Hit()
{
	curHp -= 50.0f;
	hpBar->SetAmount(curHp / maxHp);


	if (curHp <= 0)
	{
		SetState(DIE);
		return;
	}

	curState = HIT;
	instancing->PlayClip(index, HIT, 1.0f, 0.2f);
	eventIters[HIT] = totalEvents[HIT].begin();


	//SetState(HIT);
}

void Zombie::Spawn(Vector3 pos)
{
	transform->SetActive(true);
	collider->SetActive(true);

	transform->Pos() = pos;

	SetState(WALK);

	curHp = maxHp;
}

void Zombie::SetState(State state, float playRate, float takeTime)
{
	if (curState == state) return;

	curState = state;
	instancing->PlayClip(index, state, playRate, takeTime);
	eventIters[state] = totalEvents[state].begin();
}

void Zombie::Move()
{
	if (curState == STAND_UP) return;
	if (curState == HIT) return;
	if (curState == DIE) return;
	if (velocity.Length() < 3.0f) return;

	if (velocity.Length() < 50.0f)
	{
		speed = 5.0f;
		SetState(RUN);
	}
	else
	{
		speed = 3.0f;
		SetState(WALK);
	}

	velocity.y = 0;
	transform->Pos() += velocity.GetNormalized() * speed * DELTA;

	transform->Rot().y = atan2(velocity.x, velocity.z) + XM_PI;

}

void Zombie::UpdateHpBar()
{
	float scale = 1.0f / (velocity.Length() * 0.1f);
	scale = Clamp(0.2f, 1.0f, scale);

	Vector3 hpBarPos = transform->Pos() + Vector3(0, 5.0f, 0);

	if (!CAM->ContainPoint(transform->GlobalPos())) return;


	hpBar->Pos() = CAM->WorldToScreen(hpBarPos);
	hpBar->Scale() = Vector3(scale, scale, scale);
	hpBar->UpdateWorld();
}
