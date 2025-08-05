// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillController.h"
#include "GameFramework/Character.h"

USkillController::USkillController()
{
	CooldownEndTime = 0.f;
	SkillBaseInstance = nullptr;
	OwnerInstigator = nullptr;
}

void USkillController::Initialize(ACharacter* InInstigator, const FDataTableRowHandle& InDataHandle)
{
	OwnerInstigator = InInstigator; // ½ÃÀüÀÚ
	if (!SkillBaseClass)
		return;

	SkillBaseInstance = NewObject<USkillBase>(this, SkillBaseClass);
	if (SkillBaseInstance)
	{
		SkillBaseInstance->SetSkillDataHandle_Implementation(InDataHandle);
	}
}

bool USkillController::TryExecuteSkill(AActor* Target, FVector TargetLocation)
{

	return true;
}

void USkillController::StartCooldown()
{
	float Cooldown = GetSkillInformation().Cooldown;
	if (Cooldown > 0.f && GetWorld())
	{
		CooldownEndTime = GetWorld()->GetTimeSeconds() + Cooldown;
	}
}

bool USkillController::IsCooldown() const
{
	if (!GetWorld()) return false;
	return GetWorld()->GetTimeSeconds() < CooldownEndTime;
}

bool USkillController::IsEnoughMP() const
{
	return GetSkillInformation().CostMP <= 0;
}

FSkillInformation USkillController::GetSkillInformation() const
{
	return FSkillInformation();
}
