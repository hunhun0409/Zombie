#include "Framework.h"

ActiveSkill::ActiveSkill(string id, string name, float cooldown, float duration)
    :Skill(id, name), cooldown(cooldown), duration(duration)
{
}

void ActiveSkill::Update()
{
    curCooldown -= DELTA;
    curDuration -= DELTA;
}


void ActiveSkill::GUIRender()
{
}

void ActiveSkill::Activate()
{
    isActive = true;

    curCooldown = cooldown;
    curDuration = duration;
}

void ActiveSkill::Deactivate()
{
    isActive = false;
}

void ActiveSkill::LevelUp()
{
}

string ActiveSkill::GetDescription() const
{
    return string();
}
