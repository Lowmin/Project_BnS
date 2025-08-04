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
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RunSkill(ACharacter* Instigator, AActor* Target, FVector TargetLocation);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanRunSkill(ACharacter* Intigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetSkillCooldown();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetSkillDataHandle(const FDataTableRowHandle& SkillInfoHandle);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FSkillInformation GetSkillInfo();
};
