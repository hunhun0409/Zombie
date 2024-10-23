#include "Framework.h"

Skill::Skill(string id, string name)
	:id(id), name(name)
{
	info.name = name;
}

void Skill::LevelUp()
{
	if (level < MAX_LEVEL)
	{
		level++;
	}
}
