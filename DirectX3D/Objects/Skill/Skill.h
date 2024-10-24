#pragma once

class Skill : public BasicObject
{
public:
	struct SkillInfo
	{
		string id;
		string name;
		wstring iconPath;
		bool isUnlocked = false;
		int maxLevel = 5;
	};

	const int MAX_LEVEL = 5;
public:
	Skill(SkillInfo info);
	Skill(string id, string name, wstring iconPath);
	~Skill() = default;

	virtual void Activate() = 0;
	virtual void Deactivate() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void LevelUp();

	//공통 기능 
	bool CanLevelUp() const { return level < info.maxLevel; }
	SkillInfo& Info() { return info; }
	int GetCurrentLevel() const { return level; }
	bool IsUnlocked() const { return info.isUnlocked; }
	bool IsMaxLevel() const { return level == info.maxLevel; }
	virtual void SetOwner(class Character* owner) { this->owner = owner; }

protected:
	class Character* owner = nullptr;
	SkillInfo info;
	int level = 0;
};
