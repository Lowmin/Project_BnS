// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillBase.h"
#include "SkillInformation.h"
#include "SkillController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BNS_API USkillController : public UObject
{
	GENERATED_BODY()
	
public:
	USkillController();
	
	UFUNCTION(BlueprintCallable)
	void InitializeController(const FSkillInformation& skillInfo, USkillBase* skillBase);

	UFUNCTION(BlueprintCallable)
	bool ActivateSkill(ACharacter* instigator, AActor* target, FVector targetLocation);

	UFUNCTION(BlueprintCallable)
	const FSkillInformation& GetSkillInfo() const;

protected:
	UPROPERTY()
	FSkillInformation mySkillInfo;
	UPROPERTY()
	USkillBase* linkedSkillBase;
};
