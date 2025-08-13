// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillBase.h"
#include "SkillCommonData.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"

ASkillBase::ASkillBase()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ASkillBase::InitSkill(const FDataTableRowHandle& InCommonHandle, const FDataTableRowHandle& InTypeHandle)
{
	CommonHandle = InCommonHandle;
	TypeHandle = InTypeHandle;
}

void ASkillBase::InitSkillExecute_Implementation()
{
}

void ASkillBase::ExecuteSkill_Implementation()
{
	// 스킬 실행
	if (bExecuting) return;

	UAnimInstance* Anim = GetOwnerAnimInstance();
	if (!Anim) return;

	UAnimMontage* Montage = LoadMontage();
	if (!Montage) return;

	FOnMontageEnded OnEnd;
	OnEnd.BindUObject(this, &ASkillBase::OnMontageEnd);

	const float PlayLength = Anim->Montage_Play(Montage);

	if (PlayLength <= 0.f) return;

	Anim->Montage_SetEndDelegate(OnEnd, Montage);

	bExecuting = true;
}

void ASkillBase::CancelSkill_Implementation()
{
	UAnimInstance* Anim = GetOwnerAnimInstance();
	UAnimMontage* Mont = LoadMontage();
	if (Anim && Mont)
	{
		FOnMontageEnded Empty;
		Anim->Montage_SetEndDelegate(Empty, Mont);
		Anim->Montage_Stop(0.1f, Mont);
	}
	bExecuting = false;
	Destroy();
}

const FSkillCommonData* ASkillBase::GetCommonRow() const
{
	if (!CommonHandle.DataTable) return nullptr;

	return CommonHandle.GetRow<FSkillCommonData>(TEXT("SkillBase_CommonData"));
}

UAnimMontage* ASkillBase::LoadMontage() const
{
	const FSkillCommonData* Row = GetCommonRow();

	if (Row == nullptr) return nullptr;
	if (Row->AnimMontage.IsNull()) return nullptr;

	return Row->AnimMontage.LoadSynchronous();
}

ACharacter* ASkillBase::GetOwnerCharacter() const
{
	return Cast<ACharacter>(GetOwner());
}

UAnimInstance* ASkillBase::GetOwnerAnimInstance() const
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return nullptr;

	USkeletalMeshComponent* Mesh = OwnerCharacter->GetMesh();
	if (!Mesh) return nullptr;

	return Mesh->GetAnimInstance();
}

void ASkillBase::OnMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	bExecuting = false;

	//if (ACharacter* OwnerCharacter = GetOwnerCharacter())
	//{
	//	if (auto* Comp = OwnerCharacter->GetComponentByClass<USkill_System_Component>)
	//	{
	//		Comp->OnSkillMontageEnd();
	//	}
	//}

	Destroy();
}

void ASkillBase::SetSkillTarget(AActor* InTarget)
{
	TargetActor = InTarget;
}

AActor* ASkillBase::GetSkillTarget() const
{
	return TargetActor.Get();
}

