#pragma once

class PlayerController : public CharacterController, public Singleton<PlayerController>
{
private:
	friend class Singleton;
	PlayerController(Character* character = nullptr);
	~PlayerController() = default;

public:
	virtual void Possess(Character* character) override;
	virtual void UnPossess() override;

	void Update() override;
	
private:
	bool showCursor = false;
};
