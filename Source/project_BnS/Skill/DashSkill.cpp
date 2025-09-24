// Fill out your copyright notice in the Description page of Project Settings.


#include "DashSkill.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "../TargetAble.h"

// Sets default values
ADashSkill::ADashSkill()
{
	PrimaryActorTick.bCanEverTick = true;
	bAutoDestroyOnMontageEnd = false;
}

void ADashSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDash)
	{
		ACharacter* Character = GetOwnerCharacter();
		const FSkillType_Dash* DashData = GetTypeData_Dash();
		if (!Character || !DashData)
		{
			bIsDash = false;
			Destroy();
			return;
		}

		FVector CurrentLocation = Character->GetActorLocation();
		FVector TargetVector = TargetLocation - CurrentLocation;

		const float TargetDistance = FVector::Dist(Character->GetActorLocation(), TargetLocation);

		if (TargetDistance <= 50.f)	// 오차 범위 
		{
			bIsDash = false;
			Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);	// 이동 후 상태
			PerformMelee(DashData);

			if (!MyMontage)
			{
				Destroy();
			}
		}
		else
		{
			TargetVector.Normalize();
			float MoveDistance = DashData->DashSpeed * DeltaTime;

			FVector NewLocation = CurrentLocation + (TargetVector * MoveDistance);
			Character->SetActorLocation(NewLocation);

			FVector LookTargetDirection = (GetSkillTarget()->GetActorLocation() - CurrentLocation);
			LookTargetDirection.Normalize();
			FRotator LookRotation = LookTargetDirection.Rotation();
			Character->SetActorRotation(FRotator(0.f, LookRotation.Yaw, 0.f));
		}
	}
}

void ADashSkill::ExecuteSkill_Implementation()
{
	Super::ExecuteSkill_Implementation();

	AActor* Target = GetSkillTarget();
	ACharacter* Character = GetOwnerCharacter();
	const FSkillType_Dash* DashData = GetTypeData_Dash();

	if (!Target || !Character || !DashData)
	{
		Destroy();
		return;
	}

	FVector TargetDirection = (Target->GetActorLocation() - Character->GetActorLocation());
	TargetDirection.Normalize();

	float targetSize = 0;
	if (Target->GetClass()->ImplementsInterface(UTargetAble::StaticClass()))
	{
		targetSize = ITargetAble::Execute_GetTargetBoxSize(Target).X;
	}

	TargetLocation = Target->GetActorLocation() - (TargetDirection * DashData->StopDistance) - (TargetDirection * targetSize);

	bIsDash = true;
	Character->GetCharacterMovement()->SetMovementMode(MOVE_Flying); // 공중에서도 사용 가능

}

void ADashSkill::OnSkillMontageEnded(bool bInterrupted)
{
	Super::OnSkillMontageEnded(bInterrupted);
	Destroy();
}


