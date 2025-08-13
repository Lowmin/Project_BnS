// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SkillInterface.generated.h"


UINTERFACE(BlueprintType)
class PROJECT_BNS_API USkillInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT_BNS_API ISkillInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Skill")
	void InitSkillExecute();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Skill")
	void ExecuteSkill();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Skill")
	void CancelSkill();

};
