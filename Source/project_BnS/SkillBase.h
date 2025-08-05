// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "SkillInformation.h"
#include "Castable.h"
#include "SkillBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECT_BNS_API USkillBase : public UObject, public ICastable
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Data")
	FDataTableRowHandle SkillInfoHandle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data")
	FSkillInformation CachedSkillInformation;

	void LoadSkillData();

public:
	USkillBase();

	virtual void ExecuteSkill_Implementation(ACharacter* Instigator, AActor* Target, FVector TargetLocation) override;
	virtual bool CanExecuteSkill_Implementation(ACharacter* Instigator) override;
	virtual void SetSkillDataHandle_Implementation(const FDataTableRowHandle& InSkillInfoHandle) override;
	virtual FSkillInformation GetSkillInformation_Implementation() override;

};
