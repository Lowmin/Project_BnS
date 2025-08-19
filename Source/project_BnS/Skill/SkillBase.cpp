// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillBase.h"
#include "SkillCommonData.h"
#include "../CharacterBase.h"
#include "../StatComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"

ASkillBase::ASkillBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASkillBase::InitializeSkill(const FDataTableRowHandle& InCommonHandle, const FDataTableRowHandle& InTypeHandle, UAnimMontage* PreloadedMontage)
{
	CommonHandle = InCommonHandle;
	TypeHandle = InTypeHandle;
	SkillMontage = PreloadedMontage;
}

void ASkillBase::ExecuteSkill_Implementation()
{
	if (bIsExecuting || !SkillMontage) return;

	if (UAnimInstance* AnimInstance = GetOwnerAnimInstance())
	{
		bIsExecuting = true;
		FOnMontageEnded OnEndDelegate;
		OnEndDelegate.BindUObject(this, &ASkillBase::OnMontageEnded);
		AnimInstance->Montage_Play(SkillMontage);
		AnimInstance->Montage_SetEndDelegate(OnEndDelegate, SkillMontage);
	}
}

void ASkillBase::CancelSkill_Implementation()
{
	if (bIsExecuting && SkillMontage)
	{
		if (UAnimInstance* AnimInstance = GetOwnerAnimInstance())
		{
			AnimInstance->Montage_Stop(0.1f, SkillMontage);
		}
	}
	bIsExecuting = false;
}

void ASkillBase::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsExecuting = false;
	Destroy();
}

const FSkillCommonData* ASkillBase::GetCommonData() const
{
	return CommonHandle.GetRow<FSkillCommonData>(TEXT("ASkillBase::GetCommonData"));
}

int32 ASkillBase::CalculateDamage() const
{
	const FSkillCommonData* Data = GetCommonData();
	if (!Data) return 1; // 최소 대미지

	const ACharacterBase* OwnerChar = Cast<ACharacterBase>(GetOwner());
	const UStatComponent* StatComp = OwnerChar ? OwnerChar->GetStatusComponent() : nullptr;
	if (!StatComp) return 1; // 최소 대미지

	const float Atk = StatComp->GetAtk();

	const float RawDamage = Data->DamageFlat + (Data->DamageRatio * Atk);
	return FMath::Max(1, FMath::RoundToInt(RawDamage));
}

void ASkillBase::ApplyDamageToCharacter(ACharacter* DamagedCharacter) const
{
	if (!DamagedCharacter || DamagedCharacter == GetOwner()) return;

	if (ACharacterBase* HitCharacter = Cast<ACharacterBase>(DamagedCharacter))
	{
		const int32 FinalDamage = CalculateDamage();
		HitCharacter->OnDamaged(FinalDamage);
	}
}

ACharacter* ASkillBase::GetOwnerCharacter() const
{
	return GetOwner<ACharacter>();
}

UAnimInstance* ASkillBase::GetOwnerAnimInstance() const
{
	if (ACharacter* OwnerChar = GetOwnerCharacter())
	{
		if (USkeletalMeshComponent* Mesh = OwnerChar->GetMesh())
		{
			return Mesh->GetAnimInstance();
		}
	}
	return nullptr;
}

void ASkillBase::SetSkillTarget(AActor* InTarget)
{
	TargetActor = InTarget;
}

AActor* ASkillBase::GetSkillTarget() const
{
	return TargetActor.Get();
}
