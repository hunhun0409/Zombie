#include "Framework.h"

PassiveSkill::PassiveSkill(SkillInfo info, float baseEffectValue, float effectPerLevel)
    :Skill(info), baseEffectValue(baseEffectValue), effectPerLevel(effectPerLevel), effectValue(baseEffectValue)
{
}

PassiveSkill::PassiveSkill(string id, string name, wstring iconPath, float baseEffectValue, float effectPerLevel)
    :Skill(id, name, iconPath), baseEffectValue(baseEffectValue), effectPerLevel(effectPerLevel), effectValue(baseEffectValue)
{
}

void PassiveSkill::Activate()
{
    //owner에게 effectValue전달?
    //
}

void PassiveSkill::Deactivate()
{
}


void PassiveSkill::LevelUp()
{
    Skill::LevelUp();
}
