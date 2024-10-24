#include "Framework.h"

ActiveSkill::ActiveSkill(SkillInfo info, float cooldown, float duration)
    :Skill(info), baseCooldown(cooldown), cooldown(cooldown), baseDuration(duration), duration(duration)
{
}

ActiveSkill::ActiveSkill(string id, string name, wstring iconPath, float cooldown, float duration)
    :Skill(id, name, iconPath), baseCooldown(cooldown), cooldown(cooldown), baseDuration(duration), duration(duration)
{
}

void ActiveSkill::Update()
{
    if (isActive)
    {
        curDuration -= DELTA;
        if (curDuration <= 0)
        {
            Deactivate();
        }
    }
    else
    {
        curCooldown -= DELTA;
        if (curCooldown <= 0)
        {
            Activate();
        }
    }
}


void ActiveSkill::GUIRender()
{
}

void ActiveSkill::Activate()
{
    isActive = true;
    curCooldown = cooldown;
}

void ActiveSkill::Deactivate()
{
    isActive = false;
    curDuration = duration;
}

void ActiveSkill::LevelUp()
{
    Skill::LevelUp();
}
