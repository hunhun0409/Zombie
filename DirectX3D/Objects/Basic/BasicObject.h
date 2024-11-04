#pragma once
class CharacterController;
class BasicObject
{
public:
	virtual ~BasicObject() = default;

	void SetController(CharacterController* controller) { this->controller = controller; }
protected:
	CharacterController* controller = nullptr;
};

