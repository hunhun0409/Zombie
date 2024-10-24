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
	//Skill���� ���� �̸� ������
	//Rifle : active ���
	ActiveSkill* orbitalRifle = new OrbitalRifle();
	totalActiveSkills[orbitalRifle->Info().id] = orbitalRifle;

	//Armor : passive ���
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
bool SkillManager::Add(string key)
{
	// Active ��ų üũ �� �߰�
	if (totalActiveSkills.count(key) != 0)
	{
		if (currentActiveSkills.count(key) != 0) return false;
		totalActiveSkills[key]->Info().isUnlocked = true;
		currentActiveSkills[key] = totalActiveSkills[key];
		return true;
	}
	// Passive ��ų üũ �� �߰�
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

	//to do : ���� ��ų ������ ��� �߰��������
	if (currentActiveSkills.count(key) != 0)
	{
		currentActiveSkills[key]->LevelUp();
		return true;
	}
	if (currentPassiveSkills.count(key) != 0)
	{
		currentPassiveSkills[key]->LevelUp();
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
	// Active ��ų Ȯ��

	for (auto& pair : totalActiveSkills)
	{
		Skill* skill = pair.second;
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
	// Passive ��ų Ȯ��
	for (auto& pair : totalPassiveSkills)
	{
		Skill* skill = pair.second;
		if (!skill->IsMaxLevel())
		{
			availableSkills.push_back(skill);
		}
	}

	vector<Skill*> selectedSkills;
	while (selectedSkills.size() < 3 && !availableSkills.empty())
	{
		int randIndex = rand() % availableSkills.size();
		selectedSkills.push_back(availableSkills[randIndex]);
		availableSkills.erase(availableSkills.begin() + randIndex);
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
