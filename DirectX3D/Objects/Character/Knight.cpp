#include "Framework.h"

Knight::Knight()
	:Character("Knight")
{
	ReadClips();

	model->Rot().y = XM_PI;

	leftHand = new Transform();
	rightHand = new Transform();
	sword = new MeleeWeapon("sword");
	sword->SetParent(rightHand);
	sword->Damage() = 50;
	Idle();

	maxHP = 100.0f;
	curHP = maxHP;

	playerHud = new PlayerHUD(this);
}

Knight::~Knight()
{
	delete leftHand;
	delete rightHand;
	delete sword;
	delete playerHud;
}

void Knight::Update()
{
	if (!Active()) return;

	Character::Update();

	sword->Update();

	leftHand->SetWorld(model->GetTransformByNode(13));
	rightHand->SetWorld(model->GetTransformByNode(37));

	if (curState == DIE) return;
	Move();
	SetAnimation();
}

void Knight::Render()
{
	if (!Active()) return;

	Character::Render();
	sword->Render();
}

void Knight::GUIRender()
{
	if (!Active()) return;

	Character::GUIRender();
	sword->GUIRender();
}

void Knight::PostRender()
{
	playerHud->PostRender();
}

void Knight::Move()
{
	if (curState >= SLASH) return;
	Character::Move();

	Vector3 velocity = characterMovement->GetVelocity();
	if (velocity.Length() <= 2.0f)
	{
		Idle();
	}
	else
	{
		Vector3 forward = Left();
		Vector3 right = Forward();
		velocity.Normalize();
		float dotF = Dot(forward, velocity);
		float dotR = Dot(right, velocity);
		if (isDash)
		{
			if (abs(dotF) <= abs(dotR) + 0.1f)
			{
				dotR > 0 ? SetState(RUNR) : SetState(RUNL);
			}
			else
			{
				dotF > 0 ? SetState(RUNF) : SetState(RUNB);
			}
		}
		else
		{
			if (abs(dotF) <= abs(dotR) + 0.1f)
			{
				dotR > 0 ? SetState(WALKR) : SetState(WALKL);
			}
			else
			{
				dotF > 0 ? SetState(WALKF) : SetState(WALKB);
			}
		}
	}

}

void Knight::Idle()
{
	SetState(IDLE, 1.0f, 0.15f);
	isDash = false;
	characterMovement->maxSpeed = 100;
}

void Knight::Action()
{
	SetState(SLASH, 1.0f, 0.1f);
}

void Knight::RButton()
{
	//isBlock = !isBlock;
	//isBlock ? SetState(BLOCK_START) : Idle();
}

void Knight::Shift()
{
	isDash = !isDash;
	
	isDash ? characterMovement->maxSpeed = 400 : characterMovement->maxSpeed = 100;
}

void Knight::Ctrl()
{
}

void Knight::EnableAttack()
{
	sword->EnableAttack();
}

void Knight::DisableAttack()
{
	sword->DisableAttack();
}

void Knight::TakeDamage(float damage)
{
	Character::TakeDamage(damage);

	if (curHP <= 0)
	{
		SetState(DIE);
	}
	
}

void Knight::Dead()
{
	isDead = true;
	
	//SetActive(false);
}

void Knight::ReadClips()
{
	model->ReadClip("Idle", true);
	model->ReadClip("WalkF", true);
	model->ReadClip("WalkB", true);
	model->ReadClip("WalkL", true);
	model->ReadClip("WalkR", true);
	model->ReadClip("RunF", true);
	model->ReadClip("RunB", true);
	model->ReadClip("RunL", true);
	model->ReadClip("RunR", true);
	//root
	//model->ReadClip("Kick");
	model->ReadClip("Slash3");
	model->GetClip(SLASH)->SetEvent(bind(&Knight::EnableAttack, this), 0.2f);
	model->GetClip(SLASH)->SetEvent(bind(&Knight::DisableAttack, this), 0.7f);
	model->GetClip(SLASH)->SetEvent(bind(&Knight::Idle, this), 0.8f);
	/*model->ReadClip("StrongAttack", false, 0, "mixamorig:Hips");
	model->GetClip(STRONG_ATTACK)->SetEvent(bind(&Knight::EnableAttack, this), 0.2f);
	model->GetClip(STRONG_ATTACK)->SetEvent(bind(&Knight::DisableAttack, this), 0.5f);
	model->GetClip(STRONG_ATTACK)->SetEvent(bind(&Knight::Idle, this), 0.8f);*/

	/*model->ReadClip("Sheath Sword 1", false, 0, "mixamorig:Hips");
	model->GetClip(SHEATH1)->SetEvent(bind(&Knight::Sheath, this), 0.6f);
	model->ReadClip("Sheath Sword 2", false, 0, "mixamorig:Hips");
	model->GetClip(SHEATH2)->SetEvent(bind(&Knight::SheathEnd, this), 0.6f);
	model->ReadClip("Draw Sword 1", false, 0, "mixamorig:Hips");
	model->GetClip(DRAW1)->SetEvent(bind(&Knight::Draw, this), 0.6f);
	model->ReadClip("Draw Sword 2", false, 0, "mixamorig:Hips");
	model->GetClip(DRAW2)->SetEvent(bind(&Knight::DrawEnd, this), 0.6f);*/

	model->ReadClip("Die");
	model->GetClip(DIE)->SetEvent(bind(&Knight::Dead, this), 0.8f);
}

void Knight::SetAnimation()
{
	
}
