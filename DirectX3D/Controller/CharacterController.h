#pragma once

class CharacterController
{
protected:
	Character* controlledCharacter;

public:
	CharacterController(Character* character = nullptr)
		: controlledCharacter(character) {}
	virtual ~CharacterController() = default;

	virtual void Possess(Character* character);
	virtual void UnPossess();

	virtual void Update() = 0;
};
