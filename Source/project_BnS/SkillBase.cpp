// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillBase.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"

USkillBase::USkillBase()
{
}

void USkillBase::InitializeSkill(const FSkillInformation& skillInfo)
{
	mySkillInfo = skillInfo;
}

const FSkillInformation& USkillBase::GetSkillInfo() const
{
	return mySkillInfo;
}

void USkillBase::RunSkill_Implementation(ACharacter* instigator, AActor* target, FVector targetLocation)
{

}

