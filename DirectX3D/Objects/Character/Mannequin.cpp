#include "Framework.h"

Mannequin::Mannequin()
	:Character("Mannequin")
{
	ReadClips();
	//WeaponA();

	leftHand = new Transform();
	leftHand->SetParent(model);

	bow = new Model("Bow");

	bow->SetTag("bow");
	bow->SetParent(leftHand);
	bow->Load();


	crossHair = new Quad(L"Textures/UI/cursor.png");
	crossHair->Pos() = { CENTER_X, CENTER_Y + 20, 0 };
	crossHair->UpdateWorld();

	ProjectileManager::Get()->Add("playerArrow", "arrow", Vector3(0.03f, 0.03f, 0.04f));


	
}

Mannequin::~Mannequin()
{
	delete bow;

	delete crossHair;
}

void Mannequin::Update()
{
	Character::Update();

	leftHand->SetWorld(model->GetTransformByNode(35));

	bow->UpdateWorld();
}

void Mannequin::Render()
{
	Character::Render();
	bow->Render();
}

void Mannequin::GUIRender()
{
	Character::GUIRender();
	bow->GUIRender();
}

void Mannequin::PostRender()
{
	crossHair->Render();
}

void Mannequin::Move()
{
	if (curState == EQUIP || curState == UNEQUIP) return;

	characterMovement->Move();

	//velocity = characterMovement->velocity;
	//velocity.y = 0;

	//
	//Matrix rotY = XMMatrixRotationY(Rot().y);
	//Vector3 direction = XMVector3TransformCoord(velocity, rotY);

	//Pos() += direction * DELTA;
	
}

void Mannequin::Idle()
{
	if (curState == EQUIP || curState == STANDING_DRAW_ARROW || curState >= STANDING_AIM_RECOIL) return;
	if (isArmed)
	{
		if (isAim)
		{
			SetState(STANDING_AIM_IDLE, 2.0f, 0.1f);

			
		}
		else
		{
			SetState(ARM_STANDING_IDLE, 2.0f, 0.1f);

		}
	}
	else
	{
		SetState(UNARM_STANDING_IDLE, 2.0f, 0.1f);
	}
}

void Mannequin::Aim()
{
	isAim = true;
	SetState(STANDING_AIM_IDLE);
}

void Mannequin::WeaponA()
{
	if (isArmed)
	{
		SetState(UNEQUIP);
	}
	else
	{
		SetState(EQUIP);
	}

	isArmed = !isArmed;
	characterMovement->MaxSpeed() = isArmed ? MAX_RUNSPEED : MAX_WALKSPEED;
}

void Mannequin::Action()
{
	if (isArmed)
	{
		if (isAim)
		{
			SetState(STANDING_AIM_RECOIL, 1.5f);

			ProjectileManager::Get()->Shoot("playerArrow", bow->GlobalPos(), model->Back(), 100, 10);
			isAim = false;
		}
		else
		{
			SetState(STANDING_DRAW_ARROW, 1.5f);
			isAim = true;
		}
	}
}


void Mannequin::ReadClips()
{
	model->ReadClip("Unarmed Idle 01", true);
	model->ReadClip("Catwalk Walk Forward", true);
	model->ReadClip("Standing Equip Bow");
	model->GetClip(2)->SetEvent(bind(&Mannequin::Idle, this), 0.8f);

	model->ReadClip("Standing Idle 01", true);
	model->ReadClip("Standing Walk Forward", true);
	model->ReadClip("Standing Walk Back", true);
	model->ReadClip("Standing Walk Left", true);
	model->ReadClip("Standing Walk Right", true);

	model->ReadClip("Standing Draw Arrow");
	model->GetClip(4)->SetEvent(bind(&Mannequin::Aim, this), 0.8f);

	model->ReadClip("Standing Aim Idle 01");
	model->ReadClip("Standing Aim Walk Forward", true);
	model->ReadClip("Standing Aim Walk Back", true);
	model->ReadClip("Standing Aim Walk Left", true);
	model->ReadClip("Standing Aim Walk Right", true);

	model->ReadClip("Standing Aim Recoil");
	model->ReadClip("Standing Disarm Bow");
	model->GetClip(11)->SetEvent(bind(&Mannequin::Idle, this), 0.8f);
}

void Mannequin::SetAnimation()
{
	if (curState == EQUIP || curState == UNEQUIP) return;

	if (isArmed)
	{

	}
	else
	{
		
	}

	//if (velocity.Length() > 0.01f)
	//{
	//	if (isArmed)
	//	{
	//		//Forward
	//		if (dirInput[0] && !dirInput[1] && !dirInput[2] && !dirInput[3])
	//		{
	//			if (isAim)
	//			{
	//				SetState(STANDING_AIM_FORWARD);

	//			}
	//			else
	//			{
	//				SetState(STANDING_WALK_FORWARD);

	//			}
	//		}
	//		//Back
	//		if (!dirInput[0] && dirInput[1] && !dirInput[2] && !dirInput[3])
	//		{
	//			if (isAim)
	//			{
	//				SetState(STANDING_AIM_BACK);

	//			}
	//			else
	//			{
	//				SetState(STANDING_WALK_BACK);
	//			}
	//		}
	//		//Left
	//		if (!dirInput[0] && !dirInput[1] && dirInput[2] && !dirInput[3] ||
	//			dirInput[0] && !dirInput[1] && dirInput[2] && !dirInput[3] ||
	//			!dirInput[0] && dirInput[1] && dirInput[2] && !dirInput[3])
	//		{
	//			if (isAim)
	//			{
	//				SetState(STANDING_AIM_LEFT);

	//			}
	//			else
	//			{
	//				SetState(STANDING_WALK_LEFT);
	//			}
	//		}
	//		//Right
	//		if (!dirInput[0] && !dirInput[1] && !dirInput[2] && dirInput[3] ||
	//			dirInput[0] && !dirInput[1] && !dirInput[2] && dirInput[3] || 
	//			!dirInput[0] && dirInput[1] && !dirInput[2] && dirInput[3])
	//		{
	//			if (isAim)
	//			{
	//				SetState(STANDING_AIM_RIGHT);

	//			}
	//			else
	//			{
	//				SetState(STANDING_WALK_RIGHT);

	//			}
	//		}
	//	}
	//	else
	//	{
	//		SetState(UNARM_WALK_FORWARD);
	//	}
	//}
	/*else if (velocity.Length() <= 0.01f)
	{
		Idle();
	}*/
}
