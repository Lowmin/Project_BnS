// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SkillInformation.h"
#include "Castable.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI)
class UCastable : public UInterface
{
	GENERATED_BODY()
};

class PROJECT_BNS_API ICastable
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Skill")
	void ExecuteSkill(ACharacter* Instigator, AActor* Target, FVector TargetLocation);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Skill")
	bool CanExecuteSkill(ACharacter* Intigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Skill")
	void SetSkillDataHandle(const FDataTableRowHandle& SkillInfoHandle);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Skill")
	FSkillInformation GetSkillInformation();
};
