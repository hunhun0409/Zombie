#include "Framework.h"

Character::Character(string name)
{
	tag = name;

	collider = new CapsuleCollider(0.4f, 1.0f);
	collider->SetTag(name + "_Collider");
	collider->Load();
	collider->SetOwner(this);
	collider->SetParent(this);

	model = new ModelAnimator(name);
	model->SetTag(name + "_Mesh");
	model->Load();
	model->SetParent(this);

	characterMovement = new CharacterMovement();
	characterMovement->SetOwner(this);

	ColliderManager::Get()->Add(collider);

	Observer::Get()->AddEvent("LevelUpEnd", bind(&Character::LevelUpEnd, this));

	pivot = new Transform();
}

Character::~Character()
{
	delete model;
	delete characterMovement;
	delete pivot;
}

void Character::Update()
{
	if (!Active()) return;

	recoverTime -= DELTA;
	if (recoverTime <= 0)
	{
		Heal(finalStatus.finalRecovery);
		recoverTime++;
		playerHud->UpdateHp(curHP/finalStatus.finalHp);
	}

	if (isDead)
	{
		deadTime -= DELTA;

		if (deadTime <= 0)
		{
			SetActive(false);
			Observer::Get()->ExcuteEvent("ZombieBiteEnd");
			InstanceCharacterManager::Get()->SetTarget(nullptr);
			StageManager::Get()->GameOver();
		}
	}
	pivot->Pos() = Pos();
	pivot->Rot().y += DELTA;
	pivot->UpdateWorld();

	if (controller)
	{
		controller->Update();
	}

	UpdateWorld();
	collider->Update();
	model->Update();
}

void Character::Render()
{
	if (!Active()) return;

	collider->Render();
	model->Render();
}

void Character::GUIRender()
{
	if (!Active()) return;

	collider->GUIRender();
	model->GUIRender();
}

void Character::Move()
{
	if (!isPlayingRootMotion)
	{
		characterMovement->Move();
	}
	else
	{
		characterMovement->Stop();
	}
}

void Character::Rotate()
{
}

void Character::Heal(float amount)
{
	curHP = min(finalStatus.finalHp, curHP + amount);
}

void Character::TakeDamage(float damage)
{
	float ratio = 1 - (finalStatus.finalArmor / 100.0f);

	curHP -= damage * ratio;
	if (curHP > 0)
	{
		ParticleManager::Get()->Play("BloodExplode", Pos() + Vector3(0, 6, 0));
	}
}

void Character::UpdateStatus()
{
	finalStatus.finalHp = initialStatus.initialHp + upgradeStatus.upgradeHp;
	finalStatus.finalAttack = initialStatus.initialAttack + upgradeStatus.upgradeAttack;
	finalStatus.finalArmor = initialStatus.initialArmor + upgradeStatus.upgradeArmor;
	finalStatus.finalRecovery = initialStatus.initialRecovery + upgradeStatus.upgradeRecovery;
	finalStatus.finalMagnet = initialStatus.initialMagnet + upgradeStatus.upgradeMagnet;

	float ratio = curHP / finalStatus.finalHp;
	Observer::Get()->ExcuteFloatParamEvent("UpdateHp", ratio);
}

void Character::SetState(int state, float playRate, float takeTime)
{
	if (curState == state) return;

	curState = state;
	model->PlayClip(curState, playRate, takeTime);
}

void Character::GetExp(float amount)
{
	curExp += amount;
	if (curExp >= maxExp)
	{
		LevelUp();

		curExp -= maxExp;
	}

	float ratio = curExp / maxExp;
	Observer::Get()->ExcuteFloatParamEvent("UpdateExp", ratio);
}

void Character::LevelUp()
{
	LevelUpSystem::Get()->LevelUp();
}

void Character::LevelUpEnd()
{
	Timer::Get()->SetDeltaScale(1.0f);
}
