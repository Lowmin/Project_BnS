// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeSkill.h"
#include "MeleeData.h"
#include "SkillCommonData.h"
#include "../CharacterBase.h"
#include "../StatComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h" 
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"

void AMeleeSkill::ExecuteSkill_Implementation()
{
	if (!LoadMeleeData()) return;

	if (UAnimInstance* Anim = GetOwnerAnimInstance())
	{
		Anim->OnPlayMontageNotifyBegin.RemoveDynamic(this, &AMeleeSkill::OnNotifyBegin); // 중복 방지
		Anim->OnPlayMontageNotifyBegin.AddDynamic(this, &AMeleeSkill::OnNotifyBegin);
	}

	Super::ExecuteSkill_Implementation();

	// 재생 실패 처리
	if (!bExecuting)
	{
		if (UAnimInstance* Anim = GetOwnerAnimInstance())
		{
			Anim->OnPlayMontageNotifyBegin.RemoveDynamic(this, &AMeleeSkill::OnNotifyBegin);
		}
	}
	FTimerHandle onceHandle;
	GetWorld()->GetTimerManager().SetTimer(onceHandle, this, &AMeleeSkill::AttackMelee, 0.20f, false);
}

bool AMeleeSkill::LoadMeleeData()
{
	if (!TypeHandle.DataTable) return false;

	const FMeleeData* Row = TypeHandle.GetRow<FMeleeData>(TEXT("Melee"));
	if (Row == nullptr) return false;

	AttackRange = Row->AttackRange;
	AttackRadius = Row->AttackRadius;
	bCanHitMultiTarget = Row->bCanHitMultiTarget;
	ComboStepLocal = Row->ComboStep;
	ChainInputSeconds = Row->ChainInput;
	RecoverMPOnThird = Row->RecoverMPOnThird;
	BossCCOnThird = Row->BossCCOnThird;
	NextComboRow = Row->NextComboRow;

	return true;
}


void AMeleeSkill::AttackMelee()
{
	ACharacter* Caster = GetOwnerCharacter();
	if (!Caster) return;

	const FVector Start = Caster->GetActorLocation();
	const FVector Dir = Caster->GetActorForwardVector();
	const FVector End = Start + Dir * AttackRange;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(AttackRadius);
	TArray<FHitResult> Hits;

	FCollisionQueryParams QP(SCENE_QUERY_STAT(MeleeTrace), false, Caster);

	bool Multi = GetWorld()->SweepMultiByChannel(Hits, Start, End, FQuat::Identity, ECC_Pawn, Sphere, QP);

	const float LifeTime = 1.5f;
	const bool  bDepth = false;
	const uint8 Thick = 0;
	const FColor PathCol = Multi ? FColor::Green : FColor::Red;

	DrawDebugSphere(GetWorld(), Start, AttackRadius, 16, PathCol, false, LifeTime, 0, 1.5f);

}

void AMeleeSkill::OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	if (NotifyName == FName("Hit"))
	{
		AttackMelee();
	}
}