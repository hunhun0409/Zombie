#pragma once

class PlayerController : public CharacterController, public Singleton<PlayerController>
{
private:
	friend class Singleton;
	PlayerController(BasicObject* character = nullptr);
	~PlayerController() = default;

public:
	virtual void Possess(BasicObject* character) override;
	virtual void UnPossess() override;

	void Update() override;
	
private:
	bool showCursor = true;

	LightBuffer::Light* light;

};
