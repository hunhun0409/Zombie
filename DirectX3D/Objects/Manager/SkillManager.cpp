#include "Framework.h"

SkillManager::SkillManager()
{
	Init();
}

SkillManager::~SkillManager()
{
	for (auto skill : totalActiveSkills)
	{
		delete skill.second;
	}
	for (auto skill : totalPassiveSkills)
	{
		delete skill.second;
	}
}

void SkillManager::Init()
{
	//Skill들을 전부 미리 생성함
	//Rifle : active 등등
	ActiveSkill* orbitalRifle = new OrbitalRifle();
	totalActiveSkills[orbitalRifle->Info().id] = orbitalRifle;

	ActiveSkill* orbitalSniper = new OrbitalSniper();
	totalActiveSkills[orbitalSniper->Info().id] = orbitalSniper;

	ActiveSkill* heal = new Heal();
	totalActiveSkills[heal->Info().id] = heal;

	//Armor : passive 등등
	PassiveSkill* hp = new HpUpgrade();
	totalPassiveSkills[hp->Info().id] = hp;

	PassiveSkill* armor = new ArmorUpgrade();
	totalPassiveSkills[armor->Info().id] = armor;

	PassiveSkill* attack = new AttackUpgrade();
	totalPassiveSkills[attack->Info().id] = attack;

	PassiveSkill* recovery = new RecoveryUpgrade();
	totalPassiveSkills[recovery->Info().id] = recovery;
}

void SkillManager::Update()
{
	for (auto skill : currentActiveSkills)
	{
		skill.second->Update();
	}
	for (auto skill : currentPassiveSkills)
	{
		skill.second->Update();
	}
}

void SkillManager::Render()
{
	for (auto skill : currentActiveSkills)
	{
		skill.second->Render();
	}
	for (auto skill : currentPassiveSkills)
	{
		skill.second->Render();
	}
}
void SkillManager::GUIRender()
{
	for (auto skill : currentActiveSkills)
	{
		skill.second->GUIRender();
	}
}
bool SkillManager::Add(string key)
{
	// Active 스킬 체크 및 추가
	if (totalActiveSkills.count(key) != 0)
	{
		if (currentActiveSkills.count(key) != 0) return false;
		totalActiveSkills[key]->Info().isUnlocked = true;
		currentActiveSkills[key] = totalActiveSkills[key];
		return true;
	}
	// Passive 스킬 체크 및 추가
	if (totalPassiveSkills.count(key) != 0)
	{
		if (currentPassiveSkills.count(key) != 0) return false;
		totalPassiveSkills[key]->Info().isUnlocked = true;
		currentPassiveSkills[key] = totalPassiveSkills[key];
		return true;
	}
	return false;
}

bool SkillManager::LevelUp(string key)
{

	//to do : 없는 스킬 레벨업 경우 추가해줘야함
	if (totalActiveSkills.count(key) != 0)
	{
		if (totalActiveSkills[key]->GetCurrentLevel() == 0)
		{
			Add(key);
		}
		totalActiveSkills[key]->LevelUp();
		return true;
	}
	if (totalPassiveSkills.count(key) != 0)
	{
		if (totalPassiveSkills[key]->GetCurrentLevel() == 0)
		{
			Add(key);
		}
		totalPassiveSkills[key]->LevelUp();
		return true;
	}
	return false;
}

void SkillManager::Clear()
{
	currentActiveSkills.clear();
	currentPassiveSkills.clear();
}

vector<Skill*> SkillManager::GetThreeRandomSkills()
{
	vector<Skill*> availableSkills;
	// Active 스킬 확인

	for (auto& pair : totalActiveSkills)
	{
		Skill* skill = pair.second;
		if (skill->Info().id == "heal")
			continue;

		if (currentActiveSkills.size() < 5)
		{
			if (!skill->IsMaxLevel())
			{
				availableSkills.push_back(skill);
			}
		}
		else
		{
			if (skill->Info().isUnlocked && !skill->IsMaxLevel())
			{
				availableSkills.push_back(skill);
			}
		}
	}
	// Passive 스킬 확인
	for (auto& pair : totalPassiveSkills)
	{
		Skill* skill = pair.second;
		if (currentPassiveSkills.size() < 5)
		{
			if (!skill->IsMaxLevel())
			{
				availableSkills.push_back(skill);
			}
		}
		else
		{
			if (skill->Info().isUnlocked && !skill->IsMaxLevel())
			{
				availableSkills.push_back(skill);
			}
		}
	}

	vector<Skill*> selectedSkills;
	while (selectedSkills.size() < 3 && !availableSkills.empty())
	{
		int randIndex = rand() % availableSkills.size();
		selectedSkills.push_back(availableSkills[randIndex]);
		availableSkills.erase(availableSkills.begin() + randIndex);
	}

	while (selectedSkills.size() < 3)
	{
		selectedSkills.push_back(totalActiveSkills["heal"]);
	}

	return selectedSkills;
}

bool SkillManager::HasActiveSkill(string key)
{
	return currentActiveSkills.count(key) != 0;
}

bool SkillManager::HasPassiveSkill(string key)
{
	return currentPassiveSkills.count(key) != 0;
}

PassiveSkill* SkillManager::GetPassiveSkill(string key)
{
	if (HasPassiveSkill(key))
	{
		return currentPassiveSkills[key];
	}
	else
	{
		assert(0);
	}
}

ActiveSkill* SkillManager::GetActiveSkill(string key)
{
	if (HasPassiveSkill(key))
	{
		return currentActiveSkills[key];
	}
	else
	{
		assert(0);
	}
}

int SkillManager::GetSkillLevel(string key)
{
	if (totalActiveSkills.count(key) != 0)
		return totalActiveSkills[key]->GetCurrentLevel();

	if (totalPassiveSkills.count(key) != 0)
		return totalPassiveSkills[key]->GetCurrentLevel();

	return -1;
}

void SkillManager::UpdateChange()
{
	for (auto skill : totalActiveSkills)
	{
		skill.second->UpdateChange();
	}
}

void SkillManager::SetOwner(Character* owner)
{
	this->owner = owner;
	for (auto skill : totalActiveSkills)
	{
		skill.second->SetOwner(owner);
	}
	for (auto skill : totalPassiveSkills)
	{
		skill.second->SetOwner(owner);
	}
}
