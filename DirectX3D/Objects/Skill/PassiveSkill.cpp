#include "Framework.h"

void PassiveSkill::Activate()
{
}

void PassiveSkill::Deactivate()
{
}


void PassiveSkill::LevelUp()
{
    Skill::LevelUp();
}

string PassiveSkill::GetDescription() const
{
    return string();
}
