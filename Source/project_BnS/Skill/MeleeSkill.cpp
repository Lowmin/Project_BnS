// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeSkill.h"
#include "MeleeData.h"
#include "SkillCommonData.h"

#include "../CharacterBase.h"
#include "../StatComponent.h"

#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "../Enemy.h"

void AMeleeSkill::ExecuteSkill_Implementation()
{
	if (!CacheMeleeData())
	{
		Destroy();
		return;
	}

	if (UAnimInstance* Anim = GetOwnerAnimInstance())
	{
		Anim->OnPlayMontageNotifyBegin.RemoveDynamic(this, &AMeleeSkill::OnNotifyBegin);
		Anim->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &AMeleeSkill::OnNotifyBegin);
	}
	else
	{
		Destroy();
		return;
	}

	Super::ExecuteSkill_Implementation();

	if (!bIsExecuting)
	{
		CleanUp();
	}
}

void AMeleeSkill::CancelSkill_Implementation()
{
	Super::CancelSkill_Implementation();
	CleanUp();
}

void AMeleeSkill::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	CleanUp();
	Super::OnMontageEnded(Montage, bInterrupted);
}

void AMeleeSkill::CleanUp()
{
	if (UAnimInstance* Anim = GetOwnerAnimInstance())
	{
		Anim->OnPlayMontageNotifyBegin.RemoveDynamic(this, &AMeleeSkill::OnNotifyBegin);
	}
}

bool AMeleeSkill::CacheMeleeData()
{
	if (!TypeHandle.DataTable) return false;

	MeleeData = TypeHandle.GetRow<FMeleeData>(TEXT("CacheMeleeData"));

	return MeleeData != nullptr;
}


void AMeleeSkill::OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	if (NotifyName == TEXT("Hit"))
	{
		PerformMeleeAttack();
	}
}

void AMeleeSkill::PerformMeleeAttack()
{
	ACharacterBase* Caster = Cast<ACharacterBase>(GetOwnerCharacter());
	if (!Caster || !MeleeData) return;

	const FVector Start = Caster->GetActorLocation();
	const FVector Dir = Caster->GetActorForwardVector();
	const FVector End = Start + Dir * MeleeData->AttackRange;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(MeleeData->AttackRadius);
	TArray<FHitResult> Hits;
	FCollisionQueryParams QP(SCENE_QUERY_STAT(MeleeTrace), false, Caster);

	FCollisionObjectQueryParams ObjectParam;
	ObjectParam.AddObjectTypesToQuery(ECC_Pawn);
	ObjectParam.AddObjectTypesToQuery(ECC_GameTraceChannel6);

	bool bHitSomething = GetWorld()->SweepMultiByObjectType(Hits, Start, End, FQuat::Identity, ObjectParam, Sphere, QP);

	DrawDebugSphere(GetWorld(), Start, MeleeData->AttackRadius, 16, bHitSomething ? FColor::Green : FColor::Red, false, 1.5f, 0, 1.5f);

	TSet<AActor*> DamagedActors;
	bool bHitActiveEnemy = false;
	for (const FHitResult& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor || HitActor == Caster || DamagedActors.Contains(HitActor))
		{
			continue;
		}

		if (AEnemy* HitEnemy = Cast<AEnemy>(HitActor))
		{
			if (!HitEnemy->IsDead())
			{
				bHitActiveEnemy = true;
			}

		}

		ApplyDamageToCharacter(Cast<ACharacter>(HitActor));
		DamagedActors.Add(HitActor);

		if (!MeleeData->bCanHitMultiTarget) break;
	}

}
