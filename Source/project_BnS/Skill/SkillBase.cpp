// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillBase.h"
#include "../CharacterBase.h"
#include "../StatComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

ASkillBase::ASkillBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASkillBase::InitFromRow(const FSkillDataRow& InRow)
{
	MySkillID = InRow.SkillID;
	MySlot = InRow.Slot;
	MyBaseDamage = InRow.Damage;
	SavedTypeData = InRow.SkillTypeData;

	// 스킬 이펙트
	MyCastVFX = InRow.CastVFX;
	MyCastSound = InRow.CastSound;
	MyHitVFX = InRow.HitVFX;
	MyHitSound = InRow.HitSound;

	MyMontage = InRow.AnimMontage.IsNull() ? nullptr : InRow.AnimMontage.LoadSynchronous();
	if (!IsValid(MyMontage)) MyMontage = nullptr;
}

void ASkillBase::SetSkillTarget(AActor* InTarget)
{
	MyTarget = InTarget;
}

AActor* ASkillBase::GetSkillTarget() const
{
	return MyTarget.Get();
}

// 스킬 실행
void ASkillBase::ExecuteSkill_Implementation()
{
	if (bIsExecuting) return;

	ACharacter* OwnerCharacter = GetOwnerCharacter();
	if (OwnerCharacter)
	{
		if (MyCastVFX)
		{
			// 소켓 생성 시 위치 수정
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MyCastVFX, OwnerCharacter->GetActorLocation());
		}
		if (MyCastSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, MyCastSound, OwnerCharacter->GetActorLocation());
		}
	}

	if (MyMontage)
	{
		if (UAnimInstance* Anim = GetOwnerAnimInstance())
		{
			if (!Anim->Montage_IsPlaying(MyMontage))
			{
				Anim->Montage_Play(MyMontage, 1.0f);
			}
		}

		bIsExecuting = true;
	}
}

void ASkillBase::CancelSkill_Implementation()
{
	// 몽타주가 있으면 부드럽게 멈춤 (End 콜백이 들어올 것)
	bool bGetEnd = false;
	if (MyMontage)
	{
		if (UAnimInstance* Anim = GetOwnerAnimInstance())
		{
			Anim->Montage_Stop(0.15f, MyMontage);
			bGetEnd = true;
		}
	}

	if (!bGetEnd)
	{
		OnMontageEnded(nullptr, true);
	}
}

// 노티 처리
void ASkillBase::OnAnimNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	if (!MyMontage || Payload.SequenceAsset != MyMontage) return;

	if (NotifyName == TEXT("Hit"))
	{
		OnSkillNotify_Hit();	// 공격 판정 타이밍은 파생에서
		return;
	}

	OnSkillNotify_Custom(NotifyName);
}

// 몽타주 종료
void ASkillBase::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsExecuting = false;

	OnSkillMontageEnded(bInterrupted);
	OnEnded.Broadcast(MySlot, bInterrupted);

	Destroy();
}

ACharacter* ASkillBase::GetOwnerCharacter() const
{
	return Cast<ACharacter>(GetOwner());
}

UAnimInstance* ASkillBase::GetOwnerAnimInstance() const
{
	if (ACharacter* Character = GetOwnerCharacter())
	{
		if (USkeletalMeshComponent* SkMeshComp = Character->GetMesh())
		{
			return SkMeshComp->GetAnimInstance();
		}
	}
	return nullptr;
}

int32 ASkillBase::CalculateDamage() const
{
	const ACharacterBase* OwnerCharacter = Cast<ACharacterBase>(GetOwner());
	const UStatComponent* Stat = OwnerCharacter ? OwnerCharacter->GetStatusComponent() : nullptr;
	const float Atk = Stat ? Stat->GetAtk() : 1.f;

	const float Raw = MyBaseDamage * Atk;
	return FMath::Max(1, FMath::RoundToInt(Raw));
}

void ASkillBase::ApplyDamageToCharacter(ACharacter* DamagedCharacter) const
{
	if (!DamagedCharacter || DamagedCharacter == GetOwner()) return;

	if (ACharacterBase* Target = Cast<ACharacterBase>(DamagedCharacter))
	{
		const int32 Dmg = CalculateDamage();
		Target->OnDamaged(Dmg);
	}
}

