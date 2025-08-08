// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillSystemComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Engine/World.h"
#include "TimerManager.h"

USkillSystemComponent::USkillSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USkillSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ComboSkillDataTable)
	{
		TArray<FSkillInformation*> ComboSkills;
		ComboSkillDataTable->GetAllRows(TEXT("Combo Skills"), ComboSkills);

		for (FSkillInformation* Skill : ComboSkills)
		{
			if (!Skill->AnimMontage.IsNull())
			{
				UAnimMontage* LoadedMontage = Skill->AnimMontage.LoadSynchronous();
				if (LoadedMontage)
				{
					LoadedMontages.Add(FName(*FString::FromInt(Skill->Index)), LoadedMontage);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Montage Load Failed"))
				}
			}
		}
	}
}

void USkillSystemComponent::HandleBasicAttack()
{
	if (bIsComboPlaying)
	{
		bIsComboNextInput = true;   // 콤보 입력 대기 상태
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);
	CurrentComboID++;
	if (CurrentComboID > 3)
	{
		CurrentComboID = 1;
	}

	// 다음 콤보
	FSkillInformation* ComboData = ComboSkillDataTable->FindRow<FSkillInformation>(
		FName(*FString::FromInt(CurrentComboID)), TEXT("ComboLookup"));

	if (ComboData)
	{
		ExecuteComboAttack(ComboData);
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow,FString::Printf(TEXT("Combo ID: %d"), CurrentComboID));
	}
	else
	{
		ResetCombo();
	}
}

void USkillSystemComponent::ExecuteComboAttack(const FSkillInformation* ComboData)
{
	// 애니메이션 실행
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (!OwnerChar) return;

	UAnimInstance* AnimInstance = OwnerChar->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	UAnimMontage* Montage = LoadedMontages.FindRef(FName(*FString::FromInt(ComboData->Index)));
	if (Montage)
	{
		FOnMontageEnded BlendedOutDelegate;
		BlendedOutDelegate.BindUObject(this, &USkillSystemComponent::OnAttackMontageEnd);

		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_SetEndDelegate(BlendedOutDelegate);

		bIsComboPlaying = true;
		bIsComboNextInput = false;
	}
}

void USkillSystemComponent::ResetCombo()
{
	CurrentComboID = 0;
	bIsComboNextInput = false;
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Combo Reset"));
}

void USkillSystemComponent::OnAttackMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	bIsComboPlaying = false;
	if (!bInterrupted && bIsComboNextInput)
	{
		bIsComboNextInput = false;
		HandleBasicAttack();
		return;
	}

	if (LastComboDelay > 0.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &USkillSystemComponent::ResetCombo, LastComboDelay, false);
	}
	else
	{
		ResetCombo();
	}

}

