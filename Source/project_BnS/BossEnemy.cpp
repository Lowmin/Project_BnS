// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemy.h"
#include "Skill/SkillBase.h"

void ABossEnemy::SetCurrentSkill(ASkillBase* Skill)
{
	CurrentSkill = Skill;
}

void ABossEnemy::SetCCImmune(bool bImmune)
{
	bIsCCImmune = bImmune;
}

void ABossEnemy::CCApplied()
{
	if (bIsCCImmune) return;

	if (CrowdControl && CrowdControl->GetActivateStackCount() > 0)
	{
		Super::CCApplied();

		if (CurrentSkill)
		{
			CurrentSkill->CancelSkill();
		}
		SetCCImmune(true);
		if (CrowdControl)
		{
			CrowdControl->SetActivateStackCount(0);
		}
		SetCurrentSkill(nullptr);
	}
	else
	{
		Super::CCApplied();
	}
}

void ABossEnemy::CCRemoved()
{
	Super::CCRemoved();
}
