// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillController.h"

USkillController::USkillController()
{
}

void USkillController::InitializeController(const FSkillInformation& skillInfo, USkillBase* skillBase)
{
	mySkillInfo = skillInfo;
	linkedSkillBase = skillBase;
}

bool USkillController::ActivateSkill(ACharacter* instigator, AActor* target, FVector targetLocation)
{
	return false;
}

const FSkillInformation& USkillController::GetSkillInfo() const
{
	return mySkillInfo;
}
