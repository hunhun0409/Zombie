#include "Framework.h"

void CharacterController::Possess(Character* character)
{
	controlledCharacter = character;
	controlledCharacter->SetController(this);
}

void CharacterController::UnPossess()
{
	if (controlledCharacter == nullptr)
		return;

	controlledCharacter->SetController(nullptr);
	controlledCharacter = nullptr;
}
