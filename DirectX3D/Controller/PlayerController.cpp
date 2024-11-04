#include "Framework.h"

PlayerController::PlayerController(BasicObject* character)
	:CharacterController(character)
{
	Environment::Get()->AddLight();
	light = Environment::Get()->GetLight(1);
	light->active = 0;
	light->type = 1;
	light->range = 150.0f;
}

void PlayerController::Possess(BasicObject* character)
{
	CharacterController::Possess(character);

	Transform* transform = (Transform*)character;

	CAM->SetTarget(transform);
	CAM->TargetOptionLoad("PlayerCamera");
	//CAM->LookAtTarget();

	light->active = 1;
}

void PlayerController::UnPossess()
{
	if (controlledObject == nullptr)
		return;
	CharacterController::UnPossess();
	
	CAM->SetTarget(nullptr);

	light->active = 0;
}

void PlayerController::Update()
{
	if (Timer::Get()->GetDeltaScale() == 0) return;

	Character* controlledCharacter = (Character*)controlledObject;

	if (controlledCharacter)
	{
		if (controlledCharacter == nullptr) return;
		if (controlledCharacter->isStop) return;
		if (controlledCharacter->IsDead()) return;

		if (KEY_DOWN(VK_SHIFT)) {
			controlledCharacter->Shift();
		}
		if (KEY_DOWN(VK_LBUTTON)) {
			controlledCharacter->Action();
		}

		Vector3 inputDirection = Vector3();
		if (KEY_PRESS('W')) {
			inputDirection += Vector3(0, 0, 1);
		}
		if (KEY_PRESS('S'))
		{
			inputDirection += Vector3(0, 0, -1);
		}
		if (KEY_PRESS('A'))
		{
			inputDirection += Vector3(-1, 0, 0);
		}
		if (KEY_PRESS('D'))
		{
			inputDirection += Vector3(1, 0, 0);
		}

		if (inputDirection.Length() > 0.1f) {
			controlledCharacter->GetCharacterMovement()->
				ApplyAcceleration(inputDirection.GetNormalized());
		}

		Vector2 dir;
		dir.x = mousePos.x / WIN_WIDTH * 2 - 1;
		dir.y = mousePos.y / WIN_HEIGHT * 2 - 1;
		dir.Normalize();

		controlledCharacter->Rot().y =
			atan2(dir.x, dir.y) + XM_PI / 2;


		if (KEY_DOWN('R'))
		{
			controlledCharacter->WeaponA();
		}

		if (KEY_DOWN(VK_RBUTTON) || KEY_UP(VK_RBUTTON))
		{
			controlledCharacter->RButton();
		}

		if (KEY_DOWN(VK_F3))
		{
			showCursor = !showCursor;
			ShowCursor(showCursor);

		}

		light->pos = controlledCharacter->GlobalPos();
		light->pos.y = 20.0f;
	}
}
