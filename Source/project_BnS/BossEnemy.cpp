// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemy.h"
#include "Skill/SkillBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"

void ABossEnemy::SetCurrentSkill(ASkillBase* Skill)
{
	CurrentSkill = Skill;
}

bool ABossEnemy::IsCCImmune() const
{
	return bIsCCImmune;
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

void ABossEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GEngine && CrowdControl)
	{
		FString StateString = UEnum::GetValueAsString(CrowdControl->GetCrowdControlType());

		int32 CurrentStack = CrowdControl->GetCurrentStack();
		int32 MaxStack = CrowdControl->GetActivateStackCount();
		FString DebugString = FString::Printf(TEXT("Boss CC State: %s | Stack: %d / %d | Status: %s"),
			*StateString,
			CurrentStack,
			MaxStack,
			(bIsCCImmune ? TEXT("IMMUNE") : TEXT("VULNERABLE"))
		);

		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, DebugString);
	}
}