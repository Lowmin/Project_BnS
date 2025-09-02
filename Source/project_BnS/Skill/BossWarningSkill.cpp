// Fill out your copyright notice in the Description page of Project Settings.


#include "BossWarningSkill.h"
#include "SkillData.h"
#include "../CharacterBase.h"
#include "Engine/World.h"
#include "TimerManager.h"

void ABossWarningSkill::ExecuteSkill_Implementation()
{
	ACharacter* OwnerCharacter = GetOwnerCharacter();
	if (!OwnerCharacter || !WarningIndicatorClass)
	{
		Destroy();
		return;
	}

	// 준비 동작
	//PlayAnimMontage(MyMontage);

	const FSkillType_Melee* Data = GetTypeData_Melee();
	if (Data)
	{
		const FVector Start = OwnerCharacter->GetActorLocation();
		const FVector Dir = OwnerCharacter->GetActorForwardVector();

		const FVector SpawnLocation = Start + Dir * Data->AttackLength;
		SpawnedIndicator = GetWorld()->SpawnActor<AActor>(WarningIndicatorClass, SpawnLocation, FRotator::ZeroRotator);
	}

	FTimerHandle AttackTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		AttackTimerHandle,
		this,
		&ABossWarningSkill::PerformDelayedAttack,
		WarningDuration,
		false);
}

void ABossWarningSkill::PerformDelayedAttack()
{
	if (SpawnedIndicator)
	{
		SpawnedIndicator->Destroy();
	}

	//PerformMeleeAttack();

	Destroy();
}

void ABossWarningSkill::OnSkillNotify_Hit()
{

}