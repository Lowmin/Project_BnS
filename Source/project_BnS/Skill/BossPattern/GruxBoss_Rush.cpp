// Fill out your copyright notice in the Description page of Project Settings.


#include "GruxBoss_Rush.h"
#include "../../CharacterBase.h"
#include "../../CrowdControlComponent.h"
#include "../../BossEnemy.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AGruxBoss_Rush::AGruxBoss_Rush()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGruxBoss_Rush::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ACharacter* OwnerCharacter = GetOwnerCharacter();
	if (OwnerCharacter)
	{
		const FVector ConstantVelocity = DashDirection * 1000.f;
		OwnerCharacter->GetCharacterMovement()->Velocity = ConstantVelocity;
	}
}

void AGruxBoss_Rush::ExecuteSkill_Implementation()
{
	ABossEnemy* OwnerBoss = Cast<ABossEnemy>(GetOwnerCharacter());
	AActor* Target = GetSkillTarget();

	if (!OwnerBoss || !Target)
	{
		Destroy();
		return;
	}

	if (OwnerBoss)
	{
		OwnerBoss->SetCCImmune(false);

		UCrowdControlComponent* CC_Comp = OwnerBoss->GetCrowdControlComponent();
		if (CC_Comp)
		{
			CC_Comp->SetActivateStackCount(2);
		}
	}

	if (MyMontage)
	{
		if (UAnimInstance* AnimInstance = GetOwnerAnimInstance())
		{
			AnimInstance->Montage_Play(MyMontage);
		}
	}

	
	DashDirection = (Target->GetActorLocation() - OwnerBoss->GetActorLocation()).GetSafeNormal();
	DashDirection.Z = 0;

	SetActorTickEnabled(false);

	FTimerHandle AttackTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &AGruxBoss_Rush::PerformDash, 5.0f, false);
}

void AGruxBoss_Rush::PerformDash()
{
	if (SpawndIndicator)
	{
		SpawndIndicator->Destroy();
	}

	ACharacter* OwnerCharacter = GetOwnerCharacter();
	if (!OwnerCharacter)
	{
		Destroy();
		return;
	}

	OwnerCharacter->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AGruxBoss_Rush::OnDashOverlap);

	DashSpeed = 3000.0f;
	const float DashDistance = 3000.f;

	const float DashDuration = (DashSpeed > 0) ? (DashDistance / DashSpeed) : 0.0f;

	SetActorTickEnabled(true);

	FTimerHandle EndDashTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(EndDashTimerHandle, this, &AGruxBoss_Rush::EndDash, DashDuration, false);
}

void AGruxBoss_Rush::OnDashOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Error, TEXT("OnDashHit CALLED! Hit Actor: %s"), *OtherActor->GetName());

	if (OtherActor && OtherActor != GetOwner() && OtherActor->ActorHasTag(FName("Player")) && !HitActors.Contains(OtherActor))
	{
		ApplyDamageToCharacter(Cast<ACharacter>(OtherActor));
		HitActors.Add(OtherActor);
	}
}

void AGruxBoss_Rush::CancelSkill()
{

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	if (SpawndIndicator)
	{
		SpawndIndicator->Destroy();
	}

	Destroy();
}

void AGruxBoss_Rush::EndDash()
{
	SetActorTickEnabled(false);

	ABossEnemy* OwnerBoss = Cast<ABossEnemy>(GetOwnerCharacter());
	if (OwnerBoss)
	{
		OwnerBoss->SetCCImmune(true);
		UCrowdControlComponent* CC_Comp = OwnerBoss->GetCrowdControlComponent();
		if (CC_Comp)
		{
			CC_Comp->SetActivateStackCount(0);
		}
		OwnerBoss->StopAnimMontage();
	}

	Destroy();
}