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
}

void USkillSystemComponent::HandleBasicAttack()
{
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

        if (ComboData->ComboDelay > 0)
        {
            GetWorld()->GetTimerManager().SetTimer(
                ComboTimerHandle, this, &USkillSystemComponent::ResetCombo, ComboData->ComboDelay, false
            );
        }

        GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, FString::Printf(TEXT("Combo ID: %d"), CurrentComboID));
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

    if (ComboData->AnimMontage.IsValid())
    {
        UAnimMontage* Montage = ComboData->AnimMontage.LoadSynchronous();
        if (Montage)
        {
            UAnimInstance* Anim = OwnerChar->GetMesh()->GetAnimInstance();
            if (Anim)
            {
                Anim->Montage_Play(Montage);
            }
        }
    }
}

void USkillSystemComponent::ResetCombo()
{
    CurrentComboID = 0;
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Combo Reset"));
}

