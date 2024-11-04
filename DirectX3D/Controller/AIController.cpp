#include "Framework.h"

AIController::AIController(BasicObject* object)
	:CharacterController(object)
{
}

void AIController::Possess(BasicObject* object)
{
	CharacterController::Possess(object);
}

void AIController::UnPossess()
{
	CharacterController::UnPossess();
}

void AIController::Update() 
{
	InstanceZombie* controlledCharacter = (InstanceZombie*)controlledObject;
	if (target)
	{
		stateTimer -= DELTA;

		Vector3 velocity = target->GlobalPos() - 
			controlledCharacter->transform->GlobalPos();
		float targetDistance = velocity.Length();

		if (stateTimer <= 0){
			if (targetDistance > detectionRange){
				controlledCharacter->SetBehavior(InstanceZombie::NORMAL);
				stateTimer = STATE_CHANGE_COOLDOWN;
			}
			else if (targetDistance <= detectionRange 
				&& targetDistance > alertRange){
				if (controlledCharacter->curState != InstanceZombie::ALERT)
				{
					controlledCharacter->SetBehavior(InstanceZombie::CHASE);
					stateTimer = STATE_CHANGE_COOLDOWN;
				}
			}
			else{
				controlledCharacter->SetBehavior(InstanceZombie::ALERT);
				stateTimer = STATE_CHANGE_COOLDOWN;
			}
		}

		controlledCharacter->velocity = velocity;
	}
	else 
	{
		controlledCharacter->SetBehavior(InstanceZombie::NORMAL);
		stateTimer = STATE_CHANGE_COOLDOWN;
	}
}
