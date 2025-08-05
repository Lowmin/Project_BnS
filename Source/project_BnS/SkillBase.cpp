// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillBase.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/Character.h" 


USkillBase::USkillBase()
{
}

void USkillBase::LoadSkillData()
{
    if (SkillInfoHandle.DataTable && !SkillInfoHandle.RowName.IsNone())
    {
        FSkillInformation* RowData = 
            SkillInfoHandle.DataTable->FindRow<FSkillInformation>(SkillInfoHandle.RowName, TEXT("LoadSkillData"));
        if (RowData)
        {
            CachedSkillInformation = *RowData;
        }
        else
        {
            CachedSkillInformation = FSkillInformation();
        }
    }
}

void USkillBase::ExecuteSkill_Implementation(ACharacter* Instigator, AActor* Target, FVector TargetLocation)
{
    // 스킬 효과 구현
}

bool USkillBase::CanExecuteSkill_Implementation(ACharacter* Instigator)
{
    // 스킬 사용 가능 여부
    return true;
}

void USkillBase::SetSkillDataHandle_Implementation(const FDataTableRowHandle& InSkillInfoHandle) 
{
    SkillInfoHandle = InSkillInfoHandle;
    LoadSkillData();
}


FSkillInformation USkillBase::GetSkillInformation_Implementation()
{
    if (CachedSkillInformation.SkillName.IsEmpty() || SkillInfoHandle.RowName == NAME_None || SkillInfoHandle.DataTable == nullptr)
    {
        LoadSkillData();
    }
    return CachedSkillInformation;
}


