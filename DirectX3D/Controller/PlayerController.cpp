#include "Framework.h"

PlayerController::PlayerController(Character* character)
	:CharacterController(character)
{

}

void PlayerController::Possess(Character* character)
{
	CharacterController::Possess(character);
	showCursor = false;
	ShowCursor(showCursor);
}

void PlayerController::UnPossess()
{
	if (controlledCharacter == nullptr)
		return;

	CharacterController::UnPossess();
	showCursor = true;
	ShowCursor(showCursor);
}

void PlayerController::Update()
{
	if (controlledCharacter == nullptr) return;
	if (controlledCharacter->IsDead()) return;

	if (!controlledCharacter->isPlayingRootMotion)
	{
		Vector3 inputDirection = Vector3();
		if (KEY_PRESS('W'))
		{
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
		if (inputDirection.Length() > 0.1f)
		{
			controlledCharacter->GetCharacterMovement()->ApplyAcceleration(inputDirection.GetNormalized());
		}
	}
	else
	{
		controlledCharacter->GetCharacterMovement()->Stop();
	}
	
	if (KEY_DOWN('R'))
	{
		controlledCharacter->WeaponA();
	}
	if (KEY_DOWN(VK_SHIFT))
	{
		controlledCharacter->Shift();
	}
	if (KEY_DOWN(VK_LBUTTON))
	{
		controlledCharacter->Action();
	}
	if (KEY_DOWN(VK_RBUTTON) || KEY_UP(VK_RBUTTON))
	{
		controlledCharacter->RButton();
	}

	if (KEY_DOWN(VK_F3))
	{
		showCursor = !showCursor;
		ShowCursor(showCursor);

		//mousePos = Vector3(CENTER_X, CENTER_Y);
		//SetCursorPos(clientCenterPos.x, clientCenterPos.y);
	}


	Vector2 dir;
	dir.x = mousePos.x / WIN_WIDTH * 2 - 1;
	dir.y = mousePos.y / WIN_HEIGHT * 2 - 1;
	dir.Normalize();

	controlledCharacter->Rot().y = atan2(dir.x, dir.y) + XM_PI/2;

	//transform->Rot().y = atan2(dir.x, dir.z) + XM_PI;
	//transform->Pos() += vel * speed * DELTA;

	//if (!showCursor)
	//{
	//	Vector3 delta = 2 * (mousePos - Vector3(CENTER_X, CENTER_Y));
	//	SetCursorPos(clientCenterPos.x, clientCenterPos.y);

	//	controlledCharacter->Rot().y += delta.x * DELTA;
	//	//CAM->Rot().x -= delta.y * DELTA;
	//	//controlledCharacter->Rot().x += delta.y * DELTA;
	//}
	
}
