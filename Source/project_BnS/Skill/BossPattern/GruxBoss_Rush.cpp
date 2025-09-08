// Fill out your copyright notice in the Description page of Project Settings.


#include "GruxBoss_Rush.h"
#include "../../CharacterBase.h"
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
	ACharacter* OwnerCharacter = GetOwnerCharacter();
	AActor* Target = GetSkillTarget();

	if (!OwnerCharacter || !Target)
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
	
	DashDirection = (Target->GetActorLocation() - OwnerCharacter->GetActorLocation()).GetSafeNormal();
	DashDirection.Z = 0;

	SetActorTickEnabled(false);

	FTimerHandle AttackTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &AGruxBoss_Rush::PerformDash, 3.0f, false);
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

	DashSpeed = 1500.0f;
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

void AGruxBoss_Rush::EndDash()
{
	SetActorTickEnabled(false);

	ACharacter* OwnerCharacter = GetOwnerCharacter();
	if (OwnerCharacter)
	{
		// 2. 관성이 남지 않도록 속도를 0으로 만들고, 이벤트를 해제합니다.
		OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
		OwnerCharacter->GetCapsuleComponent()->OnComponentBeginOverlap.RemoveDynamic(this, &AGruxBoss_Rush::OnDashOverlap);
	}

	Destroy();
}