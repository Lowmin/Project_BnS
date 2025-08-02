// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillBase.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"

USkillBase::USkillBase()
{
}

void USkillBase::InitializeSkill(FDataTableRowHandle data)
{
	skillDataHandle = data;

	if (!skillDataHandle.DataTable)
		return;
}

const FSkillInformation& USkillBase::GetSkillInformation() const
{
	if (!skillDataHandle.DataTable || skillDataHandle.RowName.IsNone())
	{
		static const FSkillInformation emptySkillInfo = FSkillInformation();
		return emptySkillInfo;
	}

	const FSkillInformation* findSkillInfo = skillDataHandle.DataTable->FindRow<FSkillInformation>(skillDataHandle.RowName, TEXT("USkillBase::GetSkillInformation"));

	if (findSkillInfo)
	{
		return *findSkillInfo;
	}
	else
	{
		static const FSkillInformation emptySkillInfo = FSkillInformation();
		return emptySkillInfo;
	}
}

void USkillBase::RunSkill_Implementation(ACharacter* instigator, AActor* target, FVector targetLocation)
{
}

