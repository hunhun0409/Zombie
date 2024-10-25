#include "Framework.h"

AttackUpgrade::AttackUpgrade()
	:PassiveSkill("attack", "attack", L"Textures/UI/AttackIcon.png", 0, 10)
{
}

void AttackUpgrade::Activate()
{
	PassiveSkill::Activate();
}

void AttackUpgrade::Deactivate()
{
	PassiveSkill::Deactivate();
}

void AttackUpgrade::LevelUp()
{
	PassiveSkill::LevelUp();

	effectValue = baseEffectValue + effectPerLevel * level;
	Character::StatusUpgradeInfo& status = owner->GetUpgradeStatus();
	status.upgradeAttack = effectValue;
	owner->UpdateStatus();
}

void AttackUpgrade::Update()
{
	PassiveSkill::Update();
}

void AttackUpgrade::Render()
{
	PassiveSkill::Render();
}
