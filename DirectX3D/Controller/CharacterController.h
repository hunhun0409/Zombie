#pragma once

class CharacterController
{
protected:
	BasicObject* controlledObject;

public:
	CharacterController(BasicObject* object = nullptr)
		: controlledObject(object) {}
	virtual ~CharacterController() = default;

	virtual void Possess(BasicObject* character);
	virtual void UnPossess();

	virtual void Update() = 0;
};
