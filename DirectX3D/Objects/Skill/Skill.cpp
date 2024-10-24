#include "Framework.h"

Skill::Skill(SkillInfo info)
	:info(info)
{
}

Skill::Skill(string id, string name, wstring iconPath)
{
	info.id = id;
	info.name = name;
	info.iconPath = iconPath;
}

void Skill::LevelUp()
{
	if (level < MAX_LEVEL)
	{
		level++;
	}
}
