#include "Framework.h"

RecoveryUpgrade::RecoveryUpgrade()
	:PassiveSkill("recovery", "recovery", L"Textures/UI/RecoveryIcon.png", 0, 1)
{
}

void RecoveryUpgrade::Activate()
{
	PassiveSkill::Activate();
}

void RecoveryUpgrade::Deactivate()
{
	PassiveSkill::Deactivate();
}

void RecoveryUpgrade::LevelUp()
{
	PassiveSkill::LevelUp();

	effectValue = baseEffectValue + effectPerLevel * level;
	Character::StatusUpgradeInfo& status = owner->GetUpgradeStatus();
	status.upgradeRecovery = effectValue;
	owner->UpdateStatus();
}

void RecoveryUpgrade::Update()
{
	PassiveSkill::Update();
}

void RecoveryUpgrade::Render()
{
	PassiveSkill::Render();
}
