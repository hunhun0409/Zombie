#include "Framework.h"

Naruto::Naruto()
	:Character("Mannequin")
{
	rightHand = new Transform();

	kunai = new Model("kunai");
	kunai->SetParent(rightHand);
	kunai->SetTag("Kunai");
	kunai->Load();

	model->ReadClip("Idle", true);
	model->ReadClip("Standing Walk Forward", true);
	model->ReadClip("Standing Walk Back", true);
	model->ReadClip("Standing Walk Left", true);
	model->ReadClip("Standing Walk Right", true);
	model->ReadClip("THrow");
	model->GetClip(5)->SetEvent(bind(&Naruto::ThrowKunai, this), 0.15f);
	model->GetClip(5)->SetEvent(bind(&Naruto::Idle, this), 0.9f);

	crossHair = new Quad(L"Textures/UI/cursor.png");
	crossHair->Pos() = { CENTER_X, CENTER_Y, 0 };
	crossHair->UpdateWorld();


	ProjectileManager::Get()->Add("kunai", "kunai", Vector3(0.1f, 0.1f, 0.1f));

	light = Environment::Get()->GetLight(0);
	light->range = 200.0f;
	light->inner = 40;
	light->outer = 70;

}

Naruto::~Naruto()
{
	delete rightHand;
	delete kunai;
}

void Naruto::Update()
{
	Character::Update();

	Control();
	SetAnimation();

	rightHand->SetWorld(model->GetTransformByNode(11));

	kunai->UpdateWorld();
}

void Naruto::Render()
{
	Character::Render();

	kunai->Render();
}

void Naruto::PostRender()
{
	crossHair->Render();
}

void Naruto::GUIRender()
{
	Character::GUIRender();
	kunai->GUIRender();

}

void Naruto::Idle()
{
	SetState(IDLE);
	isPlayingRootMotion = false;
}

void Naruto::ThrowKunai()
{
	Vector3 dir = Forward();

	if (isTarget)
	{
		dir = targetPos - kunai->GlobalPos();
		dir.Normalize();
	}

	ProjectileManager::Get()->Shoot(kunai->GetName(), kunai->GlobalPos(), dir, 100.0f, 100);
}

void Naruto::Control()
{
	//Rotate();
	//Move();
	Attack();
}

void Naruto::Move()
{
	/*if (curState == THROW) return;

	bool isMoveZ = false;
	bool isMoveX = false;

	if (KEY_PRESS('W'))
	{
		isMoveZ = true;
	}
	if (KEY_PRESS('S'))
	{
		isMoveZ = true;
	}
	if (KEY_PRESS('D'))
	{
		isMoveX = true;
	}
	if (KEY_PRESS('A'))
	{
		isMoveX = true;
	}

	if (velocity.Length() > 1.0f)
	{
		velocity.Normalize();
	}

	if (!isMoveX)
	{
		velocity.x = Lerp(velocity.x, 0, deceleration * DELTA);
	}
	if (!isMoveZ)
	{
		velocity.z = Lerp(velocity.z, 0, deceleration * DELTA);
	}

	Matrix rotY = XMMatrixRotationY(Rot().y);
	Vector3 direction = XMVector3TransformCoord(velocity, rotY);

	Pos() -= direction * moveSpeed * DELTA;*/
}

void Naruto::Rotate()
{
	//Vector3 delta = mousePos - Vector3(CENTER_X, CENTER_Y);
	//SetCursorPos(clientCenterPos.x, clientCenterPos.y);


	//CAM->Rot().y += delta.x * rotYSpeed * DELTA;
	//CAM->Rot().x -= delta.y * rotXSpeed * DELTA;
	//CAM->Rot().x = Clamp(0, XM_PI * 0.1f, CAM->Rot().x);

	//Rot().y = CAM->Rot().y + XM_PI;
}

void Naruto::Attack()
{
	if (curState == THROW) return;


	if (KEY_DOWN(VK_LBUTTON))
	{
		SetState(THROW);
		isPlayingRootMotion = true;

		Ray ray = CAM->ScreenPointToRay(mousePos);
		isTarget = ZombieManager::Get()->IsCollision(ray, targetPos);

		targetPos;
	}
}

void Naruto::SetAnimation()
{
	if (curState == THROW) return;

	if (characterMovement->velocity.z > 2.0f)
	{
		SetState(RUN_FORWARD, 1.5f);
	}
	else if (characterMovement->velocity.z < -2.0f)
	{
		SetState(RUN_BACK, 1.5f);
	}
	else if (characterMovement->velocity.x > 2.0f)
	{
		SetState(RUN_RIGHT, 1.5f);
	}
	else if (characterMovement->velocity.x < -2.0f)
	{
		SetState(RUN_LEFT, 1.5f);
	}
	else
	{
		Idle();
	}
}

void Naruto::SetState(State state, float playRate, float takeTime)
{
	if (curState == state) return;

	curState = state;
	model->PlayClip(state, playRate, takeTime);
}
