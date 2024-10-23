#pragma once

class Skill : public BasicObject
{
public:
	struct SkillInfo
	{
		string name;
		string description[5];
	};


	const int MAX_LEVEL = 5;
public:
	Skill(string id, string name);
	~Skill() = default;

	virtual void Activate() = 0;
	virtual void Deactivate() = 0;
	virtual void LevelUp();
	virtual string GetDescription() const = 0;

	virtual void SetOwner(class Character* owner) { this->owner = owner; }

protected:
	class Character* owner = nullptr;
	SkillInfo info;
	string id;
	string name;
	int level = 1;
};
