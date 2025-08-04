// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillBase.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/Character.h" 
#include "StatComponent.h" 

USkillBase::USkillBase()
{
    IsOnCooldown = false;
}

void USkillBase::RunSkill_Implementation(ACharacter* Instigator, AActor* Target, FVector TargetLocation)
{
    const FSkillInformation SkillInfo = GetSkillInfo_Implementation();
    UE_LOG(LogTemp, Log, TEXT("Skill ID: %d"), *SkillInfo.SkillName.ToString(), SkillInfo.Index);

    // 쿨타임
    if (SkillInfo.Cooldown > 0.0f && GetWorld()) 
    {
        IsOnCooldown = true;
        GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &USkillBase::OnCooldownFinished, SkillInfo.Cooldown, false);
    }
    
    // MP 사용
    if (Instigator)
    {
        UStatComponent* StatComp = Instigator->FindComponentByClass<UStatComponent>();
        if (StatComp)
        {
            StatComp->SetCurMp(StatComp->GetCurMp() - SkillInfo.CostMP);
        }
    }
}

// 스킬 사용 가능 여부 확인
bool USkillBase::CanRunSkill_Implementation(ACharacter* Instigator)
{
    // 쿨타임 체크
    if (IsOnCooldown)
    {
        UE_LOG(LogTemp, Warning, TEXT("쿨타임 - %s"), *GetSkillInfo_Implementation().SkillName.ToString());
        return false;
    }

    // 마나 체크
    if (Instigator) 
    {
        UStatComponent* StatComp = Instigator->FindComponentByClass<UStatComponent>();
        if (StatComp) 
        {
            if (StatComp->GetCurMp() < GetSkillInfo_Implementation().CostMP)
            {
                return false;
            }
        }
    }

    return true;
}

float USkillBase::GetSkillCooldown_Implementation()
{
    return GetSkillInfo_Implementation().Cooldown;
}

void USkillBase::SetSkillDataHandle_Implementation(const FDataTableRowHandle& InSkillInfoHandle) 
{
    SkillInfoHandle = InSkillInfoHandle;
}


FSkillInformation USkillBase::GetSkillInfo_Implementation()
{
    if (SkillInfoHandle.DataTable && !SkillInfoHandle.RowName.IsNone())
    {
        const FSkillInformation* FoundRow = SkillInfoHandle.DataTable->FindRow<FSkillInformation>(SkillInfoHandle.RowName, TEXT("USkillBase::GetSkillInfo"));
        if (FoundRow)
        {
            return *FoundRow;
        }
    }
    return FSkillInformation();
}

void USkillBase::OnCooldownFinished()
{
    IsOnCooldown = false;
}

