// Fill out your copyright notice in the Description page of Project Settings.


#include "GruxBoss_Rush.h"
#include "../../CharacterBase.h"

AGruxBoss_Rush::AGruxBoss_Rush()
{
}

void AGruxBoss_Rush::ExecuteSkill_Implementation()
{
	ACharacter* OwnerCharacter = GetOwnerCharacter();
	if (!OwnerCharacter || !WarningIndicatorClass)
	{
		Destroy();
		return;
	}

	if (MyMontage)
	{
		if (UAnimInstance* AnimInstance = GetOwnerAnimInstance())
		{
			AnimInstance->Montage_Play(MyMontage);
		}
	}
	const FSkillType_Area* AreaData = GetTypeData_Area();
	if (!AreaData)
	{
		Destroy();
		return;
	}

	SpawndIndicator = GetWorld()->SpawnActor<AWarningIndicator>(WarningIndicatorClass, OwnerCharacter->GetActorLocation(), FRotator::ZeroRotator);

	if (SpawndIndicator)
		SpawndIndicator->SetDecalSize(AreaData->Radius);

	FTimerHandle AttackTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		AttackTimerHandle,
		this,
		&AGruxBoss_Rush::PerformDash,
		WarningDuration,
		false);
}

void AGruxBoss_Rush::PerformDash()
{
	if (SpawndIndicator)
	{
		SpawndIndicator->Destroy();
	}

	ACharacter* OwnerCharacter = GetOwnerCharacter();
	
	const FVector RushVelocity = OwnerCharacter->GetActorForwardVector() * DashSpeed;
}

void AGruxBoss_Rush::EndDash()
{
}
