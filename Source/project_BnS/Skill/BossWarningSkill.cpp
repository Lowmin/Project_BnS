// Fill out your copyright notice in the Description page of Project Settings.


#include "BossWarningSkill.h"
#include "SkillData.h"
#include "../CharacterBase.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"

void ABossWarningSkill::ExecuteSkill_Implementation()
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

	SpawnedIndicator = GetWorld()->SpawnActor<AWarningIndicator>(WarningIndicatorClass, OwnerCharacter->GetActorLocation(), FRotator::ZeroRotator);

	if(SpawnedIndicator)
		SpawnedIndicator->SetDecalSize(AreaData->Radius);

	FTimerHandle AttackTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		AttackTimerHandle,
		this,
		&ABossWarningSkill::PerformDelayedAttack,
		WarningDuration,
		false);
}

ABossWarningSkill::ABossWarningSkill()
{
	static ConstructorHelpers::FClassFinder<AWarningIndicator> Indicator(TEXT("/Game/Enemy/Boss/BP_WarningIndicator.BP_WarningIndicator_C"));

	if (Indicator.Succeeded())
	{
		WarningIndicatorClass = Indicator.Class;
	}
}

void ABossWarningSkill::PerformDelayedAttack()
{
	if (SpawnedIndicator)
	{
		SpawnedIndicator->Destroy();
	}

	ACharacter* OwnerCharacter = GetOwnerCharacter();
	const FSkillType_Area* AreaData = GetTypeData_Area();
	if (!OwnerCharacter || !AreaData)
	{
		Destroy();
		return;
	}

	const FVector Center = OwnerCharacter->GetActorLocation();
	const float AttackRadius = AreaData->Radius;
	UE_LOG(LogTemp, Error, TEXT("Attack Radius from Data Table: %f"), AttackRadius);


	FCollisionQueryParams QP;
	QP.AddIgnoredActor(OwnerCharacter);

	TArray<FHitResult> HitResults;
	GetWorld()->SweepMultiByChannel(
		HitResults,
		Center,
		Center,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(AttackRadius),
		QP
	);

	DrawDebugSphere(GetWorld(), Center, AttackRadius, 32, FColor::Red, false, 2.0f);

	TSet<AActor*> DamagedActors;
	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor && !DamagedActors.Contains(HitActor))
		{
			ApplyDamageToCharacter(Cast<ACharacter>(HitActor));

			DamagedActors.Add(HitActor);
		}
	}

	Destroy();
}
