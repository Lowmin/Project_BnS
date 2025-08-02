// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "SkillInformation.h"
#include "SkillBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECT_BNS_API USkillBase : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	FDataTableRowHandle skillDataHandle;

public:
	USkillBase();

	UFUNCTION(BlueprintCallable)
	void InitializeSkill(FDataTableRowHandle dataHandle);

	UFUNCTION(BlueprintCallable)
	const FSkillInformation& GetSkillInformation() const;

	UFUNCTION(BlueprintNativeEvent)
	void RunSkill(ACharacter* instigator, AActor* target, FVector targetLocation);
	virtual void RunSkill_Implementation(ACharacter* instigator, AActor* target, FVector targetLocation);

};
