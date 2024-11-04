#include "Framework.h"

void CharacterController::Possess(BasicObject* character)
{
	if (controlledObject != nullptr)
	{
		UnPossess();
	}
	controlledObject = character;
	controlledObject->SetController(this);
}

void CharacterController::UnPossess()
{
	if (controlledObject == nullptr)
		return;

	controlledObject->SetController(nullptr);
	controlledObject = nullptr;
}
