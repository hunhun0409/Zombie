#pragma once

class AiController
{
public:
	AiController();
	~AiController();

	void Update();



private:
	InstanceCharacter* owner;

	Transform* target;
};
